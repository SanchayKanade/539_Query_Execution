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

  //It seems the hs table is not getting overwritten so the table keeps on appending
  //Reinitializing ht
  SimpleHashJoinHashTable refresh;
  ht = refresh;
  //Creating hash table from left table during initialization so that we can use it int he next later
  while(left_->Next(&tuple_left)){
    //Generating hash value
    hash_t hashValue = hash_fn_->GetHash(tuple_left);

    //Populating hash table
    ht.Insert(hashValue, tuple_left);

  }
  count = 0;
  goToNext = true;
};

bool HashJoinExecutor::Next(Tuple *tuple) { 
  //Object of class tuple to parse right table and probing hash table
  //Tuple tuple_left;
  
  
  if(goToNext){
    Tuple tuple_right;
    while(right_->Next(&tuple_right)){
    //calculating hash value for the right table
    hash_t hashValue = hash_fn_->GetHash(tuple_right);

    //Storing tuples which are matching in hash value in the tupleVector
    ht.GetValue(hashValue, &tupleVector);
    if(!tupleVector.empty()){
      Tuple res = tupleVector.front();
      if(tupleVector.size()>1){
        count = 1;
        goToNext = false;
      }
      *tuple = Tuple(res.id, res.val1, res.val2);
      return true;
    }
    
  }
  return false;
  } 

  else{
    count ++;
    if(count == tupleVector.size()){
      goToNext = true;
    }

    *tuple = Tuple(tupleVector[count-1]);
    return true;
  }

}
