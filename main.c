#include "metadata_remover.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int return_code = 1;
    ExifInfo info;
    FILE *src_fd = NULL;

    // --- Argument Checking ---
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }
    const char *src_path = argv[1];
    const char *dest_path = argv[2];

    // --- Open Source File for Searching ---
    src_fd = fopen(src_path, "rb");
    if (src_fd == NULL) {
        perror("Error opening source file");
        goto cleanup;
    }

    // --- Find the EXIF Segment ---
    int find_result = find_exif_segment(src_fd, &info);
    
    fclose(src_fd);
    src_fd = NULL;

    // --- Act on the Result ---
    if (find_result == 0) {
        printf("EXIF segment found. Creating clean file...\n");
        if (create_clean_file(src_path, dest_path, &info) == 0) {
            return_code = 0;
        } else {
            fprintf(stderr, "Failed to create clean image.\n");
        }
    } else { 
        fprintf(stderr, "No action taken.\n");
    }

cleanup:
    if (src_fd != NULL) {
        fclose(src_fd);
    }
    
    return return_code;
}