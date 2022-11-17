#ifndef ZIP_UTIL_H
#define ZIP_UTIL_H

#include <stdlib.h>

void endian_swap(char *buf, size_t len) {
    size_t offset;
    char tmp;
    for (offset = 0; offset < len / 2; offset++) {
        tmp = buf[offset];
        buf[offset] = buf[len - offset - 1];
        buf[len - offset - 1] = tmp;
    }
}

void printHex(char *buf, size_t len) {
    size_t offset;
    for (offset = 0; offset < len; offset++) {
        printf("%02x", buf[offset]);
    }
}

void hexdump(char* buf, size_t len) {
    size_t offset;
    printf("Hexdump of %d bytes:\n", len);
    printf("Line\t00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n");
    for (offset = 0; offset < len; offset++) {
        // Print line number
        if (offset % 16 == 0) {
            printf("%08x\t", offset);
        }
        printf("%x ", buf[offset]);
        if (offset % 16 == 15) {
            printf("\n");
        }
    }
    printf("\nHexdump complete.\n");
}

void printEOCD(EOCD *eocd) {
    printf("--End of Central Directory\n");
    printf("  Signature\t"); printHex(eocd->signature, 4); printf("\n");
    printf("  Disk number\t"); printHex(eocd->diskNum, 2); printf("\n");
    printf("  Start disk\t"); printHex(eocd->startDisk, 2); printf("\n");
    printf("  DiskEntries\t"); printHex(eocd->numDiskEntries, 2); printf("\n");
    printf("  CDEntries\t"); printHex(eocd->totalEntries, 2); printf("\n");
    printf("  Size of CD\t"); printHex(eocd->centralDirectorySize, 4); printf("\n");
    printf("  Offset of CD\t"); printHex(eocd->centralDirectoryOffset, 4); printf("\n");
    printf("  Comment len\t"); printHex(eocd->commentLength, 2); printf("\n");
    printf("  Comment\t%s", eocd->comment); printf("\n");
    printf("-------------------------\n");
}

#endif