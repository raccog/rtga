#include <stdio.h>

#include "rtga/rtga.h"

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
    TgaHeader header;
    int success;

    // File names
    const char *UC_24B_FILENAME = "color24.tga";
    const char *UC_8B_FILENAME = "color8.tga";
    const char *UC_24B_GRADIENT_FILENAME = "color24_gradient.tga";
    const char *UC_8B_GRADIENT_FILENAME = "color8_gradient.tga";
    
    // Temporary color
    const uint8_t _TRUE_BLACK[] = {0,0,0};
    const uint8_t *TRUE_BLACK = &_TRUE_BLACK[0];

    // Set initial header values
    header.id_length = 0;
    header.color_map_type = false;
    header.image_type = UNCOMPRESSED_TRUE_COLOR_IMAGE;

    // Set color map specifications
    header.color_map_first_index = 0;
    header.color_map_length = 0;
    header.color_map_pixel_depth = 0;

    // Set image specifications
    header.x_origin = 0;
    header.y_origin = 0;
    header.width = 10;
    header.height = 10;
    header.image_pixel_depth = 24;
    header.descriptor = 0;

    // Allocate blank image, print out the header, and then write to file
    if (tga_alloc(&tga, header) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    tga_fill(&tga, TRUE_BLACK);
    tga_set_pixel(&tga, 0, 0, BLUE);
    tga_set_pixel(&tga, 1, 1, GREEN);
    success = tga_write_file(&tga, UC_24B_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_24B_FILENAME, 
        header.width, header.height, header.image_pixel_depth);
    }
    tga_free(&tga);

    // Write a grayscale image
    header.image_type = UNCOMPRESSED_BLACK_AND_WHITE_IMAGE;
    header.image_pixel_depth = 8;
    if (tga_alloc(&tga, header) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    tga_fill(&tga, BLACK);
    tga_set_pixel(&tga, 0, 0, GRAY);
    tga_set_pixel(&tga, 1, 1, LIGHT_GRAY);
    success = tga_write_file(&tga, UC_8B_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_8B_FILENAME, 
        header.width, header.height, header.image_pixel_depth);
    }
    tga_free(&tga);

    // Write a grayscale image gradient
    header.width = 255;
    header.height = 255;
    if (tga_alloc(&tga, header) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    for (int y = 0; y < header.height; ++y) {
        const uint8_t gray_value[] = {y};
        for (int x = 0; x < header.width; ++x) {
            tga_set_pixel(&tga, x, y, gray_value);
        }
    }
    success = tga_write_file(&tga, UC_8B_GRADIENT_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_8B_GRADIENT_FILENAME, 
        header.width, header.height, header.image_pixel_depth);
    }
    tga_free(&tga);

}
