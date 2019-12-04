#include "voxelquest/stylesheet.h"

StyleSheetState::StyleSheetState()
{

}

void StyleSheetState::setVal(int offsetVal, float x, float y, float z, float w)
{
	props[offsetVal]=x;
	props[offsetVal+1]=y;
	props[offsetVal+2]=z;
	props[offsetVal+3]=w;

}

void StyleSheetState::init()
{

	int i;

	for(i=0; i<E_SS_PROPS_SIZE; i++)
	{
		props[i]=0.0f;
	}
}

void StyleSheetState::copyFrom(StyleSheetState *cf)
{
	int i;

	for(i=0; i<E_SS_PROPS_SIZE; i++)
	{
		props[i]=cf->props[i];
	}
}


StyleSheet::StyleSheet()
{

}

void StyleSheet::copyFrom(StyleSheet* fromSS)
{
	compStates[E_COMP_UP].copyFrom(&(fromSS->compStates[E_COMP_UP]));
	compStates[E_COMP_OVER].copyFrom(&(fromSS->compStates[E_COMP_OVER]));
	compStates[E_COMP_DOWN].copyFrom(&(fromSS->compStates[E_COMP_DOWN]));
}

void StyleSheet::init()
{
	int i;

	for(i=0; i<E_COMP_TOTAL; i++)
	{
		compStates[i].init();
	}

}


StyleSheetResult::StyleSheetResult()
{

}

void StyleSheetResult::init(StyleSheet *_baseSS)
{
	baseSS=_baseSS;
	super::init();

	int i;

	incVal=0.1f;

	for(i=0; i<E_COMP_TOTAL; i++)
	{
		stateAmounts[i]=0.0f;
	}

	stateAmounts[E_COMP_UP]=1.0f;
}


void StyleSheetResult::incrementVal(int enumVal)
{
	int i;
	for(i=0; i<E_COMP_TOTAL; i++)
	{
		stateAmounts[i]-=incVal;
	}

	stateAmounts[enumVal]+=2.0f * incVal;

	for(i=0; i<E_COMP_TOTAL; i++)
	{
		if(stateAmounts[i]<0.0f)
		{
			stateAmounts[i]=0.0f;
		}
		if(stateAmounts[i]>1.0f)
		{
			stateAmounts[i]=1.0f;
		}
	}

	float tot=0.0f;

	for(i=0; i<E_COMP_TOTAL; i++)
	{
		tot+=stateAmounts[i];
	}
	stateTotal=tot;

}

void StyleSheetResult::update(bool mouseOver, bool mouseDown)
{
	if(mouseOver)
	{
		if(mouseDown)
		{
			incrementVal(E_COMP_DOWN);
		}
		else
		{
			incrementVal(E_COMP_OVER);
		}
	}
	else
	{
		incrementVal(E_COMP_UP);
	}

	int i;
	int j;
	float bval;

	for(i=0; i<E_SS_PROPS_SIZE; i++)
	{

		props[i]=0.0f;
		for(j=0; j<E_COMP_TOTAL; j++)
		{
			bval=baseSS->compStates[j].props[i];
			props[i]+=bval*stateAmounts[j];
		}
		props[i]/=stateTotal;
	}
}
