/*
File headers.
All multi-byte values are stored in little-endian byte order.
All length fields count the length in bytes.
*/
#ifndef ZIP_FILE_H
#define ZIP_FILE_H

#define EOCD_SIGNATURE "\x06\x05\x4b\x50"
#define EOCD_SIGNATURE_BE "\x50\x4b\x05\x06"

#define CD_SIGNATURE "\x50\x4b\x01\x02"
#define CD_SIGNATURE_BE "\x02\x01\x4b\x50"

#define LFH_SIGNATURE "\x50\x4b\x03\x04"
#define LFH_SIGNATURE_BE "\x04\x03\x4b\x50"

#define BYTE_SIZE 8

#define EOCD_HDR_SIZE 22

#define SIG_SIZE 4
#define VERSION_SIZE 2

#define FLAG_SIZE 2

#define METHOD_SIZE 2

#define TIME_SIZE 2

#define CRC_SIZE 4

#define FILE_SIZE 4

#define LENGTH_SIZE 2 // Size of filename, extra fields, and comment

#define DISK_NUMBER_SIZE 2

#define INT_ATTR_SIZE 2
#define EXT_ATTR_SIZE 4

#define OFFSET_SIZE 4

#define NUM_ENTRIES_SIZE 2

#define CENTRAL_DIR_SIZE 4

typedef struct {
    char signature[SIG_SIZE];
    
    char diskNum[DISK_NUMBER_SIZE];
    char startDisk[DISK_NUMBER_SIZE];

    char numDiskEntries[NUM_ENTRIES_SIZE];
    char totalEntries[NUM_ENTRIES_SIZE];

    char centralDirectorySize[CENTRAL_DIR_SIZE];
    char centralDirectoryOffset[OFFSET_SIZE];

    char commentLength[LENGTH_SIZE];
    char *comment;
} EOCD;

typedef struct {
    char signature[SIG_SIZE];

    char versionMadeBy[VERSION_SIZE];
    char versionNeeded[VERSION_SIZE];

    char flag[FLAG_SIZE];
    char method[METHOD_SIZE];

    char time[TIME_SIZE]; // Last modification time
    char date[TIME_SIZE]; // Last modification date

    char crc[CRC_SIZE]; // CRC-32
    char compressedSize[FILE_SIZE];
    char uncompressedSize[FILE_SIZE];

    char fileNameLength[LENGTH_SIZE];
    char extraFieldLength[LENGTH_SIZE];
    char fileCommentLength[LENGTH_SIZE];

    char diskNumber[DISK_NUMBER_SIZE];
    char internalFileAttr[INT_ATTR_SIZE];
    char externalFileAttr[EXT_ATTR_SIZE];

    char localHeaderOffset[OFFSET_SIZE];
    
    char *fileName;
    char *extraField;
    char *fileComment;
} CDFH;

/*
 * Parse a ZIP file 
*/
void parse(char *filename);

/*
Parse the EOCD header of a ZIP file
:param buffer: the buffer which starts at the beginning of the EOCD header
:return: the EOCD header
*/
EOCD* parseEOCD(char *buffer);

#endif