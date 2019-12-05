#include "voxelquest/benchmarkdemo.h"
#include "voxelquest/bullethelpers.h"
#include "voxelquest/raycastbar.h"

#include <Benchmarks/TaruData.h>

#include <iostream>

#define COLLISION_RADIUS 0.0f

#pragma warning(push)
#pragma warning(disable:4305)


btRaycastBar2 raycastBar;

btDiscreteDynamicsWorld* BenchmarkDemo::getWorld()
{
	return m_dynamicsWorld;
}

void BenchmarkDemo::stepSimulation(float deltaTime)
{
	if(m_dynamicsWorld)
	{
		m_dynamicsWorld->stepSimulation(deltaTime);
	}

	if(m_benchmark==7)
	{
		castRays();

		raycastBar.draw();

	}

}

void BenchmarkDemo::removeRigidBody(btRigidBody* body)
{
	m_dynamicsWorld->removeRigidBody(body);
}


btRigidBody* BenchmarkDemo::createRigidBodyMask(
	btScalar mass, const btTransform& startTransform, btCollisionShape* shape, int maskFrom, int maskTo
)
{

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic=(mass!=0.f);

	btVector3 localInertia(0, 0, 0);
	if(isDynamic)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}


	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState=new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body=new btRigidBody(rbInfo);

	//add the body to the dynamics world 
	m_dynamicsWorld->addRigidBody(body, maskFrom, maskTo); //

	return body;
}


void BenchmarkDemo::updateGraphicsObjects()
{
	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}


void BenchmarkDemo::beginDrop(float x, float y, float z)
{
	initOffset=btVector3(x, y, z);
	createTest5();
	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

void BenchmarkDemo::initPhysics()
{
	std::cout<<"BenchmarkDemo:initPhysics()\n";


	m_guiHelper->setUpAxis(2);

	setCameraDistance(btScalar(100.));

	///collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConstructionInfo cci;
	cci.m_defaultMaxPersistentManifoldPoolSize=32768;
	m_collisionConfiguration=new btDefaultCollisionConfiguration(cci);

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher=new	btCollisionDispatcher(m_collisionConfiguration);

	m_dispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);



	///the maximum size of the collision world. Make sure objects stay within these boundaries
	///Don't make the world AABB size too large, it will harm simulation quality and performance
	btVector3 worldAabbMin(-16384, -16384, -16384);
	btVector3 worldAabbMax(16384, 16384, 16384);

	//	btHashedOverlappingPairCache* pairCache = new btHashedOverlappingPairCache();
	//	m_broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,3500,pairCache);
	//	m_broadphase = new btSimpleBroadphase();
	m_broadphase=new btDbvtBroadphase();


	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol=new btSequentialImpulseConstraintSolver;


	m_solver=sol;

	btDiscreteDynamicsWorld* dynamicsWorld;
	m_dynamicsWorld=dynamicsWorld=new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);


	///the following 3 lines increase the performance dramatically, with a little bit of loss of quality
	m_dynamicsWorld->getSolverInfo().m_solverMode|=SOLVER_ENABLE_FRICTION_DIRECTION_CACHING; //don't recalculate friction values each frame
	dynamicsWorld->getSolverInfo().m_numIterations=5; //few solver iterations 
	//m_defaultContactProcessingThreshold = 0.f;//used when creating bodies: body->setContactProcessingThreshold(...);
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);


	m_dynamicsWorld->setGravity(btVector3(0, 0, -10));





	// if (m_benchmark<5)
	// {
	// 	///create a few basic rigid bodies
	// 	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(250.),btScalar(250.),btScalar(50.)));
	// //	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),0);

	// 	m_collisionShapes.push_back(groundShape);

	// 	btTransform groundTransform;
	// 	groundTransform.setIdentity();
	// 	groundTransform.setOrigin(btVector3(0,0,-50));

	// 	//We can also use DemoApplication::createRigidBody, but for clarity it is provided here:
	// 	{
	// 		btScalar mass(0.);

	// 		//rigidbody is dynamic if and only if mass is non zero, otherwise static
	// 		bool isDynamic = (mass != 0.f);

	// 		btVector3 localInertia(0,0,0);
	// 		if (isDynamic)
	// 			groundShape->calculateLocalInertia(mass,localInertia);

	// 		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	// 		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	// 		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
	// 		btRigidBody* body = new btRigidBody(rbInfo);

	// 		//add the body to the dynamics world
	// 		m_dynamicsWorld->addRigidBody(body);
	// 	}
	// }

	// switch (m_benchmark)
	// {
	// 	case 1:
	// 		{
	// 			createTest1();
	// 			break;
	// 		}
	// 	case 2:
	// 		{
	// 			createTest2();
	// 			break;
	// 		}
	// 	case 3:
	// 		{
	// 			createTest3();
	// 			break;
	// 		}
	// 	case 4:
	// 		{
	// 			createTest4();
	// 			break;
	// 		}
	// 	case 5:
	// 		{
	// 			createTest5();
	// 			break;
	// 		}
	// 	case 6:
	// 	{
	// 		createTest6();
	// 		break;
	// 	}
	// 	case 7:
	// 	{
	// 		createTest7();
	// 		break;
	// 	}
	// 	case 8:
	// 	{
	// 		createTest8();
	// 		break;
	// 	}


	// default:
	// 	{
	// 	}			
	// }



}


