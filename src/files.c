#include "files.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void str_null_terminate(char* str, s64 length) {
        char* loc = &str[length];
        *loc = 0;
}

void read_file_to_ptr(char* filepath, char* buffer, s64 max_bytes) {
	FILE *file;
        s64 file_size;

        fopen_s(&file, filepath, "rb");
        assert(file);

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        assert(file_size < max_bytes);

        fseek(file, 0, SEEK_SET);

        fread(buffer, 1, file_size, file);
        str_null_terminate(buffer, file_size);

        fclose(file);
}
