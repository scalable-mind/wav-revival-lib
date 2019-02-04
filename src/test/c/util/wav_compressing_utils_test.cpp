#include <catch2/catch.hpp>

#include <api/uint64_array_api.h>
#include <util/wav_compressing_utils.h>
#include <domain/compressed_data.h>
#include <domain/uint64_array.h>


TEST_CASE("WavCompressingUtils::compress_smooth_rsp_chunk - test #1", "[WavCompressingUtils]") {
    int16_t arr[12][10]  =
            {{0, 0, 0, 0, 0, 5, 5, 5, 5, 5},
             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 5, 5, 5, 5, 5, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0, 0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0, 0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0, 0, 0, 5, 5, 5},
             {5, 5, 5, 5, 5, 5, 5, 5, 5, 0},
             {0, 0, 0, 0, 5, 5, 5, 5, 5, 5},
             {5, 5, 5, 5, 0, 0, 0, 0, 0, 5},
             {5, 5, 5, 5, 0, 0, 0, 0, 0, 5},
             {5, 5, 5, 5, 5, 5, 5, 5, 5, 5}};

    size_t expected[6] = {5, 5, 12, 5, 10, 83};

    CompressedData cdata;
    cdata.compressed_data=uint64_array_api()->init(0);

    bool fill = false;

    Stfv prev = STFV_INIT;

    size_t last = 0;

    for (int i = 0; i < 12; i++) {
        wav_compressing_utils()->compress_smooth_rsp_chunk(arr[i], 10, (i == 0), &fill, 3, 9, &prev, &last, &cdata);
    }

    if (last > 0) {
        if (fill) {
            *(uint64_array_api()->end(cdata.compressed_data) - 1) += last;
        } else {
            uint64_array_api()->push_back(cdata.compressed_data, last);
        }
    }

    bool success = true;

    if (cdata.compressed_data->_size != 6) {
        uint64_array_api()->del(cdata.compressed_data);
        FAIL();
    }

    for (size_t i = 0; i < cdata.compressed_data->_size; i++) {
        success = success && (cdata.compressed_data->_data[i] == expected[i]);
    }

    uint64_array_api()->del(cdata.compressed_data);

    REQUIRE(success);
}

TEST_CASE("WavCompressingUtils::compress_smooth_rsp_chunk - test #2", "[WavCompressingUtils]") {
    int16_t arr[12][10] =
            {{0, 0, 0, 0, 0, 5, 5, 5, 5, 5},
             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 5, 5, 5, 5, 5, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0, 0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0, 0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0, 0, 0, 5, 5, 5},
             {5, 5, 5, 5, 5, 5, 5, 5, 5, 0},
             {0, 0, 0, 0, 5, 5, 5, 5, 5, 5},
             {5, 5, 5, 5, 0, 0, 0, 0, 0, 5},
             {5, 5, 5, 5, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 5, 5, 5, 5, 5, 5}};

    size_t expected[8] = {5, 5, 12, 5, 10, 67, 10, 6};

    CompressedData cdata;
    cdata.compressed_data=uint64_array_api()->init(0);

    bool fill = false;

    Stfv prev = STFV_INIT;

    size_t last = 0;

    for (int i = 0; i < 12; i++) {
        wav_compressing_utils()->compress_smooth_rsp_chunk(arr[i], 10, (i == 0), &fill, 3, 9, &prev, &last, &cdata);
    }

    if (last > 0) {
        if (fill) {
            *(uint64_array_api()->end(cdata.compressed_data) - 1) += last;
        } else {
            uint64_array_api()->push_back(cdata.compressed_data, last);
        }
    }

    bool success = true;

    if (cdata.compressed_data->_size != 8) {
        uint64_array_api()->del(cdata.compressed_data);
        FAIL();
    }

    for (size_t i = 0; i < cdata.compressed_data->_size; i++) {
        success = success && (cdata.compressed_data->_data[i] == expected[i]);
    }

    uint64_array_api()->del(cdata.compressed_data);

    REQUIRE(success);
}

TEST_CASE("WavCompressingUtils::compress_smooth_rsp_chunk - test #3", "[WavCompressingUtils]") {
    int16_t arr[24][5] =
            {{0, 0, 0, 0, 0}, {5, 5, 5, 5, 5}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
             {0, 0, 5, 5, 5}, {5, 5, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0}, {0, 0, 5, 5, 5}, {5, 5, 0, 0, 0}, {0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0}, {0, 0, 5, 5, 5}, {5, 5, 5, 5, 5}, {5, 5, 5, 5, 0},
             {0, 0, 0, 0, 5}, {5, 5, 5, 5, 5}, {5, 5, 5, 5, 0}, {0, 0, 0, 0, 5},
             {5, 5, 5, 5, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 5}, {5, 5, 5, 0, 0}};

    size_t expected[9] = {5, 5, 12, 5, 10, 67, 10, 4, 2};

    CompressedData cdata;
    cdata.compressed_data=uint64_array_api()->init(0);

    bool fill = false;

    Stfv prev = STFV_INIT;

    size_t last = 0;

    for (int i = 0; i < 24; i++) {
        wav_compressing_utils()->compress_smooth_rsp_chunk(arr[i], 5, (i == 0), &fill, 3, 9, &prev, &last, &cdata);
    }

    if (last > 0) {
        if (fill) {
            *(uint64_array_api()->end(cdata.compressed_data) - 1) += last;
        } else {
            uint64_array_api()->push_back(cdata.compressed_data, last);
        }
    }

    bool success = true;

    if (cdata.compressed_data->_size != 9) {
        uint64_array_api()->del(cdata.compressed_data);
        FAIL();
    }

    for (size_t i = 0; i < cdata.compressed_data->_size; i++) {
        success = success && (cdata.compressed_data->_data[i] == expected[i]);
    }

    uint64_array_api()->del(cdata.compressed_data);

    REQUIRE(success);
}
