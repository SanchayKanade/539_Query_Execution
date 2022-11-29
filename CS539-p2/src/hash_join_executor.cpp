#include "../include/hash_join_executor.h"

HashJoinExecutor::HashJoinExecutor(AbstractExecutor *left_child_executor,
                                   AbstractExecutor *right_child_executor,
                                   SimpleHashFunction *hash_fn)
    : left_(left_child_executor),
      right_(right_child_executor),
      hash_fn_(hash_fn) {}

void HashJoinExecutor::Init(){
  //Initializing executors
  left_->Init();
  right_->Init();

  //To traverse left table
  Tuple tuple_left;

  /*It seems the hs table is not getting overwritten so the hash table keeps on appending on every next.
  Hence reinitializing ht*/
  SimpleHashJoinHashTable refresh;
  ht = refresh;

  //Creating hash table from left table during initialization so that we can use it in the next() later
  while(left_->Next(&tuple_left)){
    //Generating hash value
    hash_t hashValue = hash_fn_->GetHash(tuple_left);

    //Populating hash table
    ht.Insert(hashValue, tuple_left);

  }

  //Initializing index as 0
  index = 0;

  //Initializing goToNext as true since intially we have to get a new row from the right table
  goToNext = true;
};

bool HashJoinExecutor::Next(Tuple *tuple) { 
  //If the tupleVector has only one matching elemnet then we need to go to the next tuple in right table      
  if(goToNext){
    //Object to parse right table
    Tuple tuple_right;

    //Traversing right table
    while(right_->Next(&tuple_right)){
    
    //Calculating hash value for the right table
    hash_t hashValue = hash_fn_->GetHash(tuple_right);

    //Storing tuples which are matching in hash value in the tupleVector
    ht.GetValue(hashValue, &tupleVector);

    /*tupleVector can have 
        1. only one matched tuple, then just return the first and only value in the vector
        2. Multiple matched tuple, then we need to return each value before going to next tuple in right table
        3. Nothing as no match was found, return false and move to next tuple*/
    if(!tupleVector.empty()){
      
      if(tupleVector.size()>1){
        /*Multiple vectors for the same hash value hence we need to return them first then move to next
        tuple in right table*/ 
        goToNext = false;

        /*Set the index to 1 as we will return the front of the vector now and return rest vectors in 
        subsequent next() calls*/ 
        index = 1;
        
      }

      //Return the first vector in the tuple
      *tuple = tupleVector.front();
      return true;
    }
    
  }
  //If no matches found for this particular right tuple
  return false;
  } 

  /*If multiple matches are founda and tupleVector has multiple elements then before going to next row in right table
  we return all vectors for this particular tuple*/

  else{
    //Keep on increasing the index in each next call till we reach the end of the vector 
    index ++;

    //If all the vectors are exhausted for a particular right tuple
    if(index == tupleVector.size()){
      //Move to the next right tuple from right table in the following next call
      goToNext = true;

    }

    *tuple = Tuple(tupleVector[index-1]);
    return true;
  }

}
