#include "../include/filter_seq_scan_executor.h"

FilterSeqScanExecutor::FilterSeqScanExecutor(Table *table,
                                             FilterPredicate *pred)
    : table_(table), pred_(pred){};

void FilterSeqScanExecutor::Init() {iter_ = table_->Begin();}

bool FilterSeqScanExecutor::Next(Tuple *tuple) { 
    //int x = tuple->val1;
    while (iter_ != table_->End()){
        const Tuple &curr_tuple = *iter_;

        FilterPredicate f(pred_->val, pred_->condition);
        if(f.evaluate(curr_tuple)){
            *tuple = Tuple(curr_tuple);
            ++iter_;
            return true;
        }
        ++iter_;
        
    }
    
    return false; 
    }