#include "rtga/rtga.h"

#include <stdlib.h>

void tga_init_blank(tga_t *tga, tga_header_t header) {
    // Allocate image id
    tga->image_id = malloc(header.id_length);

    // Allocate color map data if it exists
    if (header.color_map_spec.length > 0) {
        tga->color_map_data = malloc(header.color_map_spec.length);
    }
    
    // Allocate image data
    uint16_t pixel_bytes = header.image_spec.pixel_depth / 8;
    if (header.image_spec.pixel_depth % 8 > 0) {
        ++pixel_bytes;
    }
    tga->image_data = malloc(header.image_spec.width * header.image_spec.height * pixel_bytes);

    // Set header
    tga->header = header;
}

void tga_free(tga_t *tga) {
    free(tga->image_id);
    free(tga->color_map_data);
    free(tga->image_data);
}
