#pragma once
#include <vector>
#include <cstdint>

typedef struct
{

    int start_value;

    std::vector<uint64_t> compressed_data;

} CompressedData;
