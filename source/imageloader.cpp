#include "voxelquest/imageloader.h"

#include <fstream>
#include <cassert>
#include <ios>

//Converts a four-character array to an integer, using little-endian form
int toInt(const char* bytes)
{
    return (int)(((unsigned char)bytes[3]<<24)|
        ((unsigned char)bytes[2]<<16)|
        ((unsigned char)bytes[1]<<8)|
        (unsigned char)bytes[0]);
}

//Converts a two-character array to a short, using little-endian form
short toShort(const char* bytes)
{
    return (short)(((unsigned char)bytes[1]<<8)|
        (unsigned char)bytes[0]);
}

//Reads the next four bytes as an integer, using little-endian form
int readInt(std::ifstream &input)
{
    char buffer[4];
    input.read(buffer, 4);
    return toInt(buffer);
}

//Reads the next two bytes as a short, using little-endian form
short readShort(std::ifstream &input)
{
    char buffer[2];
    input.read(buffer, 2);
    return toShort(buffer);
}


Image::Image(unsigned char* ps, int w, int h): pixels(ps), width(w), height(h)
{

}

Image::~Image()
{
    delete[] pixels;
}


int Image::getValue(int x, int y, int c)
{
    return pixels[3*(width * y+x)+c];
}

void Image::setValue(int x, int y, int c, int v)
{
    pixels[3*(width * y+x)+c]=v;
}

void Image::setAllValues(int c, int v)
{

    int i;
    int j;

    for(j=0; j<height; j++)
    {
        for(i=0; i<width; i++)
        {
            pixels[3*(width * j+i)+c]=v;
        }
    }

}

void Image::getTextureId(GLenum filterType)
{

    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB,
        width, height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels);
}


Image* loadBMP(std::string fnString)
{
    const char* filename=fnString.c_str();
    std::ifstream input;
    input.open(filename, std::ifstream::binary);
    assert(!input.fail()||!"Could not find file");
    char buffer[2];
    input.read(buffer, 2);
    assert(buffer[0]=='B' && buffer[1]=='M'||!"Not a bitmap file");
    input.ignore(8);
    int dataOffset=readInt(input);

    //Read the header
    int headerSize=readInt(input);
    int width;
    int height;
    switch(headerSize)
    {
    case 40:
        //V3
        width=readInt(input);
        height=readInt(input);
        input.ignore(2);
        assert(readShort(input)==24||!"Image is not 24 bits per pixel");
        assert(readShort(input)==0||!"Image is compressed");
        break;
    case 12:
        //OS/2 V1
        width=readShort(input);
        height=readShort(input);
        input.ignore(2);
        assert(readShort(input)==24||!"Image is not 24 bits per pixel");
        break;
    case 64:
        //OS/2 V2
        assert(!"Can't load OS/2 V2 bitmaps");
        break;
    case 108:
        //Windows V4
        assert(!"Can't load Windows V4 bitmaps");
        break;
    case 124:
        //Windows V5
        assert(!"Can't load Windows V5 bitmaps");
        break;
    default:
        assert(!"Unknown bitmap format");
    }

    //Read the data
    int bytesPerRow=((width*3+3)/4)*4-(width*3%4);
    int size=bytesPerRow*height;
    auto_array<char> pixels(new char[size]);
    input.seekg(dataOffset, std::ios_base::beg);
    input.read(pixels.get(), size);

    //Get the data into the right format
    auto_array<unsigned char> pixels2(new unsigned char[width * height*3]);


    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            for(int c=0; c<3; c++)
            {
                pixels2[3*(width * y+x)+c]=
                    pixels[bytesPerRow * y+3*x+(2-c)];
            }
        }
    }

    input.close();
    return new Image(pixels2.release(), width, height);
}

