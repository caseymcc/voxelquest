#ifndef _voxelquest_fileio_h_
#define _voxelquest_fileio_h_

#include "voxelquest/types.h"

#include <string>
#include <fstream>

bool createFolder(std::string folderNameStr);

std::ifstream::pos_type filesize(const char* filename);


bool loadFile(std::string fnString, charArr *dest);

bool saveFile(char *fileName, charArr *source);

bool loadFloatArray(std::string fileName, float* data, int dataSizeInFloats);

bool saveFloatArray(std::string fileName, float* data, int dataSizeInFloats);

std::string loadFileString(std::string fnString);

bool saveFileString(std::string fileName, const std::string* source);


#endif//_voxelquest_fileio_h_
