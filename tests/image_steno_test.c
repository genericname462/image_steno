#include "image_steno.h"

#define STUFF_UTILITIES_DUMPBIN_IMPLEMENTATION
#define STUFF_UTILITIES_DUMPHEX_IMPLEMENTATION
#include "utilities.h"

#include <stdlib.h>
#include <immintrin.h>
#include <inttypes.h>
#include <memory.h>

int consistency_test() {
    size_t image_size = 1024 * 1024 * 4; // 4 MiB
    size_t data_size = 32 << 10; // 32 KiB
    uint8_t *image = calloc(1, image_size);
    uint8_t *output_data = calloc(1, data_size);
    union {
        uint8_t d[data_size];
        uint16_t v[data_size / 2];
    } data;
    int result = 0;

    for (unsigned int i = 0; i < data_size / 2; ++i) {
        if (_rdrand16_step(&(data.v[i])) != 1) {
            printf("Error gathering random bytes\n");
            result = -1;
            goto fail;
        }
    }

    if (embed_data(image, image_size, (uint8_t *) &data, data_size) != 0) {
        result = -2;
        goto fail;
    }

    if (extract_data(image, image_size, output_data, data_size) != 0) {
        result = -3;
        goto fail;
    }

    for (unsigned int j = 0; j < data_size; ++j) {
        if (memcmp(&data.d[j], &output_data[j], 1) != 0) {
            printf("Fail at Byte %i of %"PRIu64". Expected: %2X, got: %2X\n", j, data_size, data.d[j], output_data[j]);
            result = 1;
            goto fail;
        }
    }

    fail:
    free(image);
    free(output_data);
    return result;
}


int get_bit_test() {
    // [byte, bit_position, expected_result]
    uint8_t test_bytes_get_bit[][3] = {
            {255, 0, 1},
            {255, 7, 1},
            {0,   0, 0},
            {0,   7, 0},
            {1,   0, 0},
            {127, 7, 1},
            {127, 0, 0},

            {128, 0, 1},
            {64,  1, 1},
            {32,  2, 1},
            {16,  3, 1},
            {8,   4, 1},
            {4,   5, 1},
            {2,   6, 1},
            {1,   7, 1},
    };
    for (unsigned int i = 0; i < sizeof(test_bytes_get_bit) / sizeof(test_bytes_get_bit[0]); ++i) {
        int bit = get_bit(&test_bytes_get_bit[i][0], test_bytes_get_bit[i][1]);
        if (bit != test_bytes_get_bit[i][2]) {
            printf("%i[%i] is %i, expected %i\n", test_bytes_get_bit[i][0], test_bytes_get_bit[i][1], bit,
                   test_bytes_get_bit[i][2]);
            dumpBin(&test_bytes_get_bit[i][0], 1);
            dumpHex(&test_bytes_get_bit[i][0], 1);
            return 1;
        }
    }
    return 0;
}

int alt_get_bit_test() {
    // [byte, bit_position, expected_result]
    uint8_t test_bytes_get_bit[][3] = {
            {255, 0, 1},
            {255, 7, 1},
            {0,   0, 0},
            {0,   7, 0},
            {1,   0, 1},
            {1,   7, 0},
            {127, 7, 0},
            {127, 0, 1},

            {128, 7, 1},
            {64,  6, 1},
            {32,  5, 1},
            {16,  4, 1},
            {8,   3, 1},
            {4,   2, 1},
            {2,   1, 1},
            {1,   0, 1},
    };
    for (unsigned int i = 0; i < sizeof(test_bytes_get_bit) / sizeof(test_bytes_get_bit[0]); ++i) {
        int bit = alt_get_bit(&test_bytes_get_bit[i][0], test_bytes_get_bit[i][1]);
        if (bit != test_bytes_get_bit[i][2]) {
            printf("%i[%i] is %i, expected %i\n", test_bytes_get_bit[i][0], test_bytes_get_bit[i][1], bit,
                   test_bytes_get_bit[i][2]);
            dumpBin(&test_bytes_get_bit[i][0], 1);
            dumpHex(&test_bytes_get_bit[i][0], 1);
            return 1;
        }
    }
    return 0;
}

