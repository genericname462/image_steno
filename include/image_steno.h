#ifndef STUFF_IMAGE_STENO_H
#define STUFF_IMAGE_STENO_H

#include <stdint.h>

typedef struct _pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} pixel;

/**
 * Fetches the bit at position in data. Handling of Endianess is left to the user.
 * Assumes "streaming", strict left-to-right, notation:
 *
 * Index:    | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12| 13| 14| 15|
 * Data:     | 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 * as uint8: |              127              |               128             |
 *
 * See alt_get_bit().
 */
int get_bit(const void *data, const size_t position);

/**
 * Fetches the bit at position in data. Handling of Endianess is left to the user.
 * Assumes "logical", byte wise, notation:
 *
 * Index:    | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | 15| 14| 13| 11| 12| 10| 9 | 8 |
 * Data:     | 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 * as uint8: |              127              |               128             |
 *
 * See get_bit().
 */
int alt_get_bit(const void *data, size_t position);

void set_bit(uint8_t *data, size_t position, int bit);

int embed_data(uint8_t *image, const uint32_t x, const uint32_t y, const uint32_t channels, const uint8_t *secret,
               const size_t len_secret);

int extract_data(const uint8_t *image, const uint32_t x, const uint32_t y, const uint32_t channels, uint8_t *extract,
                 const size_t max_len_extract);

#endif //STUFF_IMAGE_STENO_H