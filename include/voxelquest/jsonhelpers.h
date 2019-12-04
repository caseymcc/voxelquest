#ifndef _voxelquest_jsonhelpers_h_
#define _voxelquest_jsonhelpers_h_

#include "voxelquest/types.h"
#include "voxelquest/json.h"
#include "voxelquest/enums.h"

#define E_JSON_PARAMS(DDD) \
DDD(E_JP_STATMENUAVAILDIV) \
DDD(E_JP_LENGTH)

const std::string E_JSON_PARAM_STRINGS[]={
    E_JSON_PARAMS(DO_DESCRIPTION)
};

enum E_JSON_PARAM_VALS
{
    E_JSON_PARAMS(DO_ENUM)
};

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