#include "../include/nested_loop_join_executor.h"
#include <iostream>

using std::cout;
using std::endl;

NestedLoopJoinExecutor::NestedLoopJoinExecutor(
    AbstractExecutor *left_child_executor,
    AbstractExecutor *right_child_executor, const std::string join_key)
    : left_(left_child_executor),
      right_(right_child_executor),
      join_key_(join_key){};

void NestedLoopJoinExecutor::Init() {
  left_->Init();
  right_->Init();

}

bool NestedLoopJoinExecutor::Next(Tuple *tuple) { 
  //Object of class tuple
  Tuple tuple_left;
  Tuple tuple_right;

  //Case when join_key_ is id
  if(join_key_ == "id"){
    while(left_->Next(&tuple_left)){
      while(right_->Next(&tuple_right)){
        if(tuple_left.id == tuple_right.id){
          *tuple = tuple_left;
          return true;
        }
      }
      //Because right executor reaches the bottom of the table then we need to set it again
      right_->Init();
    }
    return false;
  }

  //Case when join_key_ is val1
  if(join_key_ == "val1"){
      while(left_->Next(&tuple_left)){
        while(right_->Next(&tuple_right)){
          if(tuple_left.val1 == tuple_right.val1){
            *tuple = tuple_left;
            return true;
          }
        }
        //Because right executor reaches the bottom of the table then we need to set it again
        right_->Init();
      }
  return false;
  }

  //Case when join_key_ is val2
  else if(join_key_ == "val2"){
      while(left_->Next(&tuple_left)){
        while(right_->Next(&tuple_right)){
          if(tuple_left.val2 == tuple_right.val2){
            *tuple = tuple_left;
            return true;
          }
        }
        //Because right executor reaches the bottom of the table then we need to set it again
        right_->Init();
      }
  return false;
  }

  //Case when join_key_ is not a valid value
  else{
    cout<<"Not a valid join_key."<<endl;
    return false;
  }
}
   