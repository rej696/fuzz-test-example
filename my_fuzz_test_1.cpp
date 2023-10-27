#include <assert.h>

#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "src/sut.h"
#include <stdint.h>

FUZZ_TEST_SETUP() {
  // Perform any one-time setup required by the FUZZ_TEST function.
}

FUZZ_TEST(const uint8_t *data, size_t size) {
    struct output output;
    struct state state;
    output_reset(&output);
    state_reset(&state);
    uint32_t expected_size = FRAME_LEN;

    /* Fuzzed (input) Data */
    FuzzedDataProvider fuzzed_data(data, size);
    state.start_found = fuzzed_data.ConsumeBool();
    state.total_size = fuzzed_data.ConsumeIntegral<uint32_t>();
    uint16_t input_size = fuzzed_data.ConsumeIntegral<uint16_t>();
    if (input_size > BUFFER_ROWS) input_size = BUFFER_ROWS;
    if (input_size > 0) {
        uint8_t input[input_size];
        fuzzed_data.ConsumeData(&input, input_size);

        bool error = separate_data_frame(
            input,
            input_size,
            expected_size,
            &state,
            &output);
    }
}
