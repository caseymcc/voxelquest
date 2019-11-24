
class MyGLHelper : public GUIHelperInterface  {
public:
    MyGLHelper(Singleton* _singleton, CommonGraphicsApp* glApp);


    ~MyGLHelper();


    struct CommonRenderInterface* getRenderInterface();


    void createRigidBodyGraphicsObject(btRigidBody* body, const btVector3& color);


    void createCollisionObjectGraphicsObject(btCollisionObject* body, const btVector3& color);


    int registerGraphicsShape(const float* vertices, int numvertices, const int* indices, int numIndices);


    int registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling);


    static void createCollisionShapeGraphicsObjectInternal(btCollisionShape* collisionShape, const btTransform& parentTransform, btAlignedObjectArray<GLInstanceVertex>& verticesOut, btAlignedObjectArray<int>& indicesOut);


    void createCollisionShapeGraphicsObject(btCollisionShape* collisionShape);

    void syncPhysicsToGraphics(const btDiscreteDynamicsWorld* rbWorld);




    void render(const btDiscreteDynamicsWorld* rbWorld);

    void createPhysicsDebugDrawer(btDiscreteDynamicsWorld* rbWorld);


    struct Common2dCanvasInterface*	get2dCanvasInterface();


    CommonParameterInterface* getParameterInterface();


    void setUpAxis(int axis);


    void resetCamera(float camDist, float pitch, float yaw, float camPosX, float camPosY, float camPosZ);



    void autogenerateGraphicsObjects(btDiscreteDynamicsWorld* rbWorld);


    void drawText3D(const char* txt, float posX, float posY, float posZ, float size);


    struct CommonGraphicsApp* getAppInterface()

private:
	MyGLHelperInternalData* m_data;
	Singleton* singleton;




   
	
};


