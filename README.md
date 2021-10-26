# rtga
A toy library for interfacing with TGA image files.

# TGA image enums

```
TgaImageType: enum {
    NO_IMAGE = 0,
    UNCOMPRESSED_COLOR_MAPPED_IMAGE = 1,
    UNCOMPRESSED_TRUE_COLOR_IMAGE = 2,
    UNCOMPRESSED_BLACK_AND_WHITE_IMAGE = 3,
    RUN_LENGTH_ENCODED_COLOR_MAPPED_IMAGE = 9,
    RUN_LENGTH_ENCODED_BLACK_AND_WHITE_IMAGE = 11,
}
```

# TGA image structs

```
TgaHeader: struct {
    id_length: u8,
    color_map_type: bool,
    image_type: TgaImageType,
    color_map_first_index: u16,
    color_map_length: u16,
    color_map_pixel_depth: u8,
    x_origin: u16,
    y_origin: u16,
    width: u16,
    height: u16,
    image_pixel_depth: u16,
    descriptor: u16,
}  
```

```
TgaImage: struct {
    header: TgaHeader,
    image_id: *u8,
    color_map_data: *u8,
    image_data: *u8,
}
```

# TGA colors

```
TgaColor: struct {
    bgra: u8[4],
    bit_size: u8,
}
```

# TGA functions

## tga_alloc
Allocates memory for a blank TGA image in memory using the specifications from header.
```
// Returns:
//  TGA_SUCCESS,
//  TGA_ALLOCATION_ERROR
int tga_alloc(TgaImage *tga, TgaHeader header);
```

## tga_free
Frees allocated memory for a TGA image.
```
void tga_free(TgaImage *tga);
```

## tga_read_file
Reads a TGA image from a file
```
// Returns:
//  TGA_SUCCESS,
//  TGA_ALLOCATION_ERROR,
//  TGA_FILE_OPEN_ERROR,
//  TGA_FILE_READ_ERROR
int tga_read_file(TgaImage *tga, const char *filename);
```

## tga_write_file
Writes a TGA image into a file
```
// Returns:
//  TGA_SUCCESS,
//  TGA_FILE_OPEN_ERROR,
//  TGA_FILE_WRITE_ERROR
int tga_write_file(TgaImage *tga, const char *filename);
```

## tga_set_pixel
Sets a pixel to color

The length of color should match the pixel depth of the image.
```
void tga_set_pixel(TgaImage *tga, uint16_t x, uint16_t y, TgaColor color);
```

## tga_fill
Sets every pixel in the image to color.

The length of color should match the pixel depth of the image.
```
void tga_fill(TgaImage *tga, TgaColor color);
```

## tga_pixel_size
Returns the size of each pixel in bytes.

If the bit depth of a pixel is not divisible by 8, extra blank bits will be added for allignment.
```
uint8_t tga_pixel_size(const TgaHeader *header);
```

## tga_image_size
Returns the size of the image in bytes.

The header is not included in this size.
```
size_t tga_image_size(const TgaHeader *header);
```
