//
// Created by Adam Ali on 04/01/2023.
//

#include "zipfile.h"
#include "parse_exception.h"

zipfile::zipfile() {
    eocd = nullptr;
    cdfh = nullptr;
}

zipfile::~zipfile() {
    delete eocd;
    delete cdfh;
}

void zipfile::parseEOCD(char *buffer, size_t filelen) {
    // Find the EOCD signature
    size_t eocdOffset;
    for (eocdOffset = filelen; eocdOffset > 0; eocdOffset--) {
        if (memcmp(buffer + eocdOffset, EOCD_SIGNATURE, SIG_SIZE) == 0) {
            break;
        }
    }
    if (eocdOffset == 0) {
        throw parse_exception("EOCD signature not found");
    }
    if (eocdOffset + EOCD_HDR_SIZE > filelen) {
        throw parse_exception("EOCD is truncated");
    }

    // Allocate memory for the EOCD
    eocd = new EOCD;
    // Parse the EOCD
    std::memcpy(eocd, buffer + eocdOffset, EOCD_HDR_SIZE);
    // Read comment
    if (eocd->commentLength > 0) {
        eocd->comment = new char[eocd->commentLength + 1];
        std::memcpy(eocd->comment, buffer + eocdOffset + EOCD_HDR_SIZE, eocd->commentLength);
        eocd->comment[eocd->commentLength] = '\0';
    } else {
        eocd->comment = nullptr;
    }

}

void zipfile::parseCDFH(char *buffer, size_t cd_offset) {
    // Check signature
    if (memcmp(buffer + cd_offset, CDFH_SIGNATURE, SIG_SIZE) != 0) {
        throw parse_exception("CDFH signature not found");
    }

    // Allocate memory for the CDFH
    cdfh = new CDFH;
    // Parse the CDFH
    std::memcpy(cdfh, buffer + cd_offset, CDFH_HDR_SIZE);
    // Read filename
    if (cdfh->fileNameLength > 0) {
        cdfh->fileName = new char[cdfh->fileNameLength + 1];
        std::memcpy(cdfh->fileName, buffer + cd_offset + CDFH_HDR_SIZE, cdfh->fileNameLength);
        cdfh->fileName[cdfh->fileNameLength] = '\0';
    } else {
        cdfh->fileName = nullptr;
    }
    // Read extra field
    if (cdfh->extraFieldLength > 0) {
        cdfh->extraField = new char[cdfh->extraFieldLength + 1];
        std::memcpy(cdfh->extraField, buffer + cd_offset + CDFH_HDR_SIZE + cdfh->fileNameLength, cdfh->extraFieldLength);
        cdfh->extraField[cdfh->extraFieldLength] = '\0';
    } else {
        cdfh->extraField = nullptr;
    }
    // Read file comment
    if (cdfh->fileCommentLength > 0) {
        cdfh->fileComment = new char[cdfh->fileCommentLength + 1];
        std::memcpy(cdfh->fileComment, buffer + cd_offset + CDFH_HDR_SIZE + cdfh->fileNameLength + cdfh->extraFieldLength, cdfh->fileCommentLength);
        cdfh->fileComment[cdfh->fileCommentLength] = '\0';
    } else {
        cdfh->fileComment = nullptr;
    }
}

void zipfile::parse(const char *path) {
    FILE *fp;
    char *buffer;
    size_t filelen;

    // Open the file
    fp = fopen(path, "rb");
    if (fp == NULL) {
        printf("Error opening file");
        exit(1);
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    filelen = ftell(fp);
    rewind(fp);

    // Allocate memory for buffer and read file
    buffer = (char *)malloc((filelen + 1) * sizeof(char));
    fread(buffer, filelen, 1, fp);
    fclose(fp);

    // Parse the End of Central Directory
    parseEOCD(buffer, filelen);

    // Parse the Central Directory File Header
    parseCDFH(buffer, eocd->centralDirectoryOffset);

}

void zipfile::printEOCD() {
    if (eocd == nullptr) {
        printf("EOCD not parsed");
        return;
    }

    printf("--- End of Central Directory ---\n");
    printf("\tSignature: %s\n", eocd->signature);
    printf("\tDisk number: %d\n", eocd->diskNumber);
    printf("\tDisk with start of CD: %d\n", eocd->startDisk);
    printf("\tNumber of entries on disk: %d\n", eocd->diskEntries);
    printf("\tTotal number of entries: %d\n", eocd->totalEntries);
    printf("\tSize of central directory: %d\n", eocd->centralDirectorySize);
    printf("\tOffset of central directory: %d\n", eocd->centralDirectoryOffset);
    printf("\tComment length: %d\n", eocd->commentLength);
    printf("\tComment: %s\n", eocd->comment);
    printf("-------------------------------\n");
}

void zipfile::printCDFH() {
    if (cdfh == nullptr) {
        printf("CDFH not parsed");
        return;
    }
    printf("--- Central Directory File Header ---\n");
    printf("\tSignature: %s\n", cdfh->signature);

    // Get upper byte of version (compatiblity)
    uint8_t compatability = cdfh->versionMadeBy >> 8;
    printf("\tVersion made by: %d\n", cdfh->versionMadeBy);
    printf("\t\tCompatibility: %d\n", compatability);

    // Get lower byte of version (zip spec version)
    uint8_t major = (cdfh->versionMadeBy & 0xFF) / 10;
    uint8_t minor = (cdfh->versionMadeBy & 0xFF) % 10;
    printf("\t\tZip Specification Version: %d.%d\n", major, minor);

    printf("\tVersion needed to extract: %d\n", cdfh->versionNeeded);
    printf("\t\tZip Specification Version: %d.%d\n", cdfh->versionNeeded / 10, cdfh->versionNeeded % 10);

    // Print general purpose bit flag, bit by bit
    printf("\tGeneral purpose bit flag: ");
    for (int i = 0; i < 16; i++) {
        printf("%d", (cdfh->flag >> (16 - i)) & 1);
    }
    printf("\n");

    printf("\tCompression method: %d\n", cdfh->method);
    printf("\tLast mod file time: %d\n", cdfh->time);
    printf("\tLast mod file date: %d\n", cdfh->date);
    printf("\tCRC-32: %d\n", cdfh->crc32);
    printf("\tCompressed size: %d\n", cdfh->compressedSize);
    printf("\tUncompressed size: %d\n", cdfh->uncompressedSize);
    printf("\tFile name length: %d\n", cdfh->fileNameLength);
    printf("\tExtra field length: %d\n", cdfh->extraFieldLength);
    printf("\tFile comment length: %d\n", cdfh->fileCommentLength);
    printf("\tDisk number start: %d\n", cdfh->diskNumberStart);
    printf("\tInternal file attributes: %d\n", cdfh->internalFileAttr);
    printf("\tExternal file attributes: %d\n", cdfh->externalFileAttr);
    printf("\tOffset of local header: %d\n", cdfh->localHeaderOffset);
    printf("\tFile name: %s\n", cdfh->fileName);
    printf("\tExtra field: %s\n", cdfh->extraField);
    printf("\tFile comment: %s\n", cdfh->fileComment);
    printf("-------------------------------------\n");
}

void zipfile::print() {
    // Print EOCD
    zipfile::printEOCD();
    // Print CDFH
    zipfile::printCDFH();
}