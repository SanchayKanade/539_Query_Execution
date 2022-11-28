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
  //Setting left and right executors to the top of the table
  left_->Init();
  right_->Init();
}

bool NestedLoopJoinExecutor::Next(Tuple *tuple) { 
  //Object of tuple class to parse left and right table respectively
  Tuple tuple_left;
  Tuple tuple_right;

  /*If a tuple is found in the right table and we need to travese rest of the rows in the right table for the 
  same left tuple then flag will be set.*/
  if(flag == true){
    //Parsing right table but after the previously found tuple as right_ is not yet reinitialized
    while(right_->Next(&tuple_right)){
      //Case when join_id_ is id
      if(join_key_ == "id"){
        if(temp.id == tuple_right.id){
          *tuple = temp;
          return true;
        }
      }
      //Case when join_id_ is val1
      else if(join_key_ == "val1"){
        if(temp.val1 == tuple_right.val1){
          *tuple = temp;
          return true;
        }
      }
      //Case when join_id_ is val2
      else if(join_key_ == "val2"){
        if(temp.val2 == tuple_right.val2){
          *tuple =temp;
          return true;
        }
      }
      //Case when join id is not valid
      else{
        cout<<"Not a valid join id."<<endl;
        return false;
      }  
    }
    //Setting the flag false now so that we can get to the start of the right table
    flag = false;
  }

  while (left_->Next(&tuple_left)){ 
    /*Temp tuple stores the current tuple value of the left tuple so that we can use it to 
    traverse the rest of the rows of the right table if a row is found*/
    temp = tuple_left;
    
    //Reinitilizing right_ so that we can traverse from the top for a new match
    right_->Init();
    while (right_->Next(&tuple_right)){     
      //Case when join_id_ is id
      if(join_key_ == "id"){
        //If tuple is found then set the flag so that in the following next we can traverse rest of the rows first
        if(tuple_left.id == tuple_right.id){
          *tuple = tuple_left;
          flag = true;
          return true;
        }
        
      }

      //Case when join_id_ is val1
      else if (join_key_=="val1"){ 
        //If tuple is found then set the flag so that in the following next we can traverse rest of the rows first
        if(tuple_left.val1 == tuple_right.val1){
          *tuple = tuple_left;
          flag = true;
          return true;
        }
      }

      //Case when join_id_ is val2
      else if(join_key_ == "val2"){
        //If tuple is found then set the flag so that in the following next we can traverse rest of the rows first
        if(tuple_left.val2 == tuple_right.val2){
          *tuple = tuple_left;
          flag = true;
          return true;
        }
      }

      //Case if join_id is something else
      else{
        cout<<"Not a valid join id"<<endl;
        return false;
      }
    }
  }
  //If after traversing all rows in the right table we are not able to find a match then return false
  return false;
}
   