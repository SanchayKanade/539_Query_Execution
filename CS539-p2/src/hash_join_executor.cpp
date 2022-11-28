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
  Tuple tuple_right;
  
  
  if(goToNext){
    
    while(right_->Next(&tuple_right)){
    //calculating hash value for the right table
    hash_t hashValue1 = hash_fn_->GetHash(tuple_right);

    //Storing tuples which are matching in hash value in the tupleVector
    ht.GetValue(hashValue1, &tupleVector);
    if(tupleVector.size()>1){
      count = 1;
      goToNext = false;
    }
    *tuple = Tuple(tupleVector.front());
    return true;
  }
  return false;
  } 

  else{
    count ++;
    if(count == tupleVector.size()){
      goToNext = true;
    }

    *tuple = Tuple(tupleVector[count]);
    return true;
  }

}
