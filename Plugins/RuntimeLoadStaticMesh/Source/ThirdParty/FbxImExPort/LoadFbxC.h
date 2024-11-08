#ifndef LOAD_FBX_C_H
#define LOAD_FBX_C_H
#include "FbxSetting.h"

//fdata :FbxFullData_p 
extern "C" FBXIMEXPORT void  loadFbxMemory(const char* FbxFilePathUTF8, void *& fdata, FbxIOSetting *setting = 0);
extern "C" FBXIMEXPORT void  freeFbxMemory(void *& fdata);


#endif
