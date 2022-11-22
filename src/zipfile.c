#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zipfile.h"
#include "ziputil.h"

size_t find_eocd64(char* buffer, size_t filelen) {
    size_t i;
    for (i = 0; i < filelen; i++) {
        if(memcmp(buffer + i, "PK\006\006", 4) == 0) {
            return i;
        }
    }
    return 0;
}

size_t find_eocd(char* buffer, size_t filelen) {
    size_t i = 0;
    for (i = filelen - 5; i > 0; i--) {
        if (memcmp(buffer + i, "PK\005\006", 4) == 0) {
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
    printf("Buffer size: %ld\nFile Size: %ld\n", sizeof(buffer), filelen);
    size_t eocd_offset = find_eocd(buffer, filelen);

    if (eocd_offset == 0) {
        printf("Error: End of Central Directory not found.\n");
        exit(1);
    }

    char* eocdBuffer = buffer + eocd_offset;

    // Parse the EOCD
    EOCD* eocd = (EOCD*) malloc(sizeof(EOCD));
    parseEOCD(eocdBuffer, eocd);
    printEOCD(eocd);

    // Find EOCD ZIP64
    // TODO: Finish ZIP64 implementation
    size_t eocd64_offset = find_eocd64(buffer, filelen);
    if (eocd64_offset != 0) {
        printf("End of Central Directory (ZIP64) found.\n");
        char* eocd64Buffer = buffer + eocd64_offset;
    } else {
        printf("End of Central Directory (ZIP64) not found.\n");
    }

    // Parse the Central Directory
    if (eocd->centralDirectoryOffset > filelen) {
        printf("Error: Central Directory offset is greater than file size.\n");
        exit(1);
    }

    char* cdBuffer = buffer + eocd->centralDirectoryOffset;
    CDFH* cd = (CDFH*) malloc(sizeof(CDFH));
    parseCDFH(cdBuffer, cd);
    printCDFH(cd);
    
    free(buffer);
}

void parseCDFH(char* buffer, CDFH* cdfh) {
    memcpy(cdfh, buffer, CD_HDR_SIZE);

    // Parse the filename
    if (cdfh->fileNameLength > 0) {
        cdfh->fileName = (char*) malloc(cdfh->fileNameLength + 1);
        memcpy(cdfh->fileName, buffer + CD_HDR_SIZE, cdfh->fileNameLength);
        cdfh->fileName[cdfh->fileNameLength] = '\0';
    } else {
        cdfh->fileName = NULL;
    }

    // Parse the extra field
    if (cdfh->extraFieldLength > 0) {
        cdfh->extraField = (char*) malloc(cdfh->extraFieldLength + 1);
        memcpy(cdfh->extraField, buffer + CD_HDR_SIZE + cdfh->fileNameLength, cdfh->extraFieldLength);
        cdfh->extraField[cdfh->extraFieldLength] = '\0';
    } else {
        cdfh->extraField = NULL;
    }

    // Parse the file comment
    if (cdfh->fileCommentLength > 0) {
        cdfh->fileComment = (char*) malloc(cdfh->fileCommentLength + 1);
        memcpy(cdfh->fileComment, buffer + CD_HDR_SIZE + cdfh->fileNameLength + cdfh->extraFieldLength, cdfh->fileCommentLength);
        cdfh->fileComment[cdfh->fileCommentLength] = '\0';
    } else {
        cdfh->fileComment = NULL;
    }

}

void parseEOCD(char* buffer, EOCD* eocd) {
    // Read in the EOCD bytes
    memcpy(eocd, buffer, EOCD_HDR_SIZE);

    // Read in the comment
    int commentLength = eocd->commentLength;
    if (commentLength > 0) {
        eocd->comment = (char*) malloc(commentLength * sizeof(char));
        memcpy(eocd->comment, buffer + EOCD_HDR_SIZE, commentLength);
    } else {
        eocd->comment = NULL;
    }
}