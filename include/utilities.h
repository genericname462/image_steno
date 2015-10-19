#ifndef STUFF_UTILITIES_H
#define STUFF_UTILITIES_H

#include <stdio.h>

void dumpHex(const void *data, size_t size);
#ifdef STUFF_UTILITIES_DUMPHEX_IMPLEMENTATION
void dumpHex(const void *data, size_t size) {
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i) {
        printf("%02X ", ((unsigned char *) data)[i]);
        if (((unsigned char *) data)[i] >= ' ' && ((unsigned char *) data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char *) data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i + 1) % 8 == 0 || i + 1 == size) {
            printf(" ");
            if ((i + 1) % 16 == 0) {
                printf("| %s \n", ascii);
            } else if (i + 1 == size) {
                ascii[(i + 1) % 16] = '\0';
                if ((i + 1) % 16 <= 8) {
                    printf(" ");
                }
                for (j = (i + 1) % 16; j < 16; ++j) {
                    printf("   ");
                }
                printf("| %s \n", ascii);
            }
        }
    }
}
#endif // STUFF_UTILITIES_DUMPHEX_IMPLEMENTATION

void dumpBin(const void *data, size_t size);
#ifdef STUFF_UTILITIES_DUMPBIN_IMPLEMENTATION
void dumpBin(const void *data, size_t size) {
    char ascii[5] = "....";
    for (unsigned int byte = 0; byte < size; ++byte) {
        // Bits
        for (int bit = 7; bit >= 0; --bit) {
            printf("%i", ((unsigned char *) data)[byte] >> bit & 0x01);
        }
        // Ascii
        if (((unsigned char *) data)[byte] >= ' ' && ((unsigned char *) data)[byte] <= '~') {
            ascii[byte % 4] = ((unsigned char *) data)[byte];
        } else {
            ascii[byte % 4] = '.';
        }
        printf(" ");
        if ((byte + 1) % 4 == 0 || byte + 1 == size) {
            for (unsigned int i = 0; i < 3 - (byte % 4); ++i) {
                printf("         ");
            }
            printf(" | %s\n", ascii);
            ascii[0] = '.';
            ascii[1] = '.';
            ascii[2] = '.';
            ascii[3] = '.';
            ascii[4] = '\0';
        }
    }
}
#endif // STUFF_UTILITIES_DUMPBIN_IMPLEMENTATION

#endif //STUFF_UTILITIES_H