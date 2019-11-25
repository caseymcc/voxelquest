#ifndef _voxelquest_myshapedrawer_h_
#define _voxelquest_myshapedrawer_h_


class MyShapeDrawer
{
public:
    MyShapeDrawer(Singleton* _singleton);
    ~MyShapeDrawer();

    void init();

    ///drawOpenGL might allocate temporary memory, stores pointer in shape userpointer
    bool enableTexture(bool enable);

    bool hasTextureEnabled() const;

    void drawCoordSystem();

    void drawSphere(btScalar radius, int lats, int longs);

    ShapeCache*	cache(btConvexShape* shape);

    void renderSquareA(float x, float y, float z);

    inline void glDrawVector(const btVector3& v) { glVertex3d(v[0], v[1], v[2]); }

    void setId(

        int bodyUID,
        int limbUID

    );
    
    void updateMat2();

    void updateMat();
       
    void pushNewMat(btScalar* m);
    void popMat();

    void drawOrient(int uid);

    void drawOpenGL(
        btScalar* m,
        const btCollisionShape* shape,
        const btVector3& color,
        int	debugMode,
        const btVector3& worldBoundsMin,
        const btVector3& worldBoundsMax,
        int uid
    );





    void drawSceneInternal(const btDiscreteDynamicsWorld* dynamicsWorld, int pass);


    void drawScene(const btDiscreteDynamicsWorld* dynamicsWorld, bool useShadows);


private:

    //clean-up memory of dynamically created shape hulls
    btAlignedObjectArray<ShapeCache*>	m_shapecaches;
    //unsigned int						m_texturehandle;
    //bool								m_textureenabled;
    //bool								m_textureinitialized;



    Singleton* singleton;




};

#endif//_voxelquest__h_