
class GameNetwork
{
public:
    GameNetwork();

    void init(Singleton* _singleton);


    void getIntFloatLen(int opCode, int* uintLen, int* intLen, int* floatLen);


    void addNetworkAction(
        int opCode,
        uint* naUintData,
        int* naIntData,
        float* naFloatData
    );

    void addNetworkActionForSend(
        NetworkAction* na
    );

    void applyNetworkActions();


    int socketConnect(bool doConnect);


    void socketRecv();

    void socketSend();

    void checkBufferLengthSend();

    void checkBufferLengthRecv();

    void flushNetworkActions();


    void updateSend();
    void updateRecv();

private:
    Singleton* singleton;

    const static int FRAME_SIZE_IN_BYTES=256;
    const static int TOT_BUFFER_SIZE=FRAME_SIZE_IN_BYTES*256;

    FIVector4 tempVecs[8];

    WSADATA wsaData;
    SOCKET ConnectSocket;

    int recvPosInBytes;
    int sendPosInBytes;

    int recvConsumedInBytes;
    int sendConsumedInBytes;

    uint* uintPtr[8];
    int* intPtr[8];
    float* floatPtr[8];

    char recvbuf[TOT_BUFFER_SIZE];
    char sendbuf[TOT_BUFFER_SIZE];
    bool isConnected;

    std::vector<NetworkAction> netSendStack;

};