void BenchmarkDemo::createTest1()
{
	// 3000
	int size=8;
	const float cubeSize=1.0f;
	float spacing=cubeSize;
	btVector3 pos(0.0f, cubeSize*2, 0.f);
	float offset=-size*(cubeSize * 2.0f+spacing) * 0.5f;

	btBoxShape* blockShape=new btBoxShape(btVector3(cubeSize-COLLISION_RADIUS, cubeSize-COLLISION_RADIUS, cubeSize-COLLISION_RADIUS));
	btVector3 localInertia(0, 0, 0);
	float mass=2.f;
	blockShape->calculateLocalInertia(mass, localInertia);

	btTransform trans;
	trans.setIdentity();

	for(int k=0; k<47; k++)
	{
		for(int j=0; j<size; j++)
		{
			pos[2]=offset+(float)j * (cubeSize * 2.0f+spacing);
			for(int i=0; i<size; i++)
			{
				pos[0]=offset+(float)i * (cubeSize * 2.0f+spacing);

				trans.setOrigin(pos);
				btRigidBody* cmbody;
				cmbody=createRigidBody(mass, trans, blockShape);
			}
		}
		offset-=0.05f * spacing * (size-1);
		//		spacing *= 1.01f;
		pos[1]+=(cubeSize * 2.0f+spacing);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Pyramid 3

void BenchmarkDemo::createWall(const btVector3& offsetPosition, int stackSize, const btVector3& boxSize)
{

	btBoxShape* blockShape=new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS, boxSize[1]-COLLISION_RADIUS, boxSize[2]-COLLISION_RADIUS));

	float mass=1.f;
	btVector3 localInertia(0, 0, 0);
	blockShape->calculateLocalInertia(mass, localInertia);

	//	btScalar  diffX = boxSize[0] * 1.0f;
	btScalar  diffY=boxSize[1]*1.0f;
	btScalar  diffZ=boxSize[2]*1.0f;

	btScalar  offset=-stackSize*(diffZ * 2.0f) * 0.5f;
	btVector3 pos(0.0f, diffY, 0.0f);

	btTransform trans;
	trans.setIdentity();

	while(stackSize)
	{
		for(int i=0; i<stackSize; i++)
		{
			pos[2]=offset+(float)i * (diffZ * 2.0f);

			trans.setOrigin(offsetPosition+pos);
			createRigidBody(mass, trans, blockShape);

		}
		offset+=diffZ;
		pos[1]+=(diffY * 2.0f);
		stackSize--;
	}
}