int set_bit_test() {
    // [byte, bit_position, bit_value, expected_result]
    uint8_t test_bytes_set_bit[][4] = {
            {255, 0, 0, 127},
            {255, 0, 1, 255},
            {255, 7, 0, 254},
            {255, 7, 1, 255},
            {0,   0, 0, 0},
            {0,   7, 1, 1},
            {0,   7, 0, 0},

            {0,   0, 1, 128},
            {0,   1, 1, 64},
            {0,   2, 1, 32},
            {0,   3, 1, 16},
            {0,   4, 1, 8},
            {0,   5, 1, 4},
            {0,   6, 1, 2},
            {0,   7, 1, 1},
    };
    for (unsigned int i = 0; i < sizeof(test_bytes_set_bit) / sizeof(test_bytes_set_bit[0]); ++i) {
        set_bit(&test_bytes_set_bit[i][0], test_bytes_set_bit[i][1], test_bytes_set_bit[i][2]);
        if (test_bytes_set_bit[i][0] != test_bytes_set_bit[i][3]) {
            printf("Testcase %i: Got %i after setting bit %i at %i, but expected %i\n", i + 1, test_bytes_set_bit[i][0],
                   test_bytes_set_bit[i][2], test_bytes_set_bit[i][1], test_bytes_set_bit[i][3]);
            dumpBin(&test_bytes_set_bit[i][0], 1);
            dumpHex(&test_bytes_set_bit[i][0], 1);
            return 1;
        }
    }
    return 0;
}

int alt_set_bit_test() {
    // [byte, bit_position, bit_value, expected_result]
    uint8_t test_bytes_set_bit[][4] = {
            {255, 0, 0, 254},
            {255, 0, 1, 255},
            {255, 7, 0, 127},
            {255, 7, 1, 255},
            {0,   0, 0, 0},
            {0,   7, 0, 0},

            {0,   7, 1, 128},
            {0,   6, 1, 64},
            {0,   5, 1, 32},
            {0,   4, 1, 16},
            {0,   3, 1, 8},
            {0,   2, 1, 4},
            {0,   1, 1, 2},
            {0,   0, 1, 1},
    };
    for (unsigned int i = 0; i < sizeof(test_bytes_set_bit) / sizeof(test_bytes_set_bit[0]); ++i) {
        alt_set_bit(&test_bytes_set_bit[i][0], test_bytes_set_bit[i][1], test_bytes_set_bit[i][2]);
        if (test_bytes_set_bit[i][0] != test_bytes_set_bit[i][3]) {
            printf("Testcase %i: Got %i after setting bit %i at %i, but expected %i\n", i + 1, test_bytes_set_bit[i][0],
                   test_bytes_set_bit[i][2], test_bytes_set_bit[i][1], test_bytes_set_bit[i][3]);
            dumpBin(&test_bytes_set_bit[i][0], 1);
            dumpHex(&test_bytes_set_bit[i][0], 1);
            return 1;
        }
    }
    return 0;
}

int main() {
    printf("Testsuite for image_steno:\n");

    if (consistency_test() != 0) {
        printf("consistency_test() failed!\n");
        return 1;
    } else {
        printf("consistency_test() passed!\n");
    }

    if (get_bit_test() != 0) {
        printf("get_bit_test() failed!\n");
        return 1;
    } else {
        printf("get_bit_test() passed!\n");
    }

    if (alt_get_bit_test() != 0) {
        printf("alt_get_bit_test() failed!\n");
        return 1;
    } else {
        printf("alt_get_bit_test() passed!\n");
    }

    if (set_bit_test() != 0) {
        printf("set_bit_test() failed!\n");
        return 1;
    } else {
        printf("set_bit_test() passed!\n");
    }

    if (alt_set_bit_test() != 0) {
        printf("alt_set_bit_test() failed!\n");
        return 1;
    } else {
        printf("alt_set_bit_test() passed!\n");
    }

    printf("\nAll tests passed!\n");
    return 0;
}