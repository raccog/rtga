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

```
TODO: DOCUMENT TGA FUNCTIONS 
```