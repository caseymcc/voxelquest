#ifndef _voxelquest_stylesheet_h_
#define _voxelquest_stylesheet_h_

enum eSSProperties
{

	E_SS_BGCOL0_R,
	E_SS_BGCOL0_G,
	E_SS_BGCOL0_B,
	E_SS_BGCOL0_A,

	E_SS_BGCOL1_R,
	E_SS_BGCOL1_G,
	E_SS_BGCOL1_B,
	E_SS_BGCOL1_A,

	E_SS_FGCOL0_R,
	E_SS_FGCOL0_G,
	E_SS_FGCOL0_B,
	E_SS_FGCOL0_A,

	E_SS_FGCOL1_R,
	E_SS_FGCOL1_G,
	E_SS_FGCOL1_B,
	E_SS_FGCOL1_A,

	E_SS_BGCOLTEXT0_R,
	E_SS_BGCOLTEXT0_G,
	E_SS_BGCOLTEXT0_B,
	E_SS_BGCOLTEXT0_A,

	E_SS_BGCOLTEXT1_R,
	E_SS_BGCOLTEXT1_G,
	E_SS_BGCOLTEXT1_B,
	E_SS_BGCOLTEXT1_A,

	E_SS_FGCOLTEXT0_R,
	E_SS_FGCOLTEXT0_G,
	E_SS_FGCOLTEXT0_B,
	E_SS_FGCOLTEXT0_A,

	E_SS_FGCOLTEXT1_R,
	E_SS_FGCOLTEXT1_G,
	E_SS_FGCOLTEXT1_B,
	E_SS_FGCOLTEXT1_A,

	E_SS_TGCOL0_R,
	E_SS_TGCOL0_G,
	E_SS_TGCOL0_B,
	E_SS_TGCOL0_A,

	E_SS_TGCOL1_R,
	E_SS_TGCOL1_G,
	E_SS_TGCOL1_B,
	E_SS_TGCOL1_A,

	E_SS_BDCOL_R,
	E_SS_BDCOL_G,
	E_SS_BDCOL_B,
	E_SS_BDCOL_A,

	E_SS_PADDING,
	E_SS_BORDER,
	E_SS_MARGIN,
	E_SS_CORNERRAD,
	E_SS_ROUNDNESS,

	E_SS_PROPS_SIZE
};

enum eCompStates
{
	E_COMP_UP,
	E_COMP_OVER,
	E_COMP_DOWN,
	E_COMP_TOTAL
};

class StyleSheetState
{
private:

public:
	StyleSheetState();
	
	void setVal(int offsetVal, float x, float y, float z, float w);
	
	void init();
	
	void copyFrom(StyleSheetState *cf);
	
	float props[E_SS_PROPS_SIZE];
};

class StyleSheet
{
public:
	StyleSheet();
	
	void copyFrom(StyleSheet* fromSS);
	
	void init();
	
	StyleSheetState compStates[E_COMP_TOTAL];
};

class StyleSheetResult: public StyleSheetState
{
public:
	typedef StyleSheetState super;

	StyleSheetResult();
	
	void init(StyleSheet *_baseSS);
	
	void incrementVal(int enumVal);
	
	void update(bool mouseOver, bool mouseDown);
	

	StyleSheet *baseSS;
	float incVal;
	float stateTotal;
	float stateAmounts[E_COMP_TOTAL];
};

#endif//_voxelquest_stylesheet_h_