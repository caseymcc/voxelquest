

// list<int> cachedPoolIds;
// vector<intPair> cachedOrderedIds;
// vector<PooledResource *> cachedPoolItems;



class FontWrapper
{
public:
    FontWrapper();

    void init(
        Singleton* _singleton,
        string fontName,
        bool _isIcons,
        float _fontScale,
        float _additionalOffset=0.0f
    );


private:
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



