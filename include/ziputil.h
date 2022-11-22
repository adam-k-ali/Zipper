#ifndef ZIP_UTIL_H
#define ZIP_UTIL_H

#include <stdlib.h>
#include "zipfile.h"


void endian_swap(char *buf, size_t len) {
    size_t offset;
    char tmp;
    for (offset = 0; offset < len / 2; offset++) {
        tmp = buf[offset];
        buf[offset] = buf[len - offset - 1];
        buf[len - offset - 1] = tmp;
    }
}

/*
 * Endian swaps a 16-bit integer in-place. 
*/
void endian_swap_16(unsigned int *buf) {
    unsigned int tmp = *buf;
    *buf = (tmp >> 8) | (tmp << 8);
}

void endian_swap_32(unsigned int *buf) {
    *buf = *buf << 24 | *buf >> 24;
    *buf = *buf << 8 | *buf >> 8;
}

void printHex(char *buf, size_t len) {
    size_t offset;
    for (offset = 0; offset < len; offset++) {
        printf("%02x", buf[offset]);
    }
}

void hexdump(char* buf, size_t len) {
    size_t offset;
    printf("Hexdump of %ld bytes:\n", len);
    printf("Line\t00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n");
    for (offset = 0; offset < len; offset++) {
        // Print line number
        if (offset % 16 == 0) {
            printf("%08lx\t", offset);
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
    printf("  Signature\t"); printHex(eocd->signature, SIG_SIZE); printf("\n");
    printf("  Disk number\t%d\n", eocd->diskNum);
    printf("  Start disk\t%d\n", eocd->startDisk);
    printf("  DiskEntries\t%d\n", eocd->numDiskEntries);
    printf("  CDEntries\t%d\n", eocd->totalEntries);
    printf("  Size of CD\t%d\n", eocd->centralDirectorySize);
    printf("  Offset of CD\t%d\n", eocd->centralDirectoryOffset);
    printf("  Comment len\t%d\n", eocd->commentLength);
    printf("  Comment\t%s", eocd->comment); printf("\n");
    printf("-------------------------\n");
}

void printCDFH(CDFH *cd) {
    printf("--Central Directory\n");
    printf("  Signature\t"); printHex(cd->signature, SIG_SIZE); printf("\n");
    printf("  VersionMade\t%u\n", cd->versionMadeBy);
    printf("  VersionReq\t%u\n", cd->versionNeeded);
    printf("  Flags\t\t%u\n", cd->flag);
    printf("  Method\t%u\n", cd->method);
    printf("  Time\t\t%u\n", cd->time);
    printf("  Date\t\t%u\n", cd->date);
    printf("  CRC\t\t%u\n", cd->crc);
    printf("  Compressed\t%u\n",cd->compressedSize);
    printf("  Uncompressed\t%u\n", cd->uncompressedSize);
    printf("  Filename len\t%u\n", cd->fileNameLength);
    printf("  Extra len\t%u\n", cd->extraFieldLength);
    printf("  Comment len\t%u\n", cd->fileCommentLength);
    printf("  Disk num\t%u\n", cd->diskNumber);
    printf("  Int attr\t%u\n", cd->internalFileAttr);
    printf("  Ext attr\t%u\n", cd->externalFileAttr);
    printf("  Offset\t%u\n", cd->localHeaderOffset);

    // Print filename with length from header
    printf("  Filename\t%s\n", cd->fileName);
    printf("  Extra\t\t%s\n", cd->extraField);
    printf("  Comment\t%s\n", cd->fileComment);
    printf("-------------------------\n");
}

#endif