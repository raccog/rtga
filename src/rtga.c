#include "rtga/rtga.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int tga_alloc(TgaImage *tga, TgaHeader header) {
    assert(tga);

    // Allocate image id if it exists
    if (header.id_length > 0) {
        tga->image_id = malloc(header.id_length);
    }

    // Allocate color map data if it exists
    if (header.color_map_length > 0) {
        tga->color_map_data = malloc(header.color_map_length);
    }
    
    // Allocate image data
    tga->image_data = malloc(tga_image_size(&tga->header));

    // Set header
    tga->header = header;

    return 0;
}

void tga_free(TgaImage *tga) {
    assert(tga);

    // Free all allocated memory
    free(tga->image_id);
    free(tga->color_map_data);
    free(tga->image_data);

    // Assign NULL to all pointers
    tga->image_id = NULL;
    tga->color_map_data = NULL;
    tga->image_data = NULL;
}

int tga_read_file(TgaImage *tga, const char *filename) {
    assert(tga);

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
    memcpy(&header.color_map_first_index, &header_bytes[3], 2);
    memcpy(&header.color_map_length, &header_bytes[5], 2);
    memcpy(&header.color_map_pixel_depth, &header_bytes[7], 1);
    memcpy(&header.x_origin, &header_bytes[8], 2);
    memcpy(&header.y_origin, &header_bytes[10], 2);
    memcpy(&header.width, &header_bytes[12], 2);
    memcpy(&header.height, &header_bytes[14], 2);
    memcpy(&header.image_pixel_depth, &header_bytes[16], 1);
    memcpy(&header.descriptor, &header_bytes[17], 1);

    // Allocate TGA image
    tga_alloc(tga, header);

    // Read image id from file if it exists
    if (tga->header.id_length > 0) {
        fread(&tga->image_id, 1, tga->header.id_length, fp);
    }

    // Write color map to file if it exists
    if (tga->header.color_map_length > 0) {
        fread(tga->color_map_data, 1, tga->header.color_map_length, fp);
    }

    // Write image data to file
    fread(tga->image_data, 1, tga_image_size(&tga->header), fp);

    // Close file
    fclose(fp);

    return 0;
}

int tga_write_file(TgaImage *tga, const char *filename) {
    assert(tga);

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
    memcpy(&header_bytes[3], &header.color_map_first_index, 2);
    memcpy(&header_bytes[5], &header.color_map_length, 2);
    memcpy(&header_bytes[7], &header.color_map_pixel_depth, 1);
    memcpy(&header_bytes[8], &header.x_origin, 2);
    memcpy(&header_bytes[10], &header.y_origin, 2);
    memcpy(&header_bytes[12], &header.width, 2);
    memcpy(&header_bytes[14], &header.height, 2);
    memcpy(&header_bytes[16], &header.image_pixel_depth, 1);
    memcpy(&header_bytes[17], &header.descriptor, 1);

    // Write header to file
    fwrite(header_bytes, 18, 1, fp);
    
    // Write image id to file if it exists
    if (tga->header.id_length > 0) {
        fwrite(tga->image_id, 1, tga->header.id_length, fp);
    }

    // Write color map to file if it exists
    if (tga->header.color_map_length > 0) {
        fwrite(tga->color_map_data, 1, tga->header.color_map_length, fp);
    }

    // Write image data to file
    fwrite(tga->image_data, 1, tga_image_size(&tga->header), fp);

    // Close file
    fclose(fp);

    return 0;
}

void tga_set_pixel(TgaImage *tga, uint16_t x, uint16_t y, const uint8_t *color) {
    assert(tga);

    const uint8_t pixel_size = tga_pixel_size(&tga->header);
    const size_t index = (y * tga->header.width + x) * pixel_size;
    memcpy(tga->image_data + index, color, pixel_size);
}

uint8_t tga_pixel_size(const TgaHeader *header) {
    assert(header);

    uint8_t pixel_size = header->image_pixel_depth / 8;
    if (header->image_pixel_depth % 8 > 0) {
        ++pixel_size;
    }
    return pixel_size;
}

size_t tga_image_size(const TgaHeader *header) {
    assert(header);

    return header->width * header->height * tga_pixel_size(header);
}
