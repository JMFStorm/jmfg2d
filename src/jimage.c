#include "jimage.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h"

void set_vertical_flip_image_load(bool flip) {
        stbi_set_flip_vertically_on_load(flip);
}

ImageData load_image_data(char* image_path) {
        ImageData data = {0};
        data.image_data = stbi_load(image_path, &data.size_px.x, &data.size_px.y, &data.channels, 0);
        assert(data.image_data != NULL);
        return data;
}

void free_loaded_image(ImageData data) {
        stbi_image_free(data.image_data);
}
