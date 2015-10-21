#include "image_steno.h"

inline int get_bit(const void *data, const size_t position) {
    size_t byteoffset = position / 8; // Full bytes
    size_t bitoffset = 7 - position % 8; // Remaining bits
    return (((uint8_t *) data)[byteoffset] >> bitoffset) & 0x01;
}

inline int alt_get_bit(const void *data, const size_t position) {
    return (((uint8_t *) data)[position / 8] >> (position % 8)) & 0x01;
}

inline void set_bit(uint8_t *data, const size_t position, const int bit) {
    size_t byteoffset = position / 8; // Full bytes
    size_t bitoffset = 7 - position % 8; // Remaining bits
    if (bit) { // Set bit to 1
        data[byteoffset] |= 1 << bitoffset;
    } else { // Clear bit to 0
        data[byteoffset] &= ~(1 << bitoffset);
    }
    return;
}

inline void alt_set_bit(uint8_t *data, const size_t position, const int bit) {
    if (bit) { // Set bit to 1
        data[position / 8] |= 1 << position % 8;
    } else { // Clear bit to 0
        data[position / 8] &= ~(1 << position % 8);
    }
}

int embed_data(uint8_t *image, const size_t len_image, const uint8_t *secret, const size_t len_secret) {
    // Check if secret can even fit into the image
    if (len_secret * 8 * 4 > len_image * 3) {
        return -1;
    }

    size_t bit_position = 0;
    size_t byte_position_image = 0;

    while (bit_position < len_secret * 8) {
        // Skip (and keep) alpha byte, since that causes render bugs and unreliable results in browsers
        if (byte_position_image % 4 == 3) {
            ++byte_position_image;
        }
        set_bit(image + byte_position_image, 7, get_bit(secret, bit_position));
        ++bit_position;
        ++byte_position_image;
    }
    return 0;
}

int extract_data(const uint8_t *image, const size_t image_len, uint8_t *extract, const size_t max_len_extract) {
    size_t byte_position_image = 0;
    size_t bit_position_extract = 0;

    while (byte_position_image < image_len && bit_position_extract < max_len_extract * 8) {
        // Skip alpha byte
        if (byte_position_image % 4 == 3) {
            ++byte_position_image;
        }
        set_bit(extract, bit_position_extract, image[byte_position_image] % 2);
        ++bit_position_extract;
        ++byte_position_image;
    }
    return 0;
}

