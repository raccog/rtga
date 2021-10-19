#ifndef RTGA_H
#define RTGA_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define HEADER_SIZE 18

// Return codes
#define TGA_SUCCESS 0
#define TGA_ALLOCATION_ERROR 1
#define TGA_FILE_OPEN_ERROR 2
#define TGA_FILE_READ_ERROR 3
#define TGA_FILE_WRITE_ERROR 4

//
// Colors
//

// 8-bit Greyscale Values (G)
const uint8_t *WHITE;
const uint8_t *LIGHT_GRAY;
const uint8_t *GRAY;
const uint8_t *DARK_GRAY;
const uint8_t *BLACK;

// 24-bit Color Values (BGR)
const uint8_t *BLUE;
const uint8_t *GREEN;

// Image data representations 
typedef enum {
    NO_IMAGE = 0,
    UNCOMPRESSED_COLOR_MAPPED_IMAGE = 1,
    UNCOMPRESSED_TRUE_COLOR_IMAGE = 2,
    UNCOMPRESSED_BLACK_AND_WHITE_IMAGE = 3,
    RUN_LENGTH_ENCODED_COLOR_MAPPED_IMAGE = 9,
    RUN_LENGTH_ENCODED_BLACK_AND_WHITE_IMAGE = 11,
} TgaImageType;

// TGA image header
typedef struct {
    uint8_t id_length;
    bool color_map_type;
    TgaImageType image_type;
    uint16_t color_map_first_index;
    uint16_t color_map_length;
    uint8_t color_map_pixel_depth;
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t image_pixel_depth;
    uint8_t descriptor;
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
//
// Returns:
//  TGA_SUCCESS,
//  TGA_ALLOCATION_ERROR
int tga_alloc(TgaImage *tga, TgaHeader header);

// Frees allocated memory for a TGA image.
void tga_free(TgaImage *tga);

// Reads s TGA image from a file
//
// Returns:
//  TGA_SUCCESS,
//  TGA_ALLOCATION_ERROR,
//  TGA_FILE_OPEN_ERROR,
//  TGA_FILE_READ_ERROR
int tga_read_file(TgaImage *tga, const char *filename);

// Writes a TGA image into a file
//
// Returns:
//  TGA_SUCCESS,
//  TGA_FILE_OPEN_ERROR,
//  TGA_FILE_WRITE_ERROR
int tga_write_file(TgaImage *tga, const char *filename);

// Sets a pixel to color
//
// The length of color should match the pixel depth of the image.
void tga_set_pixel(TgaImage *tga, uint16_t x, uint16_t y, const uint8_t *color);

// Returns the size of each pixel in bytes.
//
// If the bit depth of a pixel is not divisible by 8, extra blank bits
// will be added for allignment.
uint8_t tga_pixel_size(const TgaHeader *header);

// Returns the size of the image in bytes.
//
// The header is not included in this size.
size_t tga_image_size(const TgaHeader *header);

#endif