void BenchmarkDemo::createPyramid(const btVector3& offsetPosition, int stackSize, const btVector3& boxSize)
{
	btScalar space=0.0001f;

	btVector3 pos(0.0f, boxSize[1], 0.0f);

	btBoxShape* blockShape=new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS, boxSize[1]-COLLISION_RADIUS, boxSize[2]-COLLISION_RADIUS));
	btTransform trans;
	trans.setIdentity();

	btScalar mass=1.f;
	btVector3 localInertia(0, 0, 0);
	blockShape->calculateLocalInertia(mass, localInertia);


	btScalar diffX=boxSize[0]*1.02f;
	btScalar diffY=boxSize[1]*1.02f;
	btScalar diffZ=boxSize[2]*1.02f;

	btScalar offsetX=-stackSize*(diffX * 2.0f+space) * 0.5f;
	btScalar offsetZ=-stackSize*(diffZ * 2.0f+space) * 0.5f;
	while(stackSize)
	{
		for(int j=0; j<stackSize; j++)
		{
			pos[2]=offsetZ+(float)j * (diffZ * 2.0f+space);
			for(int i=0; i<stackSize; i++)
			{
				pos[0]=offsetX+(float)i * (diffX * 2.0f+space);
				trans.setOrigin(offsetPosition+pos);
				this->createRigidBody(mass, trans, blockShape);


			}
		}
		offsetX+=diffX;
		offsetZ+=diffZ;
		pos[1]+=(diffY * 2.0f+space);
		stackSize--;
	}

}

void BenchmarkDemo::createTowerCircle(const btVector3& offsetPosition, int stackSize, int rotSize, const btVector3& boxSize)
{

	btBoxShape* blockShape=new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS, boxSize[1]-COLLISION_RADIUS, boxSize[2]-COLLISION_RADIUS));

	btTransform trans;
	trans.setIdentity();

	float mass=1.f;
	btVector3 localInertia(0, 0, 0);
	blockShape->calculateLocalInertia(mass, localInertia);


	float radius=1.3f * rotSize * boxSize[0]/SIMD_PI;

	// create active boxes
	btQuaternion rotY(0, 1, 0, 0);
	float posY=boxSize[1];

	for(int i=0; i<stackSize; i++)
	{
		for(int j=0; j<rotSize; j++)
		{


			trans.setOrigin(offsetPosition+rotate(rotY, btVector3(0.0f, posY, radius)));
			trans.setRotation(rotY);
			createRigidBody(mass, trans, blockShape);

			rotY*=btQuaternion(btVector3(0, 1, 0), SIMD_PI/(rotSize*btScalar(0.5)));
		}

		posY+=boxSize[1]*2.0f;
		rotY*=btQuaternion(btVector3(0, 1, 0), SIMD_PI/(float)rotSize);
	}

}

void BenchmarkDemo::createTest2()
{
	setCameraDistance(btScalar(50.));
	const float cubeSize=1.0f;

	createPyramid(btVector3(-20.0f, 0.0f, 0.0f), 12, btVector3(cubeSize, cubeSize, cubeSize));
	createWall(btVector3(-2.0f, 0.0f, 0.0f), 12, btVector3(cubeSize, cubeSize, cubeSize));
	createWall(btVector3(4.0f, 0.0f, 0.0f), 12, btVector3(cubeSize, cubeSize, cubeSize));
	createWall(btVector3(10.0f, 0.0f, 0.0f), 12, btVector3(cubeSize, cubeSize, cubeSize));
	createTowerCircle(btVector3(25.0f, 0.0f, 0.0f), 8, 24, btVector3(cubeSize, cubeSize, cubeSize));

}

