/**
 * @file sut.c
 * @brief System under test
 */

#include "sut.h"
#include <string.h>

bool buffer_push(struct buffer *output, uint8_t value) {
    if (output->head > BUFFER_ROWS) {
        return false;
    }
    output->buffer[output->head] = value;
    ++output->head;
    return true;
}

void state_reset(struct state *state) {
    state->start_found = false;
    state->total_size = 0;
}

void output_reset(struct output *output) {
    memset(output->dumped.buffer, 0U, BUFFER_ROWS);
    memset(output->polled.buffer, 0U, BUFFER_ROWS);
    output->dumped.head = 0U;
    output->polled.head = 0U;
}

bool separate_data_frame(
    const uint8_t *input_buffer,
    const uint16_t input_size,
    const uint16_t expected_size,
    struct state *state,
    struct output *output)
{
    bool error = false;
    output_reset(output);
    for (uint32_t i = 0; i < input_size; ++i) {
        if (state->start_found) {
            error = buffer_push(&output->dumped, input_buffer[i]);
            ++state->total_size;

            /* If the expected amount of dumped data has been parsed, reset the state */
            if (state->total_size >= expected_size) {
                state_reset(state);
            }
        } else if (input_buffer[i] == '$') {
            error = buffer_push(&output->dumped, input_buffer[i]);
            ++state->total_size;
            state->start_found = true;
        } else {
            error = buffer_push(&output->polled, input_buffer[i]);
        }

        if (error) {
            state_reset(state);
            break;
        }
    }
    return error;
}


uint16_t separate_data_frame_old(
    const uint8_t *input_buffer,
    const uint16_t input_size,
    uint8_t *remaining_buffer,
    uint16_t *remaining_size,
    uint8_t *output_buffer,
    bool *start_found,
    uint32_t *total_size,
    uint32_t expected_size)
{
    bool end_found = false;
    uint16_t output_len = 0;
    uint16_t start_idx = 0;
    uint16_t packet_len = input_size;

    /* Search buffer for start/end of dumped data */
    for (uint32_t i = 0; i < input_size; ++i) {
        /* Look for start of dumped data */
        if (input_buffer[i] == '$') {
            start_idx = i;
            *total_size = 0;
            *start_found = true;
        }

        /* Wait for total dumped data to reach expected size */
        if ((*start_found) && (((*total_size) + (i - start_idx) + 1) >= expected_size)) {
            packet_len = i + 1;
            *start_found = false;
            end_found = true;
        }
    }

    if ((*start_found) || end_found) {
        /* calculate data length */
        output_len = packet_len - start_idx;
        *total_size += output_len;

        // start_idx is the index of the '$', or 0

        /* Protect memcpy operation from buffer overflow */
        if ((start_idx + output_len) <= input_size) {
            memcpy(output_buffer, &input_buffer[start_idx], output_len);
        } else {
            /* Reset state in event of buffer overflow */
            *total_size = 0;
            output_len = 0;
            *start_found = false;
            end_found = false;
        }

        if (end_found) {
            /* Copy any non-dumped data from input_buffer into remaining_buffer */
            memcpy(remaining_buffer, &input_buffer[0], start_idx);
            memcpy(
                &remaining_buffer[start_idx],
                &input_buffer[start_idx + output_len],
                input_size - output_len);
            *remaining_size = input_size - output_len;

        } else if (0U != start_idx) {
            /* Dumped record has been extracted from input buffer to output buffer, but there is
             * still data in the input buffer that needs to be parsed */
            /* Copy 0 -> start part of input buffer into remaining buffer */
            memcpy(remaining_buffer, &input_buffer[0], start_idx);
            *remaining_size = start_idx;
        } else {
            /* Set remaining_size to 0 to signify that there is nothing left to parse in the input
             * buffer */
            *remaining_size = 0;
        }
    }

    return output_len;
}



