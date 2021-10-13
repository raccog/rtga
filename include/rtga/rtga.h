#ifndef RTGA_H
#define RTGA_H

#include <stdbool.h>
#include <stdint.h>

// Image data representations 
typedef enum {
    NO_IMAGE = 0,
    UNCOMPRESSED_COLOR_MAPPED_IMAGE = 1,
    UNCOMPRESSED_TRUE_COLOR_IMAGE = 2,
    RUN_LENGTH_ENCODED_COLOR_MAPPED_IMAGE = 9,
    RUN_LENGTH_ENCODED_BLACK_AND_WHITE_IMAGE = 11,
} TgaImageType;

// Color map specification
typedef struct {
    uint16_t first_index;
    uint16_t length;
    uint8_t pixel_depth;
} TgaColorMapSpec;

// Image specification
typedef struct {
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t pixel_depth;
    uint8_t descriptor;
} TgaImageSpec;

// TGA image header
typedef struct {
    uint8_t id_length;
    bool color_map_type;
    TgaImageType image_type;
    TgaColorMapSpec color_map_spec;
    TgaImageSpec image_spec;
} TgaHeader;

// TGA image
typedef struct {
    TgaHeader header;
    uint8_t *image_id;
    uint8_t *color_map_data;
    uint8_t *image_data;
} TgaImage;

// Allocates memory for a blank TGA image in memory using the 
// specifications from header.
void tga_init_blank(TgaImage *tga, TgaHeader header);

// Frees allocated memory for a TGA image.
void tga_free(TgaImage *tga);

// Reads s TGA image from a file
void tga_read_file(TgaImage *tga, const char *filename);

// Writes a TGA image into a file
void tga_write_file(TgaImage *tga, const char *filename);

#endif
