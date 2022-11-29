#include "../include/filter_seq_scan_executor.h"

FilterSeqScanExecutor::FilterSeqScanExecutor(Table *table,
                                             FilterPredicate *pred)
    : table_(table), pred_(pred){};

void FilterSeqScanExecutor::Init() {
    //Setting the iter to the top or beginnig of the table
    iter_ = table_->Begin();
    }

bool FilterSeqScanExecutor::Next(Tuple *tuple) { 
    //int x = tuple->val1;

    //Traversing the table
    while (iter_ != table_->End()){
        
        const Tuple &curr_tuple = *iter_;

        //Creating and initializing predicate object to test our conditions
        FilterPredicate f(pred_->val, pred_->condition);

        /*evaluate() function will test if the condition is satisfied
        or not*/
        if(f.evaluate(curr_tuple)){
            //If condition is satisfied then return trur and set tuple to current tuple
            *tuple = Tuple(curr_tuple);
            ++iter_;
            return true;
        }
        //Increasing iter to move to next tuple
        ++iter_;
        
    }
//If condition is not satisfied then return false    
return false; 
}