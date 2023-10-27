#include <assert.h>

#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "src/sut.h"
#include <stdint.h>

FUZZ_TEST_SETUP() {
  // Perform any one-time setup required by the FUZZ_TEST function.
}

FUZZ_TEST(const uint8_t *data, size_t size) {
    /* Fixed (output) data */
    uint8_t remaining_buffer[BUFFER_ROWS] = {0};
    uint8_t output_buffer[BUFFER_ROWS] = {0};
    uint16_t remaining_size = 0;
    uint16_t output_size = 0;
    uint32_t expected_size = FRAME_LEN;
    FuzzedDataProvider fuzzed_data(data, size);
    bool start_found = fuzzed_data.ConsumeBool();
    uint32_t total_size = fuzzed_data.ConsumeIntegral<uint32_t>();
    uint16_t input_size = fuzzed_data.ConsumeIntegral<uint16_t>();
    if (input_size > BUFFER_ROWS) input_size = BUFFER_ROWS;
    if (input_size > 0) {
        uint8_t input[input_size];
        fuzzed_data.ConsumeData(&input, input_size);

        uint16_t error = separate_data_frame_old(
            input,
            input_size,
            remaining_buffer,
            &remaining_size,
            output_buffer,
            &start_found,
            &total_size,
            expected_size);
    }
}
