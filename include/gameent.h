
class GameEnt
{

public:
    GameEnt();

    float gv(float* vals);

    void init(
        Singleton* _singleton
    );



    void initHuman();

private:
    Singleton* singleton;
    GamePageHolder* gph;
    GameEntNode* baseNode;
    FIVector4 basePosition;

    float defVecLength;





};
