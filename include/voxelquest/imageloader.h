#ifndef _voxelquest_imageloader_h_
#define _voxelquest_imageloader_h_

#include <fstream>
#include <glbinding/gl/gl.h>

using namespace gl;

//Converts a four-character array to an integer, using little-endian form
int toInt(const char* bytes);

//Converts a two-character array to a short, using little-endian form
short toShort(const char* bytes);

//Reads the next four bytes as an integer, using little-endian form
int readInt(std::ifstream &input);

//Reads the next two bytes as a short, using little-endian form
short readShort(std::ifstream &input);



//Just like auto_ptr, but for arrays
template<class T>
class auto_array
{
private:
    T* array;
    mutable bool isReleased;
public:
    explicit auto_array(T* array_=NULL):
        array(array_), isReleased(false)
    {}

    auto_array(const auto_array<T> &aarray)
    {
        array=aarray.array;
        isReleased=aarray.isReleased;
        aarray.isReleased=true;
    }

    ~auto_array()
    {
        if(!isReleased && array!=NULL)
        {
            delete[] array;
        }
    }

    T* get() const
    {
        return array;
    }

    T &operator*() const
    {
        return *array;
    }

    void operator=(const auto_array<T> &aarray)
    {
        if(!isReleased && array!=NULL)
        {
            delete[] array;
        }
        array=aarray.array;
        isReleased=aarray.isReleased;
        aarray.isReleased=true;
    }

    T* operator->() const 
    {
        return array;
    }

    T* release()
    {
        isReleased=true;
        return array;
    }

    void reset(T* array_=NULL)
    {
        if(!isReleased && array!=NULL)
        {
            delete[] array;
        }
        array=array_;
    }

    T* operator+(int i)
    {
        return array+i;
    }

    T &operator[](int i)
    {
        return array[i];
    }
};

//Represents an image
class Image
{
public:
    Image(unsigned char* ps, int w, int h);

    ~Image();

    int getValue(int x, int y, int c);

    void setValue(int x, int y, int c, int v);

    void setAllValues(int c, int v);

    void getTextureId(GLenum filterType);

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    unsigned char *getPixels() { return pixels; }

private:
    /* An array of the form (R1, G1, B1, R2, G2, B2, ...) indicating the
     * color of each pixel in image.  Color components range from 0 to 255.
     * The array starts the bottom-left pixel, then moves right to the end
     * of the row, then moves up to the next column, and so on.  This is the
     * format in which OpenGL likes images.
     */
    unsigned char* pixels;
    int width;
    int height;
    GLuint tid;


};


Image* loadBMP(std::string fnString);


#endif//_voxelquest__h_