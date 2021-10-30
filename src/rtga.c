#include "rtga/rtga.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// 8-bit Grayscale Values (G)
const TgaColor WHITE8 = COLOR8(255);
const TgaColor LIGHT_GRAY8 = COLOR8(196);
const TgaColor GRAY8 = COLOR8(128);
const TgaColor DARK_GRAY8 = COLOR8(64);
const TgaColor BLACK8 = COLOR8(0);

// 16-bit Color Values (RGB)
const TgaColor RED16 = COLOR16(31, 0, 0);
const TgaColor GREEN16 = COLOR16(0, 31, 0);
const TgaColor BLUE16 = COLOR16(0, 0, 31);
const TgaColor YELLOW16 = COLOR16(31, 31, 0);
const TgaColor MAGENTA16 = COLOR16(31, 0, 31);
const TgaColor CYAN16 = COLOR16(0, 31, 31);
const TgaColor PURPLE16 = COLOR16(16, 0, 31);
const TgaColor ROSE16 = COLOR16(31, 0, 16);
const TgaColor ORANGE16 = COLOR16(31, 16, 0);
const TgaColor LIME16 = COLOR16(16, 31, 0);
const TgaColor MINT16 = COLOR16(0, 31, 16);
const TgaColor AZURE16 = COLOR16(0, 16, 31);
const TgaColor WHITE16 = COLOR16(31, 31, 31);
const TgaColor BLACK16 = COLOR16(0, 0, 0);

// 24-bit Color Values (RGB)
const TgaColor BLUE24 = COLOR24(0, 0, 255);
const TgaColor GREEN24 = COLOR24(0, 255, 0);
const TgaColor RED24 = COLOR24(255, 0, 0);
const TgaColor YELLOW24 = COLOR24(255, 255, 0);
const TgaColor MAGENTA24 = COLOR24(255, 0, 255);
const TgaColor CYAN24 = COLOR24(0, 255, 255);
const TgaColor PURPLE24 = COLOR24(128, 0, 255);
const TgaColor ROSE24 = COLOR24(255, 0, 128);
const TgaColor ORANGE24 = COLOR24(255, 128, 0);
const TgaColor LIME24 = COLOR24(128, 255, 0);
const TgaColor MINT24 = COLOR24(0, 255, 128);
const TgaColor AZURE24 = COLOR24(0, 128, 255);
const TgaColor WHITE24 = COLOR24(255, 255, 255);
const TgaColor BLACK24 = COLOR24(0, 0, 0);

int tga_alloc(TgaImageType image_type, uint16_t width, uint16_t height, uint8_t pixel_depth, TgaImage *tga) {
    assert(tga);
    assert(tga_valid_depth(pixel_depth));

    // Return error codes if arguments are invalid
    if (!tga) return TGA_NULL_PTR_ERROR;
    if (!tga_valid_depth(pixel_depth)) return TGA_INVALID_PIXEL_DEPTH_ERROR;

    // Update header and state
    memset(&tga->header, 0, sizeof(TgaHeader));
    tga->header.image_type = image_type;
    tga->header.width = width;
    tga->header.height = height;
    tga->header.image_pixel_depth = pixel_depth;
    tga->state = IS_UNCOMPRESSED;

    // Allocate image data
    tga->image_data = malloc(tga_image_size(&tga->header));
    if (!tga->image_data) return TGA_ALLOCATION_ERROR;

    return TGA_SUCCESS;
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
    uint8_t header_bytes[TGA_HEADER_SIZE];
    FILE *fp;
    size_t bytes_read;

    assert(tga);

    // Open file
    fp = fopen(filename, "rb");
    if (!fp) return TGA_FILE_OPEN_ERROR;

    // Read header from file
    bytes_read = fread(&header_bytes[0], 1, TGA_HEADER_SIZE, fp);
    if (bytes_read != TGA_HEADER_SIZE) return TGA_FILE_READ_ERROR;

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
    if (tga_alloc(header.image_type, header.width, header.height, header.image_pixel_depth, tga) != TGA_SUCCESS) return TGA_ALLOCATION_ERROR;

    // Read image id from file if it exists
    if (tga->header.id_length > 0) {
        bytes_read = fread(&tga->image_id, 1, tga->header.id_length, fp);
        if (bytes_read != tga->header.id_length) return TGA_FILE_READ_ERROR;
    }

    // Write color map to file if it exists
    if (header.color_map_type && tga->header.color_map_length > 0) {
        bytes_read = fread(tga->color_map_data, 1, tga->header.color_map_length, fp);
        if (bytes_read != tga->header.color_map_length) return TGA_FILE_READ_ERROR;
    }

    // Write image data to file
    bytes_read = fread(tga->image_data, 1, tga_image_size(&tga->header), fp);
    if (bytes_read != tga_image_size(&tga->header)) return TGA_FILE_READ_ERROR;

    // Close file
    fclose(fp);

    return TGA_SUCCESS;
}

