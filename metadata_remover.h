#ifndef METADATA_REMOVER_H
#define METADATA_REMOVER_H

#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t offset;
    size_t length;
} ExifInfo;

/**
 * Searches a file for the EXIF (FFE1) segment.
 * @param fd A valid, open file pointer to the source image.
 * @param info A pointer to an ExifInfo struct that will be filled if the segment is found.
 * @return 0 on success (if found), -1 on failure (if not found).
 */
int find_exif_segment(FILE *fd, ExifInfo *info);

/**
 * Creates a new image file with the EXIF segment removed.
 * @param src_path The path to the original image file.
 * @param dest_path The path where the new, clean image will be saved.
 * @param info A pointer to the ExifInfo struct containing the location of the segment to remove.
 */
int create_clean_file(const char *src_path, const char *dest_path, const ExifInfo *info);

#endif // METADATA_REMOVER_H