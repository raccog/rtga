#include <stdio.h>

#include "rtga/rtga.h"
#include "rtga/rtga_version.h"

/*
 * All filenames
 *
 */

// Color test filenames
#define FILENAME8 "color8.tga"
#define FILENAME16 "color16.tga"
#define FILENAME24 "color24.tga"
#define FILENAME32 "color32.tga"

// Gradient test filenames
#define FILENAME8_GRADIENT "gradient8.tga"
#define FILENAME16_GRADIENT "gradient16.tga"
#define FILENAME24_GRADIENT "gradient24.tga"
#define FILENAME32_GRADIENT "gradient32.tga"

// Image
TgaImage tga;
// Image specifications
uint16_t width, height;
uint8_t pixel_depth;

void write_file(const char *filename) {
    int success = tga_write_file(&tga, filename);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", filename, width, height, pixel_depth);
    }
}

int test_color8() {
    width = 9;
    height = 9;
    pixel_depth = 8;
    if (tga_alloc(UNCOMPRESSED_BLACK_AND_WHITE_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error in function %s\n", __func__);
        return 1;
    }

    // Fill with white
    tga_fill(&tga, WHITE8);
    // Explicit greyscale colors
    tga_set_pixel(&tga, 2, 2, LIGHT_GRAY8);
    tga_set_pixel(&tga, 4, 4, GRAY8);
    tga_set_pixel(&tga, 6, 6, DARK_GRAY8);
    tga_set_pixel(&tga, 8, 8, BLACK8);
    
    write_file(FILENAME8);
    tga_free(&tga);

    return 0;
}

int test_color16() {
    width = 13;
    height = 13;
    pixel_depth = 16;
    if (tga_alloc(UNCOMPRESSED_TRUE_COLOR_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error in function %s\n", __func__);
        return 1;
    }

    // Fill with white
    tga_fill(&tga, WHITE16);
    // Explicit greyscale colors
    tga_set_pixel(&tga, 1, 1, RED16);
    tga_set_pixel(&tga, 2, 2, BLUE16);
    tga_set_pixel(&tga, 3, 3, GREEN16);
    tga_set_pixel(&tga, 4, 4, YELLOW16);
    tga_set_pixel(&tga, 5, 5, MAGENTA16);
    tga_set_pixel(&tga, 6, 6, CYAN16);
    tga_set_pixel(&tga, 7, 7, PURPLE16);
    tga_set_pixel(&tga, 8, 8, ROSE16);
    tga_set_pixel(&tga, 9, 9, ORANGE16);
    tga_set_pixel(&tga, 10, 10, LIME16);
    tga_set_pixel(&tga, 11, 11, MINT16);
    tga_set_pixel(&tga, 10, 10, AZURE16);
    tga_set_pixel(&tga, 12, 12, BLACK16);
    
    write_file(FILENAME16);
    tga_free(&tga);

    return 0;
}

int test_color24() {
    width = 13;
    height = 13;
    pixel_depth = 24;
    if (tga_alloc(UNCOMPRESSED_TRUE_COLOR_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error in function %s\n", __func__);
        return 1;
    }

    // Fill with white
    tga_fill(&tga, WHITE24);
    // Explicit greyscale colors
    tga_set_pixel(&tga, 1, 1, RED24);
    tga_set_pixel(&tga, 2, 2, BLUE24);
    tga_set_pixel(&tga, 3, 3, GREEN24);
    tga_set_pixel(&tga, 4, 4, YELLOW24);
    tga_set_pixel(&tga, 5, 5, MAGENTA24);
    tga_set_pixel(&tga, 6, 6, CYAN24);
    tga_set_pixel(&tga, 7, 7, PURPLE24);
    tga_set_pixel(&tga, 8, 8, ROSE24);
    tga_set_pixel(&tga, 9, 9, ORANGE24);
    tga_set_pixel(&tga, 10, 10, LIME24);
    tga_set_pixel(&tga, 11, 11, MINT24);
    tga_set_pixel(&tga, 10, 10, AZURE24);
    tga_set_pixel(&tga, 12, 12, BLACK24);
    
    write_file(FILENAME24);
    tga_free(&tga);

    return 0;
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
    test_color8();
    test_color16();
    test_color24();
    /*
    TgaImage tga;
    int success;
    uint16_t width, height;
    uint8_t pixel_depth;

    // File names
    const char *UC_24B_FILENAME = "color24.tga";
    const char *UC_16B_FILENAME = "color16.tga";
    const char *UC_8B_FILENAME = "color8.tga";
    const char *UC_32B_GRADIENT_FILENAME = "color32_gradient.tga";
    const char *UC_24B_GRADIENT_FILENAME = "color24_gradient.tga";
    const char *UC_8B_GRADIENT_FILENAME = "color8_gradient.tga";

    // Print package version
    printf("rtga version: %s\n", RTGA_VERSION);

    // Allocate blank image, print out the header, and then write to file

    // Allocate blank image, print out the header, and then write to file
    pixel_depth = 16;
    if (tga_alloc(UNCOMPRESSED_TRUE_COLOR_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    tga_fill(&tga, WHITE16);
    tga_set_pixel(&tga, 0, 0, BLUE16);
    tga_set_pixel(&tga, 1, 1, GREEN16);
    tga_set_pixel(&tga, 2, 2, RED16);
    tga_set_pixel(&tga, 3, 3, BLACK16);
    success = tga_write_file(&tga, UC_16B_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_16B_FILENAME, width, height, pixel_depth);
    }
    tga_free(&tga);

    // Write a grayscale image
    pixel_depth = 8;
    if (tga_alloc(UNCOMPRESSED_BLACK_AND_WHITE_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    tga_fill(&tga, BLACK8);
    tga_set_pixel(&tga, 0, 0, GRAY8);
    tga_set_pixel(&tga, 1, 1, LIGHT_GRAY8);
    success = tga_write_file(&tga, UC_8B_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_8B_FILENAME, width, height, pixel_depth);
    }
    tga_free(&tga);

    // Write a grayscale gradient image
    width = 255;
    height = 255;
    if (tga_alloc(UNCOMPRESSED_BLACK_AND_WHITE_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    for (int y = 0; y < height; ++y) {
        TgaColor gray_color = COLOR8(y);
        for (int x = 0; x < width; ++x) {
            tga_set_pixel(&tga, x, y, gray_color);
        }
    }
    success = tga_write_file(&tga, UC_8B_GRADIENT_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_8B_GRADIENT_FILENAME, width, height, pixel_depth);
    }
    tga_free(&tga);

    // Write a red-colored alpha gradient image
    pixel_depth = 32;
    if (tga_alloc(UNCOMPRESSED_TRUE_COLOR_IMAGE, width, height, pixel_depth, &tga) != TGA_SUCCESS) {
        printf("Memory allocation error on line %u\n", __LINE__);
    }
    for (int y = 0; y < height; ++y) {
        TgaColor red_alpha = COLOR32(255, 0, 0, y);
        for (int x = 0; x < width; ++x) {
            tga_set_pixel(&tga, x, y, red_alpha);
        }
    }
    success = tga_write_file(&tga, UC_32B_GRADIENT_FILENAME);
    if (success == TGA_FILE_OPEN_ERROR) {
        printf("File opening error\n");
    } else if (success == TGA_FILE_WRITE_ERROR) {
        printf("File write error\n");
    } else {
        printf("Wrote %s with Width{%u},Height{%u},Depth{%u}\n", UC_32B_GRADIENT_FILENAME, width, height, pixel_depth);
    }
    tga_free(&tga);
    */
}