void BenchmarkDemo::createTest5()
{
    //setCameraDistance(btScalar(250.));
    btVector3 boxSize(1.5f, 1.5f, 1.5f);
    float boxMass=1.0f;
    float sphereRadius=1.5f;
    float sphereMass=1.0f;
    float capsuleHalf=2.0f;
    float capsuleRadius=1.0f;
    float capsuleMass=1.0f;

    btRigidBody* tempBody=NULL;

    {
        int size=10;
        int height=10;

        const float cubeSize=boxSize[0];
        float spacing=2.0f;
        btVector3 pos(0.0f, 0.0f, 20.0f);
        float offset=-size*(cubeSize * 2.0f+spacing) * 0.5f;

        int numBodies=0;

        for(int k=0; k<height; k++)
        {
            for(int j=0; j<size; j++)
            {
                pos[1]=offset+(float)j * (cubeSize * 2.0f+spacing);
                for(int i=0; i<size; i++)
                {
                    pos[0]=offset+(float)i * (cubeSize * 2.0f+spacing);
                    btVector3 bpos=btVector3(0, 0, 300)+btVector3(5.0f, 5.0f, 1.0f)*pos+initOffset;
                    int idx=rand()%9;
                    btTransform trans;
                    trans.setIdentity();
                    trans.setOrigin(bpos);

                    switch(idx)
                    {
                    case 0:case 1:case 2:
                    {
                        float r=0.5f * (idx+1);
                        btBoxShape* boxShape=new btBoxShape(boxSize*r);
                        tempBody=createRigidBody(boxMass*r, trans, boxShape);
                    }
                    break;

                    case 3:case 4:case 5:
                    {
                        float r=0.5f * (idx-3+1);
                        btSphereShape* sphereShape=new btSphereShape(sphereRadius*r);
                        tempBody=createRigidBody(sphereMass*r, trans, sphereShape);
                    }
                    break;

                    case 6:case 7:case 8:
                    {
                        float r=0.5f * (idx-6+1);
                        btCapsuleShape* capsuleShape=new btCapsuleShape(capsuleRadius*r, capsuleHalf*r);
                        tempBody=createRigidBody(capsuleMass*r, trans, capsuleShape);
                    }
                    break;
                    }

//                    tempBody->bodyUID=-1;
//                    tempBody->limbUID=-1;

                    numBodies++;
                }
            }
            offset-=0.05f * spacing * (size-1);
            spacing*=1.1f;
            pos[1]+=(cubeSize * 2.0f+spacing);
        }
    }

    //createLargeMeshBody();
}
void BenchmarkDemo::createTest6()
{
    setCameraDistance(btScalar(250.));

    btVector3 boxSize(1.5f, 1.5f, 1.5f);

    btConvexHullShape* convexHullShape=new btConvexHullShape();

    for(int i=0; i<TaruVtxCount; i++)
    {
        btVector3 vtx(TaruVtx[i*3], TaruVtx[i*3+1], TaruVtx[i*3+2]);
        convexHullShape->addPoint(vtx);
    }

    btTransform trans;
    trans.setIdentity();

    float mass=1.f;
    btVector3 localInertia(0, 0, 0);
    convexHullShape->calculateLocalInertia(mass, localInertia);


    {
        int size=10;
        int height=10;

        const float cubeSize=boxSize[0];
        float spacing=2.0f;
        btVector3 pos(0.0f, 20.0f, 0.0f);
        float offset=-size*(cubeSize * 2.0f+spacing) * 0.5f;


        for(int k=0; k<height; k++)
        {
            for(int j=0; j<size; j++)
            {
                pos[2]=offset+(float)j * (cubeSize * 2.0f+spacing);
                for(int i=0; i<size; i++)
                {
                    pos[0]=offset+(float)i * (cubeSize * 2.0f+spacing);
                    btVector3 bpos=btVector3(0, 25, 0)+btVector3(5.0f, 1.0f, 5.0f)*pos;
                    trans.setOrigin(bpos);

                    createRigidBody(mass, trans, convexHullShape);
                }
            }
            offset-=0.05f * spacing * (size-1);
            spacing*=1.1f;
            pos[1]+=(cubeSize * 2.0f+spacing);
        }
    }


    //createLargeMeshBody();
}

class RagDoll
{
    enum
    {
        BODYPART_PELVIS=0,
        BODYPART_SPINE,
        BODYPART_HEAD,

