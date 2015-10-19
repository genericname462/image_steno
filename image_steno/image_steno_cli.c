#include "image_steno.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <sys/stat.h>
#include <inttypes.h>


int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "decode or encode?\n");
        return -1;
    }
    if(argc < 3) {
        fprintf(stderr, "Specify path to image!\n");
        return -1;
    }
    if(argc < 4) {
        fprintf(stderr, "Specify path to data!\n");
        return -1;
    }

    if (!strncmp(argv[1], "encode", 7)) {
        int32_t x, y, channels;
        uint8_t *image = stbi_load(argv[2], &x, &y, &channels, 4);
        if (image == NULL) {
            perror(stbi_failure_reason());
            return -1;
        } else {
            channels = 4; // contains the number of channels the image would have had if we had not forced it to 4.
        }

        struct stat64 s;
        if (stat64(argv[3], &s) != 0) {
            perror(NULL);
            return -1;
        }
        int64_t image_file_size = s.st_size;

        FILE *fp;
        if ((fp = fopen(argv[3], "rb")) == NULL) {
            perror(NULL);
            return -1;
        }
        uint8_t *buffer = malloc((size_t) image_file_size + 1);
        if (fread(buffer, 1, (size_t) image_file_size, fp) != image_file_size) {
            perror(NULL);
            return -1;
        }
        fclose(fp);
        printf("Read %"PRIi64" Bytes from %s\n", image_file_size, argv[3]);

        if (embed_data(image, x, y, channels, buffer, (size_t) image_file_size)) {
            fprintf(stderr, "Secret data too large!\n");
            return -1;
        }
        free(buffer);

        if (stbi_write_png("image_output.png", x, y, channels, image, 0) == 0) {
            fprintf(stderr, "Error saving image!");
            return -1;
        }
        free(image);
    } else if (!strncmp(argv[1], "decode", 7)) {
        int32_t x, y, channels;
        uint8_t *image = stbi_load(argv[2], &x, &y, &channels, 4);
        if (image == NULL) {
            perror(stbi_failure_reason());
            return -1;
        }

        size_t max_size_extract = (size_t) (x * y * 3) / 8;
        uint8_t *extracted_data = malloc(max_size_extract);
        extract_data(image, x, y, channels, extracted_data, max_size_extract);
        //dumpHex(extracted_data, 16);

        FILE *fp;
        if ((fp = fopen(argv[3], "wb")) == NULL) {
            fprintf(stderr, "Can't open %s!\n", argv[3]);
            return -1;
        }
        if (fwrite(extracted_data, 1, max_size_extract, fp) != max_size_extract) {
            perror(NULL);
        }
        fclose(fp);

        free(extracted_data);
        free(image);
    } else {
        fprintf(stderr, "decode or encode?");
        return -1;
    }
    return 0;
}