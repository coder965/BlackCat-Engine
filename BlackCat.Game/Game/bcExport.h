// [05/01/2016 MRB]

#pragma once

#ifdef BC_GAME_DLL

#ifdef _MSC_VER

#define BC_GAME_DLL __declspec(dllexport)
#else
#endif

#else

#ifdef _MSC_VER

#define BC_GAME_DLL __declspec(dllimport)
#else
#endif

#endif