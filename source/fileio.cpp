#include "voxelquest/fileio.h"

bool createFolder(string folderNameStr)
{

    std::wstring folderNameWstr=s2ws(folderNameStr);

    if(
        CreateDirectory(folderNameWstr.c_str(), NULL)
        )
    {
        return true;
    }
    else
    {
        if(GetLastError()==ERROR_ALREADY_EXISTS)
        {
            return true;
        }
    }
    return false;
}


std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate|std::ifstream::binary);
    return in.tellg();
}


bool loadFile(string fnString, charArr *dest)
{

    const char* fileName=fnString.c_str();

    doTraceND("Loading: ", fileName);

    if(dest==NULL)
    {
        doTraceND("Null Data");
        return false;
    }

    std::ifstream infile(fileName, std::ifstream::in);


    if(!infile.is_open())
    {
        doTraceND("Could Not Open File For Loading");
        return false;
    }

    // // get size of file
    // infile.seekg (0, infile.end);
    // long size = (long)infile.tellg();
    // infile.seekg (0, infile.beg);

    long mySize=filesize(fnString.c_str());

    dest->size=mySize;

    if(dest->data!=NULL)
    {
        delete[] dest->data;
        dest->data=NULL;
    }

    dest->data=new char[mySize+8];

    // read content of infile
    infile.read(dest->data, mySize);

    if(infile.bad())
    {
        doTraceND("Could Not Load From File");
        infile.close();
        return false;
    }

    infile.close();

    dest->data[mySize]='\0';

    doTraceND("Load Successful");

    return true;
}


bool saveFile(char *fileName, charArr *source)
{
    if(source->data==NULL)
    {
        doTraceND("Null Data");
        return false;
    }

    std::ofstream outfile(fileName, std::ofstream::out);

    if(!outfile.is_open())
    {
        doTraceND("Could Not Open File For Saving");
        return false;
    }

    outfile.write(source->data, source->size);

    if(outfile.bad())
    {
        doTraceND("Could Not Save To File");
        outfile.close();
        return false;
    }

    outfile.close();

    doTraceND("Save Successful");

    return true;
}

bool loadFloatArray(string fileName, float* data, int dataSizeInFloats)
{
    if(dataSizeInFloats==0)
    {
        return true;
    }

    // ifstream infile (fileName, ios::in | ios::binary);
    // infile.read(reinterpret_cast<char*>(&data), sizeof(float)*dataSizeInFloats);
    // infile.close();

    FILE * pFile;
    pFile=fopen(fileName.c_str(), "rb");

    if(pFile!=NULL)
    {
        fread(data, sizeof(float), dataSizeInFloats, pFile);
        fclose(pFile);
        return true;
    }
    else
    {
        return false;
    }
}

bool saveFloatArray(string fileName, float* data, int dataSizeInFloats)
{
    if(dataSizeInFloats==0)
    {
        return true;
    }

    // ofstream outfile (fileName, ios::out | ios::binary);
    // outfile.write(reinterpret_cast<char*>(&data), sizeof(float)*dataSizeInFloats);
    // outfile.close();

    FILE * pFile;
    pFile=fopen(fileName.c_str(), "wb");

    if(pFile!=NULL)
    {
        fwrite(data, sizeof(float), dataSizeInFloats, pFile);
        fclose(pFile);
        return true;
    }
    else
    {
        return false;
    }

}

std::string loadFileString(std::string fnString)
{
    std::ifstream t(fnString);
    std::stringstream buffer;
    buffer<<t.rdbuf();

    return buffer.str();
}

bool saveFileString(string fileName, string* source)
{
    if(source==NULL)
    {
        doTraceND("Null Data");
        return false;
    }

    std::ofstream outfile(fileName.c_str(), std::ofstream::out);

    if(!outfile.is_open())
    {
        doTraceND("Could Not Open File For Saving");
        return false;
    }

    outfile.write(source->c_str(), source->size());

    if(outfile.bad())
    {
        doTraceND("Could Not Save To File");
        outfile.close();
        return false;
    }

    outfile.close();

    cout<<"Save Successful";

    return true;
}

