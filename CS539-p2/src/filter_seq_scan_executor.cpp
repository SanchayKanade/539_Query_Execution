#include "../include/filter_seq_scan_executor.h"

FilterSeqScanExecutor::FilterSeqScanExecutor(Table *table,
                                             FilterPredicate *pred)
    : table_(table), pred_(pred){};

void FilterSeqScanExecutor::Init() {}

bool FilterSeqScanExecutor::Next(Tuple *tuple) { return false; }