        BODYPART_LEFT_UPPER_LEG,
        BODYPART_LEFT_LOWER_LEG,

        BODYPART_RIGHT_UPPER_LEG,
        BODYPART_RIGHT_LOWER_LEG,

        BODYPART_LEFT_UPPER_ARM,
        BODYPART_LEFT_LOWER_ARM,

        BODYPART_RIGHT_UPPER_ARM,
        BODYPART_RIGHT_LOWER_ARM,

        BODYPART_COUNT
    };

    enum
    {
        JOINT_PELVIS_SPINE=0,
        JOINT_SPINE_HEAD,

        JOINT_LEFT_HIP,
        JOINT_LEFT_KNEE,

        JOINT_RIGHT_HIP,
        JOINT_RIGHT_KNEE,

        JOINT_LEFT_SHOULDER,
        JOINT_LEFT_ELBOW,

        JOINT_RIGHT_SHOULDER,
        JOINT_RIGHT_ELBOW,

        JOINT_COUNT
    };

    btDynamicsWorld* m_ownerWorld;
    btCollisionShape* m_shapes[BODYPART_COUNT];
    btRigidBody* m_bodies[BODYPART_COUNT];
    btTypedConstraint* m_joints[JOINT_COUNT];

    btRigidBody* createRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
    {
        bool isDynamic=(mass!=0.f);

        btVector3 localInertia(0, 0, 0);
        if(isDynamic)
            shape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* myMotionState=new btDefaultMotionState(startTransform);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
        btRigidBody* body=new btRigidBody(rbInfo);

        m_ownerWorld->addRigidBody(body);

        return body;
    }

public:
    RagDoll(btDynamicsWorld* ownerWorld, const btVector3& positionOffset, btScalar scale)
        : m_ownerWorld(ownerWorld)
    {
        // Setup the geometry
        m_shapes[BODYPART_PELVIS]=new btCapsuleShape(btScalar(0.15)*scale, btScalar(0.20)*scale);
        m_shapes[BODYPART_SPINE]=new btCapsuleShape(btScalar(0.15)*scale, btScalar(0.28)*scale);
        m_shapes[BODYPART_HEAD]=new btCapsuleShape(btScalar(0.10)*scale, btScalar(0.05)*scale);
        m_shapes[BODYPART_LEFT_UPPER_LEG]=new btCapsuleShape(btScalar(0.07)*scale, btScalar(0.45)*scale);
        m_shapes[BODYPART_LEFT_LOWER_LEG]=new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.37)*scale);
        m_shapes[BODYPART_RIGHT_UPPER_LEG]=new btCapsuleShape(btScalar(0.07)*scale, btScalar(0.45)*scale);
        m_shapes[BODYPART_RIGHT_LOWER_LEG]=new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.37)*scale);
        m_shapes[BODYPART_LEFT_UPPER_ARM]=new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.33)*scale);
        m_shapes[BODYPART_LEFT_LOWER_ARM]=new btCapsuleShape(btScalar(0.04)*scale, btScalar(0.25)*scale);
        m_shapes[BODYPART_RIGHT_UPPER_ARM]=new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.33)*scale);
        m_shapes[BODYPART_RIGHT_LOWER_ARM]=new btCapsuleShape(btScalar(0.04)*scale, btScalar(0.25)*scale);

        // Setup all the rigid bodies
        btTransform offset; offset.setIdentity();
        offset.setOrigin(positionOffset);

        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.), btScalar(0.)));
        m_bodies[BODYPART_PELVIS]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_PELVIS]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.2), btScalar(0.)));
        m_bodies[BODYPART_SPINE]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_SPINE]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.6), btScalar(0.)));
        m_bodies[BODYPART_HEAD]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_HEAD]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(0.65), btScalar(0.)));
        m_bodies[BODYPART_LEFT_UPPER_LEG]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_UPPER_LEG]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(0.2), btScalar(0.)));
        m_bodies[BODYPART_LEFT_LOWER_LEG]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_LOWER_LEG]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(0.18), btScalar(0.65), btScalar(0.)));
        m_bodies[BODYPART_RIGHT_UPPER_LEG]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_UPPER_LEG]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(0.18), btScalar(0.2), btScalar(0.)));
        m_bodies[BODYPART_RIGHT_LOWER_LEG]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_LOWER_LEG]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(-0.35), btScalar(1.45), btScalar(0.)));
        transform.getBasis().setEulerZYX(0, 0, M_PI_2);
        m_bodies[BODYPART_LEFT_UPPER_ARM]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_UPPER_ARM]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(-0.7), btScalar(1.45), btScalar(0.)));
        transform.getBasis().setEulerZYX(0, 0, M_PI_2);
        m_bodies[BODYPART_LEFT_LOWER_ARM]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_LOWER_ARM]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(0.35), btScalar(1.45), btScalar(0.)));
        transform.getBasis().setEulerZYX(0, 0, -M_PI_2);
        m_bodies[BODYPART_RIGHT_UPPER_ARM]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_UPPER_ARM]);

        transform.setIdentity();
        transform.setOrigin(scale*btVector3(btScalar(0.7), btScalar(1.45), btScalar(0.)));
        transform.getBasis().setEulerZYX(0, 0, -M_PI_2);
        m_bodies[BODYPART_RIGHT_LOWER_ARM]=createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_LOWER_ARM]);

        // Setup some damping on the m_bodies
        for(int i=0; i<BODYPART_COUNT; ++i)
        {
            m_bodies[i]->setDamping(btScalar(0.05), btScalar(0.85));
            m_bodies[i]->setDeactivationTime(btScalar(0.8));
            m_bodies[i]->setSleepingThresholds(btScalar(1.6), btScalar(2.5));
        }

        // Now setup the constraints
        btHingeConstraint* hingeC;
        btConeTwistConstraint* coneC;

        btTransform localA, localB;

        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, M_PI_2, 0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.15), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, M_PI_2, 0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.15), btScalar(0.)));
        hingeC=new btHingeConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_SPINE], localA, localB);
        hingeC->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
        m_joints[JOINT_PELVIS_SPINE]=hingeC;
        m_ownerWorld->addConstraint(m_joints[JOINT_PELVIS_SPINE], true);


        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, 0, M_PI_2); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.30), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, 0, M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
        coneC=new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_HEAD], localA, localB);
        coneC->setLimit(M_PI_4, M_PI_4, M_PI_2);
        m_joints[JOINT_SPINE_HEAD]=coneC;
        m_ownerWorld->addConstraint(m_joints[JOINT_SPINE_HEAD], true);


        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, 0, -M_PI_4*5); localA.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(-0.10), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, 0, -M_PI_4*5); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
        coneC=new btConeTwistConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_LEFT_UPPER_LEG], localA, localB);
        coneC->setLimit(M_PI_4, M_PI_4, 0);
        m_joints[JOINT_LEFT_HIP]=coneC;
        m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_HIP], true);

        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, M_PI_2, 0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, M_PI_2, 0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
        hingeC=new btHingeConstraint(*m_bodies[BODYPART_LEFT_UPPER_LEG], *m_bodies[BODYPART_LEFT_LOWER_LEG], localA, localB);
        hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
        m_joints[JOINT_LEFT_KNEE]=hingeC;
        m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_KNEE], true);


        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, 0, M_PI_4); localA.setOrigin(scale*btVector3(btScalar(0.18), btScalar(-0.10), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, 0, M_PI_4); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
        coneC=new btConeTwistConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_RIGHT_UPPER_LEG], localA, localB);
        coneC->setLimit(M_PI_4, M_PI_4, 0);
        m_joints[JOINT_RIGHT_HIP]=coneC;
        m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_HIP], true);

        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, M_PI_2, 0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, M_PI_2, 0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
        hingeC=new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_LEG], *m_bodies[BODYPART_RIGHT_LOWER_LEG], localA, localB);
        hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
        m_joints[JOINT_RIGHT_KNEE]=hingeC;
        m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_KNEE], true);


        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, 0, M_PI); localA.setOrigin(scale*btVector3(btScalar(-0.2), btScalar(0.15), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, 0, M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
        coneC=new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_LEFT_UPPER_ARM], localA, localB);
        coneC->setLimit(M_PI_2, M_PI_2, 0);
        m_joints[JOINT_LEFT_SHOULDER]=coneC;
        m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_SHOULDER], true);

        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, M_PI_2, 0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, M_PI_2, 0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
        hingeC=new btHingeConstraint(*m_bodies[BODYPART_LEFT_UPPER_ARM], *m_bodies[BODYPART_LEFT_LOWER_ARM], localA, localB);
        hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
        m_joints[JOINT_LEFT_ELBOW]=hingeC;
        m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_ELBOW], true);



        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, 0, 0); localA.setOrigin(scale*btVector3(btScalar(0.2), btScalar(0.15), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, 0, M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
        coneC=new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_RIGHT_UPPER_ARM], localA, localB);
        coneC->setLimit(M_PI_2, M_PI_2, 0);
        m_joints[JOINT_RIGHT_SHOULDER]=coneC;
        m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_SHOULDER], true);

        localA.setIdentity(); localB.setIdentity();
        localA.getBasis().setEulerZYX(0, M_PI_2, 0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
        localB.getBasis().setEulerZYX(0, M_PI_2, 0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
        hingeC=new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_ARM], *m_bodies[BODYPART_RIGHT_LOWER_ARM], localA, localB);
        hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
        m_joints[JOINT_RIGHT_ELBOW]=hingeC;
        m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_ELBOW], true);
    }

    virtual	~RagDoll()
    {
        int i;

        // Remove all constraints
        for(i=0; i<JOINT_COUNT; ++i)
        {
            m_ownerWorld->removeConstraint(m_joints[i]);
            delete m_joints[i]; m_joints[i]=0;
        }

        // Remove all bodies and shapes
        for(i=0; i<BODYPART_COUNT; ++i)
        {
            m_ownerWorld->removeRigidBody(m_bodies[i]);

            delete m_bodies[i]->getMotionState();

            delete m_bodies[i]; m_bodies[i]=0;
            delete m_shapes[i]; m_shapes[i]=0;
        }
    }
};

