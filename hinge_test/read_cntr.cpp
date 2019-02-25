#include "inf_motion_aa.h"

DWORD Readcntr(LPVOID lpvThreadParm)
{
	long position;

	while (1){
		//=== update card-1 Encoder value ===================================================
		WaitForSingleObject(Motors1_hMutex,INFINITE);
			mn_get_enccounter(LINE_NO_1, DEVICE_SM101_No, &position);
			Card1_Z_Position = position;
			
			mn_get_enccounter(LINE_NO_1, DEVICE_SM102_No, &position);
			Card1_T_Position = position;
			
			mn_get_enccounter(LINE_NO_1, DEVICE_SM103_No, &position);
			Card1_Y_Position = position;
			
			mn_get_enccounter(LINE_NO_1, DEVICE_SM104_No, &position);
			Card1_X_Position = (position);

			mn_get_enccounter(LINE_NO_1, DEVICE_SM105_No, &position);
			Card1_U_Position = (position);


		ReleaseMutex(Motors1_hMutex);
		Sleep(20);



	}



	return FALSE;
}
