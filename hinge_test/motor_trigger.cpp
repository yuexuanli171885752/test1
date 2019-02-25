#include "interface.h"
#include "inf_motion_aa.h"



//===========================================================================================
// TriggerSM101 Thread
//===========================================================================================
DWORD TriggerSM101(LPVOID lpvThreadParm)
{
	SM101_TRIGGER = 0x00;
	log("sm101 ready.");
	while(1){
		while(SM101_TRIGGER==0x00) Sleep(20);
		log("sm101_trigger=%d", SM101_TRIGGER);
		SM101_MOTION_ERR = (SM101_fun[SM101_TRIGGER])(); 
		SM101_TRIGGER = 0x00;
		Sleep(2);
	}
	return(0);
}

//===========================================================================================
// TriggerSM102 Thread
//===========================================================================================
DWORD TriggerSM102(LPVOID lpvThreadParm)
{
	SM102_TRIGGER = 0x00;
	log("sm102 ready.");
	while(1){
		while(SM102_TRIGGER==0x00) Sleep(20);
		log("sm102_trigger=%d", SM102_TRIGGER);
		SM102_MOTION_ERR = (SM102_fun[SM102_TRIGGER])(); 
		SM102_TRIGGER = 0x00;
		Sleep(2);
	}
	return(0);
}

//===========================================================================================
// TriggerSM103 Thread
//===========================================================================================
DWORD TriggerSM103(LPVOID lpvThreadParm)
{
	SM103_TRIGGER = 0x00;
	while(1){
		while(SM103_TRIGGER==0x00) Sleep(20);
		SM103_MOTION_ERR = (SM103_fun[SM103_TRIGGER])(); 
		SM103_TRIGGER = 0x00;
		Sleep(2);
	}
	return(0);
}

//===========================================================================================
// TriggerSM104 Thread
//===========================================================================================
DWORD TriggerSM104(LPVOID lpvThreadParm)
{
	SM104_TRIGGER = 0x00;
	log("sm104 ready");
	while(1){
		while(SM104_TRIGGER==0x00) Sleep(20);
		log("sm104_trigger=%d", SM104_TRIGGER);
		SM104_MOTION_ERR = (SM104_fun[SM104_TRIGGER])(); 
		SM104_TRIGGER = 0x00;
		Sleep(2);
	}
	return(0);
}

//===========================================================================================
// TriggerSM201 Thread
//===========================================================================================
DWORD TriggerSM105(LPVOID lpvThreadParm)
{
	SM105_TRIGGER = 0x00;
	log("sm105 ready.");
	while(1){
		while(SM105_TRIGGER==0x00) Sleep(20);
		log("sm105_trigger=%d", SM105_TRIGGER);
		SM105_MOTION_ERR = (SM105_fun[SM105_TRIGGER])(); 
		SM105_TRIGGER = 0x00;
		Sleep(2);
	}
	return(0);
}