void BenchmarkDemo::createTest3()
{
    setCameraDistance(btScalar(50.));

    int size=16;

    float sizeX=1.f;
    float sizeY=1.f;

    //int rc=0;

    btScalar scale(3.5);
    btVector3 pos(0.0f, sizeY, 0.0f);
    while(size)
    {
        float offset=-size*(sizeX * 6.0f) * 0.5f;
        for(int i=0; i<size; i++)
        {
            pos[0]=offset+(float)i * (sizeX * 6.0f);

            RagDoll* ragDoll=new RagDoll(m_dynamicsWorld, pos, scale);
            m_ragdolls.push_back(ragDoll);
        }

        offset+=sizeX;
        pos[1]+=(sizeY * 7.0f);
        pos[2]-=sizeX*2.0f;
        size--;
    }

}
void BenchmarkDemo::createTest4()
{
    setCameraDistance(btScalar(50.));

    int size=8;
    const float cubeSize=1.5f;
    float spacing=cubeSize;
    btVector3 pos(0.0f, cubeSize*2, 0.0f);
    float offset=-size*(cubeSize * 2.0f+spacing) * 0.5f;

    btConvexHullShape* convexHullShape=new btConvexHullShape();

    btScalar scaling(1);

    convexHullShape->setLocalScaling(btVector3(scaling, scaling, scaling));

    for(int i=0; i<TaruVtxCount; i++)
    {
        btVector3 vtx(TaruVtx[i*3], TaruVtx[i*3+1], TaruVtx[i*3+2]);
        convexHullShape->addPoint(vtx*btScalar(1./scaling));
    }

    //this will enable polyhedral contact clipping, better quality, slightly slower
    //convexHullShape->initializePolyhedralFeatures();

    btTransform trans;
    trans.setIdentity();

    float mass=1.f;
    btVector3 localInertia(0, 0, 0);
    convexHullShape->calculateLocalInertia(mass, localInertia);

    for(int k=0; k<15; k++)
    {
        for(int j=0; j<size; j++)
        {
            pos[2]=offset+(float)j * (cubeSize * 2.0f+spacing);
            for(int i=0; i<size; i++)
            {
                pos[0]=offset+(float)i * (cubeSize * 2.0f+spacing);
                trans.setOrigin(pos);
                createRigidBody(mass, trans, convexHullShape);
            }
        }
        offset-=0.05f * spacing * (size-1);
        spacing*=1.01f;
        pos[1]+=(cubeSize * 2.0f+spacing);
    }
}

