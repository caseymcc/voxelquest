#include "voxelquest/materials.h"
#include "voxelquest/jsonhelpers.h"
#include "voxelquest/helperfuncs.h"

#include <string>

void getMaterialString()
{
    std::string resString="\n";

    JSONValue* jv=fetchJSONData("materials.js", false);

    if(jv!=NULL)
    {
        JSONValue* jv2=jv->Child("materials");
        JSONValue* curChild;

        int numChildren=jv2->CountChildren();
        int i;

        std::string lastKey;

        for(i=0; i<numChildren; i++)
        {
            curChild=jv2->Child(i);
            lastKey=jv2->lastKey;

            splitStrings.clear();
            splitStrings=split(lastKey, '_');

            resString.append("const float TEX_"+splitStrings[1]+"="+i__s(i)+".0/255.0;\n");

        }

        resString.append("\n");

        includeMap["materials"]=resString;

        //cout << "\n\n" << resString << "\n\n";

    }

}
