#ifndef _voxelquest_gamephsrig_h_
#define _voxelquest_gamephsrig_h_


class GamePhysRig
{
public:
    GamePhysRig(
        Singleton* _singleton,
        int _geId,
        btDynamicsWorld* ownerWorld,
        const btVector3& positionOffset
    );
    virtual	~GamePhysRig();

    void updatePivot(int jointId);

    int addJoint(
        int nodeName,
        int parentId,
        int jointType,
        float mass,
        GameOrgNode* curNode
    );




    void initFromOrg(
        GameOrgNode* curNode,
        int curParent
    );


    void reinit();

    void removeAllBodies();

private:
    Singleton* singleton;
    btDynamicsWorld*	m_ownerWorld;
    std::vector<RigJointStruct> rigJoints;
    int geId;

    btVector3 origOffset;
    GameOrg* baseOrg;
    BaseObj* baseEnt;



    /*

    rbA = parentRigidBody
    rbB = childRigidBody
    frameInA = parentTransform - pivot relative to parent
    frameInB = childTransform - pivot relative to child

    btFixedConstraint::btFixedConstraint(
        btRigidBody& rbA,
        btRigidBody& rbB,
        const btTransform& frameInA,
        const btTransform& frameInB
    )

    */





    //btTypedConstraint** GetJoints() {return &m_joints[0];}

};






#endif//_voxelquest__h_