#include <stdio.h>

#include "rtga/rtga.h"

/*
 *  RTGA Test
 *
 *  Initializes a blank TGA image into memory, and then writes that
 *  image into a file called 'test.tga'.
 *
 *  When opened by an image viewing software, the TGA image should have
 *  a size of 40x40 pixels, an 8 bit pixel depth, and an alpha layer.
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
    TgaColorMapSpec *color_map_spec = &header.color_map_spec;
    color_map_spec->first_index = 0;
    color_map_spec->length = 0;
    color_map_spec->pixel_depth = 0;

    // Set image specifications
    TgaImageSpec *image_spec = &header.image_spec;
    image_spec->x_origin = 0;
    image_spec->y_origin = 0;
    image_spec->width = 40;
    image_spec->height = 40;
    image_spec->pixel_depth = 32;
    image_spec->descriptor = 0;

    tga_init_blank(&tga, header);

    tga_write_file(&tga, "test.tga");

    tga_free(&tga);
}
