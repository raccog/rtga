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
} tga_image_t;

// Color map specification
typedef struct {
    uint16_t first_index;
    uint16_t length;
    uint8_t pixel_depth;
} tga_color_map_spec_t;

// Image specification
typedef struct {
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t pixel_depth;
    uint8_t descriptor;
} tga_image_spec_t;

// TGA image header
typedef struct {
    uint8_t id_length;
    bool color_map_type;
    tga_image_t image_type;
    tga_color_map_spec_t color_map_spec;
    tga_image_spec_t image_spec;
} tga_header_t;

// TGA image
typedef struct {
    tga_header_t header;
    uint8_t *image_id;
    uint8_t *color_map_data;
    uint8_t *image_data;
} tga_t;

// Allocates memory for a blank TGA image in memory using the 
// specifications from header.
void tga_init_blank(tga_t *tga, tga_header_t header);

// Frees allocated memory for a TGA image.
void tga_free(tga_t *tga);

#endif
