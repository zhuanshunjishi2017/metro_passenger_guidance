#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int read_file_to_array(const char* filename, uint8_t* buffer, uint32_t max_size) {
    FILE *file;
    long file_size;
    size_t bytes_read;

    if (!filename || !buffer) {
        return -1;
    }

    file = fopen(filename, "rb");
    if (!file) {
        return -1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return -1;
    }

    file_size = ftell(file);
    if (file_size < 0 || (uint32_t)file_size > max_size) {
        fclose(file);
        return -1;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return -1;
    }

    bytes_read = fread(buffer, 1, (size_t)file_size, file);
    if (bytes_read != (size_t)file_size) {
        fclose(file);
        return -1;
    }

    fclose(file);

    return (int)bytes_read;
}
