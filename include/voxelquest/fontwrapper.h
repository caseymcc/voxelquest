#ifndef _voxelquest_fontwrapper_h_
#define _voxelquest_fontwrapper_h_

#include "voxelquest/types.h"
#include "voxelquest/json.h"
#include "voxelquest/imageloader.h"

// list<int> cachedPoolIds;
// vector<intPair> cachedOrderedIds;
// vector<PooledResource *> cachedPoolItems;

struct CharStruct
{
    float consumedW;
    float consumedH;

    float offsetX;
    float offsetY;

    float sampX;
    float sampY;
    float sampW;
    float sampH;
};

class Singleton;

class FontWrapper
{
public:
    FontWrapper();

    void init(
        Singleton* _singleton,
        std::string fontName,
        bool _isIcons,
        float _fontScale,
        float _additionalOffset=0.0f
    );


//private:
    Singleton* singleton;

    charArr lastJSONBuffer;
    JSONValue *jsRoot;

    Image *fontImage;

    float ascender;
    float descender;
    float fontHeight;

    float maxWidth;
    float fontScale;
    float additionalOffset;

    bool isIcons;

    CharStruct charVals[4096];


};




#endif//_voxelquest__h_