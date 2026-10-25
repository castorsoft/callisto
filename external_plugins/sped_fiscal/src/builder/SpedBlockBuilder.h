#pragma once

#include <vector>
#include <memory>
#include "../collector/SpedDataCollector.h"
#include "../records/SpedRecordBase.h"

class SpedBlockBuilder {
public:
    static std::vector<std::shared_ptr<SpedRecordBase>> build(const SpedCollectedData &data);
};

