#ifndef DLL_HEADER_H
#define DLL_HEADER_H

#ifdef FBX_ImExport
#define FBXIMEXPORT __declspec(dllexport)
#else
#define FBXIMEXPORT __declspec(dllimport)

#endif

#endif