/*
File headers.
All multi-byte values are stored in little-endian byte order.
All length fields count the length in bytes.
*/
#ifndef ZIP_FILE_H
#define ZIP_FILE_H

#define EOCD_SIGNATURE "PK\005\006"
#define EOCD_SIGNATURE_E "\006\005KP"
#define CDFH_SIGNATURE "PK\001\002"
#define CDFH_SIGNATURE_E "\002\001KP"
#define LFH_SIGNATURE "PK\003\004"
#define LFH_SIGNATURE_E "\004\003KP"

#define BYTE_SIZE 8

#define EOCD_HDR_SIZE 22
#define CD_HDR_SIZE 46

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
    char signature[SIG_SIZE]; // 0x06054b50
    
    u_int16_t diskNum; // Number of this disk
    u_int16_t startDisk; // Disk number where central directory starts

    u_int16_t numDiskEntries; // Number of entries on this disk
    u_int16_t totalEntries; // Total number of entries in the central directory on this disk

    u_int32_t centralDirectorySize; // Size of the central directory
    u_int32_t centralDirectoryOffset; // Offset of start of central directory relative to start of archive

    u_int16_t commentLength; // Comment length
    char* comment; // Comment
} EOCD;

typedef struct {
    char signature[SIG_SIZE]; // 0x02014b50

    u_int16_t versionMadeBy;
    u_int16_t versionNeeded;

    u_int16_t flag;
    u_int16_t method;

    u_int16_t time; // Last modification time
    u_int16_t date; // Last modification date

    u_int32_t crc; // CRC-32
    u_int32_t compressedSize;
    u_int32_t uncompressedSize;

    u_int16_t fileNameLength;
    u_int16_t extraFieldLength;
    u_int16_t fileCommentLength;

    u_int16_t diskNumber;
    u_int16_t internalFileAttr;
    u_int32_t externalFileAttr;

    u_int32_t localHeaderOffset;
    
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
:param eocd: the EOCD struct to fill
*/
void parseEOCD(char *buffer, EOCD *eocd);

void parseCDFH(char *buffer, CDFH *cdfh);

int validateEOCD(EOCD *eocd);

#endif