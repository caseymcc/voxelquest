#include "voxelquest/benchmarkdemo.h"

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
	cout<<"BenchmarkDemo:initPhysics()\n";


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