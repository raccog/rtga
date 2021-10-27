#include <stdio.h>

#include "rtga/rtga.h"
#include "rtga/rtga_version.h"

/*
 *  RTGA Test
 *
 *  Initializes a black TGA image into memory, then writes that
 *  image into a file called 'test.tga', and then prints out the header.
 *
 *  When opened by an image viewing software, the TGA image should have
 *  a size of 10x10 pixels, and an 8 bit pixel depth.
 *
 *  This file is then read into memory and the header is printed out
 *  again.
 *
 *  Through this test, the following functions are tested:
 *      tga_init_blank()
 *      tga_free()
 *      tga_write_file()
 *      tga_read_file()
 *
 */
int main(void) {
    TgaImage tga;
    int success;
    uint16_t width, height;
    uint8_t pixel_depth;

    // File names
    const char *UC_24B_FILENAME = "color24.tga";
    const char *UC_8B_FILENAME = "color8.tga";
    const char *UC_24B_GRADIENT_FILENAME = "color24_gradient.tga";
    const char *UC_8B_GRADIENT_FILENAME = "color8_gradient.tga";

    // Print package version
    printf("rtga version: %s\n", RTGA_VERSION);

    // Allocate blank image, print out the header, and then write to file
    width = 10;
    height = 10;
    pixel_depth = 24;
    if (tga_alloc(UNCOMPRESSED_TRUE_COLOR_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    tga_fill(&tga, BLACK24);
    tga_set_pixel(&tga, 0, 0, BLUE24);
    tga_set_pixel(&tga, 1, 1, GREEN24);
    success = tga_write_file(&tga, UC_24B_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_24B_FILENAME, width, height, pixel_depth);
    }
    tga_free(&tga);

    // Write a grayscale image
    pixel_depth = 8;
    if (tga_alloc(UNCOMPRESSED_BLACK_AND_WHITE_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    tga_fill(&tga, BLACK8);
    tga_set_pixel(&tga, 0, 0, GRAY8);
    tga_set_pixel(&tga, 1, 1, LIGHT_GRAY8);
    success = tga_write_file(&tga, UC_8B_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_8B_FILENAME, width, height, pixel_depth);
    }
    tga_free(&tga);

    // Write a grayscale image gradient
    width = 255;
    height = 255;
    if (tga_alloc(UNCOMPRESSED_BLACK_AND_WHITE_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    for (int y = 0; y < height; ++y) {
        TgaColor gray_color = {{y, 0, 0, 0}, 8};
        for (int x = 0; x < width; ++x) {
            tga_set_pixel(&tga, x, y, gray_color);
        }
    }
    success = tga_write_file(&tga, UC_8B_GRADIENT_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_8B_GRADIENT_FILENAME, width, height, pixel_depth);
    }
    tga_free(&tga);

}
