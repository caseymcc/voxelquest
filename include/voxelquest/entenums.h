#ifndef _voxelquest_entenums_h_
#define _voxelquest_entenums_h_

// do not rearrange
enum ENT_TYPE
{
    E_ENTTYPE_NPC,
    E_ENTTYPE_WEAPON,
    E_ENTTYPE_OBJ,
    E_ENTTYPE_BULLET,
    E_ENTTYPE_TRACE,
    E_ENTTYPE_DEBRIS,
    E_ENTTYPE_LENGTH
};

enum ENT_SUBTYPE
{

    E_ENTSUB_LENGTH
};


enum E_WEAPON_STATES
{
    E_WEAPON_STATE_IDLE,
    E_WEAPON_STATE_BEG,
    E_WEAPON_STATE_END,
    E_WEAPON_STATE_LENGTH
};

enum E_WEAPON_POS
{
    E_WEAPON_POS_RELAXED,

    E_WEAPON_POS_FORWARD,

    E_WEAPON_POS_LEFT,
    E_WEAPON_POS_RIGHT,
    E_WEAPON_POS_UP,
    E_WEAPON_POS_DOWN,

    E_WEAPON_POS_UP_LEFT,
    E_WEAPON_POS_UP_RIGHT,
    E_WEAPON_POS_DOWN_LEFT,
    E_WEAPON_POS_DOWN_RIGHT,

    E_WEAPON_POS_LENGTH
};



std::string boneStrings[]={

	"E_BONE_L_BEG", ///////////////
	"E_BONE_L_SHOULDER",
	"E_BONE_L_UPPERARM",
	"E_BONE_L_LOWERARM",
	"E_BONE_L_METACARPALS",
	"E_BONE_L_PROXIMAL0", // pinky
	"E_BONE_L_PROXIMAL1", // ring
	"E_BONE_L_PROXIMAL2", // middle
	"E_BONE_L_PROXIMAL3", // index
	"E_BONE_L_PROXIMAL4", // thumb
	"E_BONE_L_INTER0", // pinky
	"E_BONE_L_INTER1", // ring
	"E_BONE_L_INTER2", // middle
	"E_BONE_L_INTER3", // index
	"E_BONE_L_INTER4", // thumb
	"E_BONE_L_DISTAL0", // pinky
	"E_BONE_L_DISTAL1", // ring
	"E_BONE_L_DISTAL2", // middle
	"E_BONE_L_DISTAL3", // index
	"E_BONE_L_DISTAL4", // thumb
	"E_BONE_L_HIP",
	"E_BONE_L_UPPERLEG",
	"E_BONE_L_LOWERLEG",
	"E_BONE_L_TALUS",
	"E_BONE_L_PHALANGE",
	"E_BONE_L_END", //////////////
	"E_BONE_R_BEG", //////////////
	"E_BONE_R_SHOULDER",
	"E_BONE_R_UPPERARM",
	"E_BONE_R_LOWERARM",
	"E_BONE_R_METACARPALS",
	"E_BONE_R_PROXIMAL0", // pinky
	"E_BONE_R_PROXIMAL1", // ring
	"E_BONE_R_PROXIMAL2", // middle
	"E_BONE_R_PROXIMAL3", // index
	"E_BONE_R_PROXIMAL4", // thumb
	"E_BONE_R_INTER0", // pinky
	"E_BONE_R_INTER1", // ring
	"E_BONE_R_INTER2", // middle
	"E_BONE_R_INTER3", // index
	"E_BONE_R_INTER4", // thumb
	"E_BONE_R_DISTAL0", // pinky
	"E_BONE_R_DISTAL1", // ring
	"E_BONE_R_DISTAL2", // middle
	"E_BONE_R_DISTAL3", // index
	"E_BONE_R_DISTAL4", // thumb
	"E_BONE_R_HIP",
	"E_BONE_R_UPPERLEG",
	"E_BONE_R_LOWERLEG",
	"E_BONE_R_TALUS",
	"E_BONE_R_PHALANGE",
	"E_BONE_R_END", /////////////////
	"E_BONE_C_BEG", /////////////////
	"E_BONE_C_BASE",
	"E_BONE_C_SPINE0", // tail
	"E_BONE_C_SPINE1",
	"E_BONE_C_SPINE2",
	"E_BONE_C_SPINE3",
	"E_BONE_C_SPINE4", // neck
	"E_BONE_C_SKULL",


	"E_BONE_WEAPON_POMMEL",
	"E_BONE_WEAPON_HANDLE",
	"E_BONE_WEAPON_CENTER",
	"E_BONE_WEAPON_CROSSR",
	"E_BONE_WEAPON_CROSSL",
	"E_BONE_WEAPON_BLADER",
	"E_BONE_WEAPON_BLADEL",
	"E_BONE_WEAPON_BLADEU",

	"E_BONE_C_END" //////////////////
};

#endif//_voxelquest_entenums_h_