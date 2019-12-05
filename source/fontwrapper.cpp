#include "voxelquest/fontwrapper.h"
#include "voxelquest/fileio.h"
#include "voxelquest/jsonhelpers.h"

#include <iostream>

FontWrapper::FontWrapper()
{
    jsRoot=NULL;
}

void FontWrapper::init(
    Singleton* _singleton,
    std::string fontName,
    bool _isIcons,
    float _fontScale,
    float _additionalOffset
)
{

    int counter;
    int i;
    int j;
    int ind;

    additionalOffset=_additionalOffset;
    fontScale=_fontScale;
    isIcons=_isIcons;

    JSONValue *cn;
    JSONValue *cc;

    singleton=_singleton;
    lastJSONBuffer.data=NULL;
    lastJSONBuffer.size=0;

    std::cout<<"\n\n\nFONT LOAD\n\n\n";

    fontImage=loadBMP("..\\data\\fonts\\"+fontName+".bmp");
    if(isIcons)
    {
        fontImage->getTextureId(GL_NEAREST);
    }
    else
    {
        fontImage->getTextureId(GL_LINEAR);
    }


    maxWidth=0.0f;
    counter=0;

    if(isIcons)
    {
        maxWidth=16.0f;
        ascender=0.0f;
        descender=-15.0f+additionalOffset;
        fontHeight=16.0f;


        for(j=0; j<fontImage->getHeight()/16; j++)
        {
            for(i=0; i<fontImage->getWidth()/16; i++)
            {
                charVals[counter].consumedW=16.0f;
                charVals[counter].consumedH=16.0f;
                charVals[counter].offsetX=0.0f;
                charVals[counter].offsetY=0.0f;
                charVals[counter].sampX=i*16.0f;
                charVals[counter].sampY=j*16.0f;
                charVals[counter].sampW=16.0f;
                charVals[counter].sampH=16.0f;

                counter++;
            }
        }

    }
    else
    {
        if(loadFile("..\\data\\fonts\\"+fontName+".js", &lastJSONBuffer))
        {
            charArr nullBuffer;

            nullBuffer.data=new char[1];
            nullBuffer.data[0]='\0';
            nullBuffer.size=0;

            if(processJSON(&lastJSONBuffer, &(nullBuffer), &jsRoot))
            {
                ascender=(float)jsRoot->Child("metrics")->Child("ascender")->number_value;
                descender=(float)jsRoot->Child("metrics")->Child("descender")->number_value;
                fontHeight=(float)jsRoot->Child("metrics")->Child("height")->number_value;

                cn=jsRoot->Child("chars");


                for(i=32; i<=126; i++)
                {
                    ind=i-32;
                    cc=cn->Child(ind);

                    charVals[i].consumedH=fontHeight;
                    charVals[i].consumedW=(float)cc->Child("width")->number_value;

                    charVals[i].offsetX=(float)cc->Child("ox")->number_value;
                    charVals[i].offsetY=(float)cc->Child("oy")->number_value;

                    charVals[i].sampX=(float)cc->Child("x")->number_value;
                    charVals[i].sampY=(float)cc->Child("y")->number_value;
                    charVals[i].sampW=(float)cc->Child("w")->number_value;
                    charVals[i].sampH=(float)cc->Child("h")->number_value;

                    if(charVals[i].consumedW>maxWidth)
                    {
                        maxWidth=charVals[i].consumedW;
                    }
                }
            }

            delete nullBuffer.data;
        }
    }
}
