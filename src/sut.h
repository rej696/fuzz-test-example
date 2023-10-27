/**
 * @file sut.h
 * @brief System under test
 */

#ifndef SUT_H_
#define SUT_H_
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BUFFER_ROWS (800)
#define BUFFER_SIZE (100)
#define FRAME_LEN (381)

struct state {
    uint8_t polled[BUFFER_ROWS];
    uint8_t dumped[BUFFER_ROWS];
    uint16_t polled_head;
    uint16_t dumped_head;
    bool start_found;
    uint32_t total_size;
    uint32_t expected_size;
};

struct buffer {
    uint8_t buffer[BUFFER_ROWS];
    uint16_t head;
};

struct output {
    struct buffer dumped;
    struct buffer polled;
};

bool buffer_push(struct buffer *output, uint8_t value);

void state_reset(struct state *state);

void output_reset(struct output *output);

bool separate_data_frame(
    const uint8_t *input_buffer,
    const uint16_t input_size,
    const uint16_t expected_size,
    struct state *state,
    struct output *output);

uint16_t separate_data_frame_old(
    const uint8_t *input_buffer,
    const uint16_t input_size,
    uint8_t *remaining_buffer,
    uint16_t *remaining_size,
    uint8_t *output_buffer,
    bool *start_found,
    uint32_t *total_size,
    uint32_t expected_size);

#ifdef __cplusplus
}
#endif
#endif /* __SUT_H_ */
