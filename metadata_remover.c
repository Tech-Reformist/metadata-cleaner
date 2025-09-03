#include "metadata_remover.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int find_exif_segment(FILE *fd, ExifInfo *info) {
    unsigned char buffer[4096];
    size_t bytes_read;
    size_t total_offset = 0;

    if (fread(buffer, 1, 2, fd) < 2) {
    fprintf(stderr, "Error: File is too small to be a JPEG.\n");
    return -1;
    }

    if (buffer[0] != 0xFF || buffer[1] != 0xD8) {
    fprintf(stderr, "Error: File is not a valid JPEG.\n");
    return -1;
    }

    rewind(fd);

    while ((bytes_read = fread(buffer, 1, 4096, fd)) > 0) {
        for (size_t i = 0; i < bytes_read - 3; i++) {
            if (buffer[i] == 0xFF && buffer[i + 1] == 0xE1) {
                unsigned int segment_length = (buffer[i + 2] * 256) + buffer[i + 3];
                info->offset = total_offset + i;
                info->length = segment_length;

                printf(
                    "Found the EXIF marker (FFE1) at byte position %zu! | Segment length is %u bytes\n", 
                    total_offset + i, 
                    segment_length
                );
                return 0;
            }
        }
        total_offset += bytes_read;
    }
    fprintf(stderr, "Couldn't find the Exif marker...\n");
    return -1;
}

int create_clean_file(const char *src_path, const char *dest_path, const ExifInfo *info) {
    FILE *src_fd = NULL;
    FILE *dest_fd = NULL;
    unsigned char *buffer = NULL;
    int return_code = -1;

    src_fd = fopen(src_path, "rb");
    if (src_fd == NULL) {
        perror("opening source file");
        goto cleanup;
    }

    dest_fd = fopen(dest_path, "wb");
    if (dest_fd == NULL) {
        perror("opening destination file");
        goto cleanup;
    }

    // --- Copy Header ---
    buffer = malloc(info->offset);
    if (buffer == NULL) {
        perror("malloc for header");
        goto cleanup;
    }
    if (fread(buffer, 1, info->offset, src_fd) != info->offset) {
        fprintf(stderr, "Could not read the full header\n");
        goto cleanup;
    }
    if (fwrite(buffer, 1, info->offset, dest_fd) != info->offset) {
        fprintf(stderr, "Failed to write the full header\n");
        goto cleanup;
    }
    free(buffer);
    buffer = NULL;
    printf("Header copied successfully.\n");

    // --- Skip EXIF and Copy Rest of File ---
    long resume_position = info->offset + 2 + info->length;
    if (fseek(src_fd, resume_position, SEEK_SET) != 0) {
        perror("fseek");
        goto cleanup;
    }

    buffer = malloc(4096);
    if (buffer == NULL) {
        perror("malloc for chunk buffer");
        goto cleanup;
    }
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, 4096, src_fd)) > 0) {
        if (fwrite(buffer, 1, bytes_read, dest_fd) != bytes_read) {
            fprintf(stderr, "Failed to write remaining file data\n");
            goto cleanup;
        }
    }
    printf("Clean file created successfully!\n");

    return_code = 0;

cleanup:
    if (src_fd != NULL) fclose(src_fd);
    if (dest_fd != NULL) fclose(dest_fd);
    if (buffer != NULL) free(buffer);
    return return_code;
}