#include "voxelquest/jsonhelpers.h"
#include "voxelquest/helperfuncs.h"
#include "voxelquest/baseobject.h"
#include "voxelquest/fileio.h"

#include <iostream>

std::vector<std::string> jsonPostStack;
std::string jsonPostString;

bool replaceStr(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos=str.find(from);
    if(start_pos==std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool processJSONFromString(
    std::string* sourceBuffer,
    JSONValue** destObj
)
{
    cleanJVPointer(destObj);

    *destObj=JSON::Parse(sourceBuffer->c_str());

    if(*destObj==NULL)
    {
        doTraceND("Invalid JSON\n\n");
        doAlert();
        //cout << sourceBuffer << "\n\n";
        return false;
    }
    else
    {
        doTraceND("\nValid JSON\n");
        return true;
    }

}



void specialReplace(
    std::string &allTextString,
    std::string preDelim,
    std::string pstDelim
)
{

    std::string paramName;

    jsonPostStack.clear();

    std::size_t found;
    std::size_t found2;
    std::size_t found3;

    int baseIndex=0;
    bool doCont=true;

    int preLen=(int)preDelim.size();
    int pstLen=(int)pstDelim.size();

    while(doCont)
    {
        found=allTextString.find(preDelim, baseIndex);
        if(found!=std::string::npos)
        {


            baseIndex=(int)(found+preLen);
            //allTextString[found] = ' ';

            found3=allTextString.find(' ', baseIndex);
            found2=allTextString.find(pstDelim, baseIndex);

            if(found2!=std::string::npos)
            {

                if(
                    ((found2-found)>32)|| // max var length of 32
                    (found3<found2) // found a space between the delimitters
                    )
                {

                }
                else
                {
                    baseIndex=(int)(found2+pstLen);
                    //allTextString[found2] = ' ';

                    paramName=allTextString.substr(found+preLen, (found2-found)-pstLen);

                    jsonPostStack.push_back(paramName);

                    std::cout<<"PARAM NAME "<<paramName<<"\n";

                }

                doCont=true;
            }
            else
            {
                doCont=false;
            }


        }
        else
        {
            doCont=false;
        }
    }

}

void jsonPostProc()
{

    std::size_t found=jsonPostString.find("@@", 0);
    if(found==std::string::npos)
    {
        return;
    }

    std::string preStr="\"@@";
    std::string pstStr="@@\"";

    std::cout<<"jsonPostProc\n";

    specialReplace(jsonPostString, preStr, pstStr);

    int i;
    int enumVal;
    bool doProc;

    std::string newString="";

    for(i=0; i<jsonPostStack.size(); i++)
    {
        enumVal=stringToEnum(E_JSON_PARAM_STRINGS, E_JP_LENGTH, jsonPostStack[i]);

        doProc=true;

        switch(enumVal)
        {
        case E_JP_STATMENUAVAILDIV:
            newString=i__s(E_CS_LENGTH*MAX_STAT_VALUE);
            break;
        default:
            doProc=false;
            std::cout<<"invalid JSON Post Process Enum "<<jsonPostStack[i]<<"\n";
            break;
        }

        if(doProc)
        {
            replaceStr(jsonPostString, preStr+jsonPostStack[i]+pstStr, newString);
        }




    }

    jsonPostStack.clear();

}

bool processJSON(
    charArr *sourceBuffer,
    charArr *saveBuffer,
    JSONValue** destObj
)
{

    cleanJVPointer(destObj);

    //doTraceND("destObj is now NULL");


    //doTraceND("processJSON()");

    char *buf=sourceBuffer->data;
    int len=sourceBuffer->size;
    //JSONValue *jsonVal = NULL;

    charArr nullBuffer;

    nullBuffer.data=new char[1];
    nullBuffer.data[0]='\0';
    nullBuffer.size=0;

    if(saveBuffer!=&nullBuffer)
    {
        if(saveBuffer->data!=NULL)
        {
            delete[] saveBuffer->data;
            saveBuffer->data=NULL;
        }
        saveBuffer->data=new char[len];
        strncpy(saveBuffer->data, buf, len);
        saveBuffer->size=len;
    }

    delete nullBuffer.data;
    //doTraceND("Begin JSON::Parse()");

    if(buf==NULL)
    {
        //doTraceND("buf is NULL");
        return false;
    }
    else
    {
        //doTraceND("buf is not NULL");

        jsonPostString=std::string(buf);
        jsonPostProc();


        *destObj=JSON::Parse(jsonPostString.c_str());
    }


    //doTraceND("End JSON::Parse()");


    if(*destObj==NULL)
    {
        doTraceND("Invalid JSON\n\n");
        doAlert();
        return false;
    }
    else
    {
        doTraceND("\nValid JSON\n");



        return true;
    }


}

bool loadJSON(
    std::string path,
    JSONValue** destObj
)
{

    bool res=false;

    charArr dest;
    dest.data=NULL;
    dest.size=0;

    if(loadFile(path, &dest))
    {
        charArr nullBuffer;

        nullBuffer.data=new char[1];
        nullBuffer.data[0]='\0';
        nullBuffer.size=0;

        if(processJSON(&dest, &nullBuffer, destObj))
        {
            res=true;
        }
        else
        {
            res=false;
        }

        delete nullBuffer.data;
    }
    else
    {
        res=false;
    }


    if(dest.data!=NULL)
    {
        delete[] dest.data;
        dest.data=NULL;
    }

    return res;

}

void getJVNodeByString(
	JSONValue* rootNode,
	JSONValue** resultNode,
	std::string stringToSplit
	//, bool dd = false
)
{
	//if (TEMP_DEBUG) std::cout << "getJVNodeByString(" << stringToSplit <<  ")\n";

	int i;
	*resultNode=rootNode;

    std::vector<std::string> splitStrings;
	splitStrings=split(stringToSplit, '.');

	for(i=0; i<splitStrings.size(); i++)
	{
		//if (dd) std::cout << splitStrings[i] << "\n";

		if(
			(splitStrings[i][0]>='0')&&
			(splitStrings[i][0]<='9')
			)
		{
			*resultNode=(*resultNode)->Child(
				stoi(splitStrings[i])
			);
		}
		else
		{
			if((*resultNode)->HasChild(splitStrings[i]))
			{
				*resultNode=(*resultNode)->Child(splitStrings[i]);
			}
			else
			{
				std::cout<<"NULL RESULT NODE\n";
				*resultNode=NULL;
				return;
			}
		}



	}

}


std::string makePretty(std::string sourceString, std::string remString)
{
	std::string newString=sourceString.substr(remString.size());


	std::string::size_type i;

	for(i=0; i<newString.length(); i++)
	{
		if(i==0)
		{

		}
		else
		{
			if(newString[i-1]=='_')
			{
				newString[i]=toupper(newString[i]);
			}
			else
			{
				newString[i]=tolower(newString[i]);
			}
		}

	}

	for(i=0; i<newString.length(); i++)
	{
		if(newString[i]=='_')
		{
			newString[i]=' ';
		}
	}

	return newString;

}

void cleanJVPointer(JSONValue** jv)
{

	if(*jv!=NULL)
	{
		delete *jv;
	}

	*jv=NULL;

}
