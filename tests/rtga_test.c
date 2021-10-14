#include <assert.h>
#include <stdio.h>

#include "rtga/rtga.h"

/*
 *  Print TGA image header
 *
 *  Prints out each field of a TGA image header.
 *
 */
void print_tga_image_header(TgaImage *tga) {
    assert(tga);
    TgaHeader *header = &tga->header;
    printf("ID Length: %u\nColor Map Exists: %u\nImage Type: %u\nColor Map First Index: %u\nColor Map Length: %u\nColor Map Pixel Depth: %u\nImage X Origin: %u\nImage Y Origin %u\nImage Width: %u\nImage Height: %u\nImage Pixel Depth: %u\nImage Descriptor: %u\n", header->id_length, header->color_map_type, header->image_type, header->color_map_first_index, header->color_map_length, header->color_map_pixel_depth, header->x_origin, header->y_origin, header->width, header->height, header->image_pixel_depth, header->descriptor);
}

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
    tga_alloc(&tga, header);
    printf("Image Before Read:\n");
    print_tga_image_header(&tga);
    printf("\n");
    const uint8_t BLUE[] = {255, 0, 0};
    tga_set_pixel(&tga, 0, 0, &BLUE[0]);
    tga_write_file(&tga, "test.tga");
    tga_free(&tga);

    // Read image from file and print out the header
    tga_read_file(&tga, "test.tga");
    printf("Image After Read:\n");
    print_tga_image_header(&tga);
    tga_free(&tga);
}
