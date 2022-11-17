/*
Reads in and decompresses a zip file.
Structure of a zip file:
[Section 1]
    [LocalHeader_1] [FileData_1] 
    [LocalHeader_2] [FileData_2]
    ...
    [LocalHeader_n] [FileData_n]
[Section 2]
    [CentralDirectory]
    [EndOfCentralDirectory]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zipfile.h"

int main() {

    parse("../resources/three_files.zip");

    return 0;
}