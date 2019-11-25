#ifndef _voxelquest_helperfuncs_h_
#define _voxelquest_helperfuncs_h_

int traceLevel = 0; int popCount = 0;

std::string i__s(int i);
std::string fi__s(float f);
std::string f__s(float f);

std::wstring s2ws(const std::string& s);


string getPaddedInt(int curInt, int maxInt);


union hex_converter{
	float f_val;
	unsigned int u_val;
};
float hexToFloat(std::string* s);






std::string floatToHex(float f);


// // return 1 on error
// char FLOAT_TO_HEX_BUF[32];
// string floatToHex(float x) {
//   sprintf(FLOAT_TO_HEX_BUF, "0x%lx", (unsigned long) x);
  
//   return string(FLOAT_TO_HEX_BUF);
  
// }



std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);




bool pairIsEqual(intPair a, intPair b);
bool pairIsNeg(intPair a);

float quickDis(float x1, float y1, float x2, float y2);

float coordDis(coordAndIndex* c1, coordAndIndex* c2);


void bubbleSortF(floatAndIndex* num, int numLength);






std::string intToString(int i);
std::string floatToString(float f);






//////////


void doTraceND(std::string traceVal0="", std::string traceVal1="", std::string traceVal2="", std::string traceVal3="", std::string traceVal4="", std::string traceVal5="", std::string traceVal6="", std::string traceVal7="", std::string traceVal8="", std::string traceVal9="", std::string traceVal10="");
void doTraceVecND(std::string traceVal0, FIVector4 *fv);
void traceBTV(std::string traceVal0, btVector3myBTV);
void doTraceVecND4(std::string traceVal0, FIVector4 *fv);

void pushTraceND(std::string traceVal0="", std::string traceVal1="", std::string traceVal2="", std::string traceVal3="", std::string traceVal4="", std::string traceVal5="", std::string traceVal6="", std::string traceVal7="", std::string traceVal8="", std::string traceVal9="", std::string traceVal10="");
void popTraceND()


void doTrace(std::string traceVal0="", std::string traceVal1="", std::string traceVal2="", std::string traceVal3="", std::string traceVal4="", std::string traceVal5="", std::string traceVal6="", std::string traceVal7="", std::string traceVal8="", std::string traceVal9="", std::string traceVal10="");
void doTraceVec(std::string traceVal0, FIVector4 *fv);

void pushTrace(std::string traceVal0="", std::string traceVal1="", std::string traceVal2="", std::string traceVal3="", std::string traceVal4="", std::string traceVal5="", std::string traceVal6="", std::string traceVal7="", std::string traceVal8="", std::string traceVal9="", std::string traceVal10="");
void popTrace();




#endif//_voxelquest__h_