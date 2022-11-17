#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zipfile.h"
#include "ziputil.h"

size_t find_eocd(char* buffer, size_t filelen) {
    size_t i = 0;
    printf("buffer size: %d\n", filelen);
    // Last element of the buffer
    printf("Last element: %x\n", buffer[filelen - 1]);

    for (i = filelen - 5; i > 0; i--) {
        // memcmp compares the first 4 bytes of the buffer with the EOCD signature
        // it returns 0 if the first 4 bytes of the buffer match the EOCD signature
        printf("i: %d\n", i);

        // If the first 4 bytes of the buffer match the EOCD signature
        if (memcmp(buffer + i, "PK\005\006", 4) == 0) {
            printf("Found EOCD at %d", i);
            return i;
        }
    }
    return 0;
}

void  parse(char *filename) {
    FILE *fp;
    char *buffer;
    size_t filelen;
    
    // Open the file
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    // Get the file length
    fseek(fp, 0, SEEK_END);
    filelen = ftell(fp);
    rewind(fp);

    // Read the entire file into a buffer in little endian
    buffer = (char *)malloc((filelen + 1) * sizeof(char));
    fread(buffer, filelen, 1, fp);
    fclose(fp);

    // Parse the EOCD
    printf("Buffer size: %d\nFile Size: %d\n", sizeof(buffer), filelen);
    size_t eocd_offset = find_eocd(buffer, filelen);

    if (eocd_offset == 0) {
        printf("Error: End of Central Directory not found.\n");
        exit(1);
    }
    printf("End of Central Directory found at offset %d\n", eocd_offset);

    char* eocdBuffer = buffer + eocd_offset;
    hexdump(buffer, 22);
    hexdump(buffer + eocd_offset, 22);
    // Parse the EOCD
    EOCD* eocd = parseEOCD(eocdBuffer);
    printEOCD(eocd);
    
    free(buffer);
}

EOCD* parseEOCD(char* buffer) {
    // Parse the EOCD
    EOCD* eocd = (EOCD *)malloc(sizeof(EOCD));

    // Read in the EOCD bytes
    memcpy(eocd, buffer, EOCD_HDR_SIZE);
    // Perform endian conversion
    endian_swap(eocd->signature, SIG_SIZE);
    endian_swap(eocd->diskNum, DISK_NUMBER_SIZE);
    endian_swap(eocd->startDisk, DISK_NUMBER_SIZE);
    endian_swap(eocd->numDiskEntries, NUM_ENTRIES_SIZE);
    endian_swap(eocd->totalEntries, NUM_ENTRIES_SIZE);
    endian_swap(eocd->centralDirectorySize, CENTRAL_DIR_SIZE);
    endian_swap(eocd->centralDirectoryOffset, OFFSET_SIZE);
    endian_swap(eocd->commentLength, LENGTH_SIZE);

    // Read in the comment
    int commentLength = 0;
    memcpy(&commentLength, eocd->commentLength, LENGTH_SIZE);
    if (commentLength > 0) {
        eocd->comment = (char *)malloc(commentLength * sizeof(char));
        memcpy(eocd->comment, buffer + EOCD_HDR_SIZE, commentLength);
        // Endian swap the comment
        endian_swap(eocd->comment, commentLength);
    } else {
        eocd->comment = NULL;
    }

    // Check the signature
    if (memcmp(eocd->signature, EOCD_SIGNATURE, SIG_SIZE)) {
        printf("Error: Invalid End of Central Directory signature.\n");
        exit(1);
    }


    return eocd;
}