int tga_write_file(TgaImage *tga, const char *filename) {
    uint8_t header_bytes[TGA_HEADER_SIZE];
    FILE *fp;
    size_t bytes_written;

    assert(tga);

    // Open file
    fp = fopen(filename, "wb");
    if (!fp) return TGA_FILE_OPEN_ERROR;

    // Convert header into byte array
    // (Hopefully I can find a better method than this monstrosity...)
    TgaHeader header = tga->header;
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
    bytes_written = fwrite(header_bytes, 1, TGA_HEADER_SIZE, fp);
    if (bytes_written != TGA_HEADER_SIZE) return TGA_FILE_WRITE_ERROR;
    
    // Write image id to file if it exists
    if (tga->header.id_length > 0) {
        bytes_written = fwrite(tga->image_id, 1, tga->header.id_length, fp);
        if (bytes_written != tga->header.id_length) return TGA_FILE_WRITE_ERROR;
    }

    // Write color map to file if it exists
    if (header.color_map_type && tga->header.color_map_length > 0) {
        bytes_written = fwrite(tga->color_map_data, 1, tga->header.color_map_length, fp);
        if (bytes_written != tga->header.color_map_length) return TGA_FILE_WRITE_ERROR;
    }

    // Write image data to file
    bytes_written = fwrite(tga->image_data, 1, tga_image_size(&tga->header), fp);
    if (bytes_written != tga_image_size(&tga->header)) return TGA_FILE_WRITE_ERROR;

    // Close file
    fclose(fp);

    return TGA_SUCCESS;
}

void tga_set_pixel(TgaImage *tga, uint16_t x, uint16_t y, TgaColor color) {
    assert(tga);

    uint8_t pixel_size = tga_pixel_size(&tga->header);
    size_t index = (y * tga->header.width + x) * pixel_size;
 
    assert(pixel_size == color.bit_size / 8);
    memcpy(tga->image_data + index, color.bgra, pixel_size);
}

void tga_fill(TgaImage *tga, TgaColor color) {
    assert(tga);

    for (uint16_t y = 0; y < tga->header.height; ++y) {
        for (uint16_t x = 0; x < tga->header.width; ++x) {
            tga_set_pixel(tga, x, y, color);
        }
    }
}

int tga_to_color_map(TgaImage *tga) {
    return 1;
}

int tga_from_color_map(TgaImage *tga) {
    return 1;
}

uint8_t tga_pixel_size(const TgaHeader *header) {
    assert(header);
    assert(tga_valid_depth(header->image_pixel_depth));
    
    uint8_t pixel_size = header->image_pixel_depth / 8;

    if (header->image_pixel_depth % 8 != 0) {
        ++pixel_size;
    }

    return pixel_size;
}

size_t tga_image_size(const TgaHeader *header) {
    assert(header);

    return header->width * header->height * tga_pixel_size(header);
}

bool tga_valid_depth(uint8_t pixel_depth) {
    return (pixel_depth % 8 == 0 && pixel_depth <= 32 || pixel_depth == 15);
}
