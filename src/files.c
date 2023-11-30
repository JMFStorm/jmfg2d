#include <stdio.h>
#include <stdlib.h>

#include "j_assert.h"

void read_file_to_string(char* filepath, char* buffer, s64 max_bytes)
{
	FILE *file;
    s64 file_size;

    file = fopen(filepath, "rb");
    ASSERT_TRUE(file, "Opened file");

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    ASSERT_TRUE(file_size < max_bytes, "Opened file");

    fseek(file, 0, SEEK_SET);

    fread(buffer, 1, file_size, file);
    fclose(file);
}