void BenchmarkDemo::initRays()
{
    raycastBar=btRaycastBar2(2500.0, 0, 50.0, m_guiHelper);
}



void BenchmarkDemo::castRays()
{
    raycastBar.cast(m_dynamicsWorld);
}

void BenchmarkDemo::createTest7()
{

    createTest6();
    setCameraDistance(btScalar(150.));
    initRays();
}


void BenchmarkDemo::createTest8()
{
    setCameraDistance(btScalar(250.));
    btVector3 boxSize(1.5f, 1.5f, 1.5f);
    float boxMass=1.0f;
    float sphereRadius=1.5f;
    float sphereMass=1.0f;
    float capsuleHalf=2.0f;
    float capsuleRadius=1.0f;
    float capsuleMass=1.0f;

    {
        int size=10;
        int height=10;

        const float cubeSize=boxSize[0];
        float spacing=2.0f;
        btVector3 pos(0.0f, 20.0f, 0.0f);
        float offset=-size*(cubeSize * 2.0f+spacing) * 0.5f;

        int numBodies=0;

        for(int k=0; k<height; k++)
        {
            for(int j=0; j<size; j++)
            {
                pos[2]=offset+(float)j * (cubeSize * 2.0f+spacing);
                for(int i=0; i<size; i++)
                {
                    pos[0]=offset+(float)i * (cubeSize * 2.0f+spacing);
                    btVector3 bpos=btVector3(0, 25, 0)+btVector3(5.0f, 1.0f, 5.0f)*pos;
                    int idx=rand()%9;
                    btTransform trans;
                    trans.setIdentity();
                    trans.setOrigin(bpos);

                    switch(idx)
                    {
                    case 0:case 1:case 2:
                    {
                        float r=0.5f * (idx+1);
                        btBoxShape* boxShape=new btBoxShape(boxSize*r);
                        createRigidBody(boxMass*r, trans, boxShape);
                    }
                    break;

                    case 3:case 4:case 5:
                    {
                        float r=0.5f * (idx-3+1);
                        btSphereShape* sphereShape=new btSphereShape(sphereRadius*r);
                        createRigidBody(sphereMass*r, trans, sphereShape);
                    }
                    break;

                    case 6:case 7:case 8:
                    {
                        float r=0.5f * (idx-6+1);
                        btCapsuleShape* capsuleShape=new btCapsuleShape(capsuleRadius*r, capsuleHalf*r);
                        createRigidBody(capsuleMass*r, trans, capsuleShape);
                    }
                    break;
                    }

                    numBodies++;
                }
            }
            offset-=0.05f * spacing * (size-1);
            spacing*=1.1f;
            pos[1]+=(cubeSize * 2.0f+spacing);
        }
    }

    //createLargeMeshBody();
}

void BenchmarkDemo::exitPhysics()
{
    int i;

    for(i=0; i<m_ragdolls.size(); i++)
    {
        RagDoll* doll=m_ragdolls[i];
        delete doll;
    }
    m_ragdolls.clear();

    CommonRigidBodyBase::exitPhysics();


}

#pragma warning(pop)