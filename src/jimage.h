#pragma once

#include "types.h"
#include "jmath.h"

typedef struct ImageData {
	Point size_px;
	s32 channels;
	byte* image_data;
} ImageData;

void set_vertical_flip_image_load(bool flip);

ImageData load_image_data(char* image_path);

void free_loaded_image(ImageData data);
