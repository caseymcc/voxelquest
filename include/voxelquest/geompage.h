#ifndef _voxelquest_geompage_h_
#define _voxelquest_geompage_h_

class GeomPage
{
public:

    GeomPage()
    {

    }

    void init()
    {

        /*
            for (i = 0; i < 16; i++) {
                gameGeom.push_back(new GameGeom());
                gameGeom.back()->initRand();
            }
        */
    }

private:
    std::vector<int> containsGeomIds;
};

#endif//_voxelquest__h_