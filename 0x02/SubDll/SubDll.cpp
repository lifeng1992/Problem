#include "SubDll.h"

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD  Reason, LPVOID Reserved){
	switch(Reason){
		case DLL_PROCESS_ATTACH:{
			;
		}break;

		case DLL_PROCESS_DETACH:{
			;
		}break;

		case DLL_THREAD_ATTACH:{
			;
		}break;

		case DLL_THREAD_DETACH:{
			;
		}

		default:break;
	}

	return TRUE;
}
