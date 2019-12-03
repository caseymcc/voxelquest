#ifndef _voxelquest_benchmarkdemo_h_
#define _voxelquest_benchmarkdemo_h_

#pragma warning(push)
#pragma warning(disable:4305)
#include <CommonInterfaces/CommonRigidBodyBase.h>
#pragma warning(pop)

class BenchmarkDemo: public CommonRigidBodyBase
{

	//keep the collision shapes, for deletion/cleanup

	btAlignedObjectArray<class RagDoll*>	m_ragdolls;

	//btOverlapFilterCallback * filterCallback;

	btVector3 initOffset;

	int	m_benchmark;

	void myinit()
	{
		//??
	}

	void setCameraDistance(btScalar dist)
	{
	}
	void createTest1();
	void createTest2();
	void createTest3();
	void createTest4();
	void createTest5();
	void createTest6();
	void createTest7();
	void createTest8();


	void createWall(const btVector3& offsetPosition, int stackSize, const btVector3& boxSize);
	void createPyramid(const btVector3& offsetPosition, int stackSize, const btVector3& boxSize);
	void createTowerCircle(const btVector3& offsetPosition, int stackSize, int rotSize, const btVector3& boxSize);
	//void createLargeMeshBody();


	class SpuBatchRaycaster* m_batchRaycaster;
	class btThreadSupportInterface* m_batchRaycasterThreadSupport;

	void castRays();
	void initRays();

public:


	BenchmarkDemo(struct GUIHelperInterface* helper, int benchmark)
		:CommonRigidBodyBase(helper),
		m_benchmark(benchmark)
	{
	}
	virtual ~BenchmarkDemo()
	{
		exitPhysics();
	}

	btDiscreteDynamicsWorld* getWorld();

	btRigidBody* createRigidBodyMask(
		btScalar mass, const btTransform& startTransform, btCollisionShape* shape, int maskFrom, int maskTo
	);

	void removeRigidBody(btRigidBody* body);

	void updateGraphicsObjects();

	void beginDrop(float x, float y, float z);

	// btRigidBody* bodyPick(const btVector3& rayFromWorld, const btVector3& rayToWorld);

	void initPhysics();

	void exitPhysics();

	void stepSimulation(float deltaTime);

	void resetCamera()
	{
		float dist=120;
		float pitch=52;
		float yaw=35;
		float targetPos[3]={0.0f, 10.46f, 0.0f};
		m_guiHelper->resetCamera(dist, pitch, yaw, targetPos[0], targetPos[1], targetPos[2]);
	}
};

// btRigidBody* BenchmarkDemo::bodyPick(const btVector3& rayFromWorld, const btVector3& rayToWorld) {

// 	if (m_dynamicsWorld==0) {
// 		cout << "world not ready\n";
// 		return NULL;
// 	}

// 	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFromWorld, rayToWorld);

// 	m_dynamicsWorld->rayTest(rayFromWorld, rayToWorld, rayCallback);
// 	if (rayCallback.hasHit())
// 	{

// 		btVector3 pickPos = rayCallback.m_hitPointWorld;
// 		btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
// 		if (body)
// 		{
// 			//other exclusions?
// 			if (!(body->isStaticObject() || body->isKinematicObject()))
// 			{

// 				return body;

// 				// m_pickedBody = body;
// 				// m_savedState = m_pickedBody->getActivationState();
// 				// m_pickedBody->setActivationState(DISABLE_DEACTIVATION);
// 				// //printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
// 				// btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
// 				// btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body, localPivot);
// 				// m_dynamicsWorld->addConstraint(p2p, true);
// 				// m_pickedConstraint = p2p;
// 				// btScalar mousePickClamping = 30.f;
// 				// p2p->m_setting.m_impulseClamp = mousePickClamping;
// 				// //very weak constraint for picking
// 				// p2p->m_setting.m_tau = 0.001f;
// 			}
// 		}


// 		// m_oldPickingPos = rayToWorld;
// 		// m_hitPos = pickPos;
// 		// m_oldPickingDist = (pickPos - rayFromWorld).length();
// 	}
// 	return NULL;

// }

#endif//_voxelquest_benchmarkdemo_h_