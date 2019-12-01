#ifndef _voxelquest_jsonhelpers_h_
#define _voxelquest_jsonhelpers_h_

#include "voxelquest/types.h"
#include "voxelquest/json.h"

JSONValue* fetchJSONData(std::string dataFile, bool doClean, JSONValue* params=NULL);

bool processJSONFromString(
    std::string* sourceBuffer,
    JSONValue** destObj
);

void specialReplace(
    std::string &allTextString,
    std::string preDelim,
    std::string pstDelim
);

void jsonPostProc();

bool processJSON(
    charArr *sourceBuffer,
    charArr *saveBuffer,
    JSONValue** destObj
);

bool loadJSON(
    std::string path,
    JSONValue** destObj
);

void getJVNodeByString(
	JSONValue* rootNode,
	JSONValue** resultNode,
	std::string stringToSplit
	//, bool dd = false
);

std::vector<std::string> splitStrings;

std::string makePretty(std::string sourceString, std::string remString);

void cleanJVPointer(JSONValue** jv);

#endif//_voxelquest_jsonhelpers_h_