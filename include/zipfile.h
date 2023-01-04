/*
File headers.
All multi-byte values are stored in little-endian byte order.
All length fields count the length in bytes.
*/
#ifndef ZIP_FILE_H
#define ZIP_FILE_H

#include <stdint.h>
#include <vector>

#define EOCD_SIGNATURE "PK\005\006"
#define CDFH_SIGNATURE "PK\001\002"
#define LFH_SIGNATURE "PK\003\004"

#define EOCD_HDR_SIZE 22
#define CDFH_HDR_SIZE 46

#define SIG_SIZE 4

// Version made by
enum Version {
    MS_DOS = 0,
    AMIGA = 1,
    OPEN_VMS = 2,
    UNIX = 3,
    VM_CMS = 4,
    ATARI_ST = 5,
    OS_2_HPFS = 6,
    MACINTOSH = 7,
    Z_SYSTEM = 8,
    CP_M = 9,
    WINDOWS_NTFS = 10,
    MVS = 11,
    VSE = 12,
    ACORN_RISC = 13,
    VFAT = 14,
    ALTERNATE_MVS = 15,
    BEOS = 16,
    TANDEM = 17,
    OS_400 = 18,
    OS_X = 19
};

enum CompressionMethod {
    STORED = 0,
    SHRUNK = 1,
    REDUCED_1 = 2,
    REDUCED_2 = 3,
    REDUCED_3 = 4,
    REDUCED_4 = 5,
    IMPLODED = 6,
    DEFLATED = 8,
    ENHANCED_DEFLATED = 9,
    PKWARE_DCL_IMPLODED = 10,
    BZIP2 = 12,
    LZMA = 14,
    IBM_TERSE = 18,
    IBM_LZ77_Z = 19,
    PPMD = 98
};

typedef struct {
    char signature[SIG_SIZE]; // 0x06054b50
    
    u_int16_t diskNumber; // Number of this disk
    u_int16_t startDisk; // Disk number where central directory starts

    u_int16_t diskEntries; // Number of entries on this disk
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

    u_int32_t crc32; // CRC-32
    u_int32_t compressedSize;
    u_int32_t uncompressedSize;

    u_int16_t fileNameLength;
    u_int16_t extraFieldLength;
    u_int16_t fileCommentLength;

    u_int16_t diskNumberStart;
    u_int16_t internalFileAttr;
    u_int32_t externalFileAttr;

    u_int32_t localHeaderOffset;
    
    char *fileName;
    char *extraField;
    char *fileComment;
} CDFH;

class zipfile {
private:
    EOCD* eocd;
    CDFH* cdfh;

    /**
     * Parse the End of Central Directory
     * @param buffer The buffer containing the zip file
     * @param filelen The length of the zip file
     */
    void parseEOCD(char *buffer, size_t filelen);

    /**
     * Parse the Central Directory File Header
     * @param buffer The buffer containing the zip file
     * @param cd_offset The offset of the Central Directory (found in the EOCD)
     */
    void parseCDFH(char *buffer, size_t cd_offset);

    /**
     * Print the End of Central Directory
     */
    void printEOCD();

    /**
     * Print the Central Directory File Header
     */
    void printCDFH();
public:
    zipfile();
    ~zipfile();

    /**
     * Parse a zip file.
     * @param path Path to the zip file.
     */
    void parse(const char *path);

    /**
     * Print the parsed zip file information.
     */
    void print();
};

#endif