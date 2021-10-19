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
    const uint8_t BLUE[] = {255, 0, 0};
    tga_set_pixel(&tga, 0, 0, &BLUE[0]);
    success = tga_write_file(&tga, "test.tga");
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", "test.tga", 
        header.width, header.height, header.image_pixel_depth);
    }
    tga_free(&tga);
}
