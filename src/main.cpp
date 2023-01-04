//
// Created by Adam Ali on 04/01/2023.
//
#include "zipfile.h"

int main() {
    // Try parsing a zip file
    zipfile zip;
    zip.parse("../resources/three_files.zip");
    zip.print();
    return 0;
}