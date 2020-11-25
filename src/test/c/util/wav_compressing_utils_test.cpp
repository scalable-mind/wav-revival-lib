#include <catch2/catch.hpp>

#include <util/wav_compressing_utils.hpp>
#include <domain/compressed_data.hpp>

TEST_CASE("WavCompressingUtils::compress_smooth_amp_chunk - test #1", "[WavCompressingUtils]") {
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

    CompressedData cdata{};

    bool fill = false;

    Stfv prev = STFV_INIT;

    size_t last = 0;

    for (int i = 0; i < 12; i++) {
        wav_compressing_utils::compress_smooth_amp_chunk(arr[i], 10, (i == 0), &fill, 3, 9, &prev, &last, &cdata);
    }

    if (last > 0) {
        if (fill) {
            cdata.compressed_data.back() += last;
        } else {
            cdata.compressed_data.push_back(last);
        }
    }

    bool success = true;

    if (cdata.compressed_data.size() != 6) {
        FAIL();
    }

    for (size_t i = 0; i < cdata.compressed_data.size(); i++) {
        success = success && (cdata.compressed_data[i] == expected[i]);
    }

    REQUIRE(success);
}

TEST_CASE("WavCompressingUtils::compress_smooth_amp_chunk - test #2", "[WavCompressingUtils]") {
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

    CompressedData cdata{};

    bool fill = false;

    Stfv prev = STFV_INIT;

    size_t last = 0;

    for (int i = 0; i < 12; i++) {
        wav_compressing_utils::compress_smooth_amp_chunk(arr[i], 10, (i == 0), &fill, 3, 9, &prev, &last, &cdata);
    }

    if (last > 0) {
        if (fill) {
            cdata.compressed_data.back() += last;
        } else {
            cdata.compressed_data.push_back(last);
        }
    }

    bool success = true;

    if (cdata.compressed_data.size() != 8) {
        FAIL();
    }

    for (size_t i = 0; i < cdata.compressed_data.size(); i++) {
        success = success && (cdata.compressed_data[i] == expected[i]);
    }

    REQUIRE(success);
}

TEST_CASE("WavCompressingUtils::compress_smooth_amp_chunk - test #3", "[WavCompressingUtils]") {
    int16_t arr[24][5] =
            {{0, 0, 0, 0, 0}, {5, 5, 5, 5, 5}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
             {0, 0, 5, 5, 5}, {5, 5, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0}, {0, 0, 5, 5, 5}, {5, 5, 0, 0, 0}, {0, 0, 5, 5, 5},
             {5, 5, 0, 0, 0}, {0, 0, 5, 5, 5}, {5, 5, 5, 5, 5}, {5, 5, 5, 5, 0},
             {0, 0, 0, 0, 5}, {5, 5, 5, 5, 5}, {5, 5, 5, 5, 0}, {0, 0, 0, 0, 5},
             {5, 5, 5, 5, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 5}, {5, 5, 5, 0, 0}};

    size_t expected[9] = {5, 5, 12, 5, 10, 67, 10, 4, 2};

    CompressedData cdata{};

    bool fill = false;

    Stfv prev = STFV_INIT;

    size_t last = 0;

    for (int i = 0; i < 24; i++) {
        wav_compressing_utils::compress_smooth_amp_chunk(arr[i], 5, (i == 0), &fill, 3, 9, &prev, &last, &cdata);
    }

    if (last > 0) {
        if (fill) {
            cdata.compressed_data.back() += last;
        } else {
            cdata.compressed_data.push_back(last);
        }
    }

    bool success = true;

    if (cdata.compressed_data.size() != 9) {
        FAIL();
    }

    for (size_t i = 0; i < cdata.compressed_data.size(); i++) {
        success = success && (cdata.compressed_data[i] == expected[i]);
    }

    REQUIRE(success);
}
