#include "rtga/rtga.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tga_init_blank(TgaImage *tga, TgaHeader header) {
    assert(tga);

    // Allocate image id if it exists
    if (header.id_length > 0) {
        tga->image_id = malloc(header.id_length);
    }

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

void tga_free(TgaImage *tga) {
    assert(tga);
    free(tga->image_id);
    free(tga->color_map_data);
    free(tga->image_data);
}

void tga_read_file(TgaImage *tga, const char *filename) {
    // Open file
    FILE *fp = fopen(filename, "rb");
    assert(fp);

    // Read header from file
    uint8_t header_bytes[18];
    fread(&header_bytes[0], 1, 18, fp);

    // Convert header into struct instance
    TgaHeader header;
    memcpy(&header.id_length, &header_bytes[0], 1);
    memcpy(&header.color_map_type, &header_bytes[1], 1);
    memcpy(&header.image_type, &header_bytes[2], 1);
    memcpy(&header.color_map_spec.first_index, &header_bytes[3], 2);
    memcpy(&header.color_map_spec.length, &header_bytes[5], 2);
    memcpy(&header.color_map_spec.pixel_depth, &header_bytes[7], 1);
    memcpy(&header.image_spec.x_origin, &header_bytes[8], 2);
    memcpy(&header.image_spec.y_origin, &header_bytes[10], 2);
    memcpy(&header.image_spec.width, &header_bytes[12], 2);
    memcpy(&header.image_spec.height, &header_bytes[14], 2);
    memcpy(&header.image_spec.pixel_depth, &header_bytes[16], 1);
    memcpy(&header.image_spec.descriptor, &header_bytes[17], 1);

    // Allocate TGA image
    tga_init_blank(tga, header);

    // Read image id from file if it exists
    if (tga->header.id_length > 0) {
        fread(&tga->image_id, 1, tga->header.id_length, fp);
    }

    // Write color map to file if it exists
    if (tga->header.color_map_spec.length > 0) {
        fread(tga->color_map_data, 1, tga->header.color_map_spec.length, fp);
    }

    // Write image data to file
    uint16_t pixel_bytes = tga->header.image_spec.pixel_depth / 8;
    if (tga->header.image_spec.pixel_depth % 8 > 0) {
        ++pixel_bytes;
    }
    size_t image_size = tga->header.image_spec.width * tga->header.image_spec.height * pixel_bytes;
    fread(tga->image_data, 1, image_size, fp);

    // Close file
    fclose(fp);
}

void tga_write_file(TgaImage *tga, const char *filename) {
    // Open file
    FILE *fp = fopen(filename, "wb");
    assert(fp);

    // Convert header into byte array
    // (Hopefully I can find a better method than this monstrosity...)
    TgaHeader header = tga->header;
    uint8_t header_bytes[18];
    memcpy(&header_bytes[0], &header.id_length, 1);
    memcpy(&header_bytes[1], &header.color_map_type, 1);
    memcpy(&header_bytes[2], &header.image_type, 1);
    memcpy(&header_bytes[3], &header.color_map_spec.first_index, 2);
    memcpy(&header_bytes[5], &header.color_map_spec.length, 2);
    memcpy(&header_bytes[7], &header.color_map_spec.pixel_depth, 1);
    memcpy(&header_bytes[8], &header.image_spec.x_origin, 2);
    memcpy(&header_bytes[10], &header.image_spec.y_origin, 2);
    memcpy(&header_bytes[12], &header.image_spec.width, 2);
    memcpy(&header_bytes[14], &header.image_spec.height, 2);
    memcpy(&header_bytes[16], &header.image_spec.pixel_depth, 1);
    memcpy(&header_bytes[17], &header.image_spec.descriptor, 1);

    // Write header to file
    fwrite(header_bytes, 18, 1, fp);
    
    // Write image id to file if it exists
    if (tga->header.id_length > 0) {
        fwrite(tga->image_id, 1, tga->header.id_length, fp);
    }

    // Write color map to file if it exists
    if (tga->header.color_map_spec.length > 0) {
        fwrite(tga->color_map_data, 1, tga->header.color_map_spec.length, fp);
    }

    // Write image data to file
    uint16_t pixel_bytes = tga->header.image_spec.pixel_depth / 8;
    if (tga->header.image_spec.pixel_depth % 8 > 0) {
        ++pixel_bytes;
    }
    size_t image_size = tga->header.image_spec.width * tga->header.image_spec.height * pixel_bytes;
    fwrite(tga->image_data, 1, image_size, fp);

    // Close file
    fclose(fp);
}
