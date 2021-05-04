#ifndef _voxelquest_gamenetwork_h_
#define _voxelquest_gamenetwork_h_

#include "voxelquest/types.h"
#include "voxelquest/vectors.h"

const static int NA_SIZE_IN_BYTES=32;

class Singleton;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
//#include "windows.h"
#include "winsock2.h"
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SD_SEND SHUT_WR

#define closesocket(value) close(value)
#define ZeroMemory(ptr, value, size) memset(ptr, value, size);
#endif

struct NetworkAction
{
	char data[NA_SIZE_IN_BYTES];
};

enum E_NET_OPS
{
	E_NO_TERMINAL,
	E_NO_KEY_ACTION,
	E_NO_ADD_ENT,
	E_NO_REM_ENT,
	E_NO_DRAG_ENT,
	E_NO_LENGTH
};

enum E_KEYMAP
{
    KEYMAP_UP,
    KEYMAP_DOWN,
    KEYMAP_FORWARD,
    KEYMAP_BACKWARD,
    KEYMAP_LEFT,
    KEYMAP_RIGHT,
    KEYMAP_FIRE_PRIMARY,
    KEYMAP_GRAB,
    KEYMAP_THROW,
    KEYMAP_LENGTH
};

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

    void addNetworkActionForSend(NetworkAction* na);

    void applyNetworkActions();

    int socketConnect(bool doConnect);

    void socketRecv();

    void socketSend();

    void checkBufferLengthSend();

    void checkBufferLengthRecv();

    void flushNetworkActions();

    void updateSend();
    void updateRecv();

    void applyKeyAction(bool isReq, int actorId, uint keyFlags, float camRotX, float camRotY);

    Singleton* singleton;

    const static int FRAME_SIZE_IN_BYTES=256;
    const static int TOT_BUFFER_SIZE=FRAME_SIZE_IN_BYTES*256;

    FIVector4 tempVecs[8];

#ifdef _WIN32
    WSADATA wsaData;
    SOCKET ConnectSocket;
#else
    int ConnectSocket;
#endif//_WIN32

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

    uint naUintData[8];
    int naIntData[8];
    float naFloatData[8];
    bool keyMapResultUnzipped[KEYMAP_LENGTH];
};

#endif//_voxelquest__h_
