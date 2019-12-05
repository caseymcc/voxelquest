#ifndef _voxelquest_materials_h_
#define _voxelquest_materials_h_

#include <string>
#include <vector>
#include <map>

#ifdef MATERIAL_NOEXTERN
#define MATERIAL_EXTERN
#else
#define MATERIAL_EXTERN extern
#endif

MATERIAL_EXTERN std::map<std::string, std::string> includeMap;

void getMaterialString();

#endif//_voxelquest_materials_h_
