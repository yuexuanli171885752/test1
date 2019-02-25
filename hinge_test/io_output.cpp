#include "interface.h"
#include "inf_motion_aa.h"
#include "defio.h"
#include "s_define.h"

#pragma warning(disable:4996)
extern MOTION_SHM mshm;
//extern void SetInPutStatus_ID1();

class ioStatusDlg_output:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, UINT code_notify);
	virtual void close(HWND hdlg);

private:
};
ioStatusDlg_output io_dlg_output;

BOOL CALLBACK ioStatus_output_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, io_dlg_output.init	 );
		HANDLE_DLGMSG(hdlg, WM_COMMAND,    io_dlg_output.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,	   io_dlg_output.close);
	}
	return 0;
}

BOOL ioStatusDlg_output::init(HWND hdlg, HWND hfocus, LPARAM linit)
{


	//============================================================================================================
	initial_setting();
	//log("IO Card Init OK.");

	ai_win::read_string_key(s_test::config_path, "system", "default_recipe_setting", s_test::recipe_name, sizeof(s_test::recipe_name));
	SetDlgItemText(hdlg, IDC_SELECTED_RECIPE, s_test::recipe_name);

	char path[MAX_PATH] = {0};
	sprintf(path, "%s\\recipe\\", s_test::dir);
	char folder_name[50][FOLDER_NAME_LENGTH] = {0};
	int folder_cnt = ai_win::get_sub_folders_name(path, folder_name, 50);

	int index = 0;
	for (int i=0; i<folder_cnt; i++){
		LISTBOX_ADDSTR(hdlg, IDC_RECIPE_LIST, folder_name[i]);
		if (!strcmp(folder_name[i], s_test::recipe_name)){
			index = i;
		}
	}

	LISTBOX_SETCURSEL(hdlg, IDC_RECIPE_LIST, index);

	char title_name[MAX_PATH] = {0};
	sprintf(title_name, "Recipe Select: [%s]", s_test::recipe_name);
	SetWindowText(hdlg, title_name);

	char file_name[50][FOLDER_NAME_LENGTH] = {0};
	sprintf(path, "%s\\recipe\\io_display_output.ini", s_test::dir);

	int file_cnt = ai_win::get_sub_files_name(path, file_name, 50, "*.ini");
	for (int i=0; i<file_cnt; i++){
		LISTBOX_ADDSTR(hdlg, IDC_RECIPE_LIST_FILE, file_name[i]);
	}

	log("Dlg:recipe setting init OK, select recipe[%s]", s_test::recipe_name);
	return TRUE;
}

void ioStatusDlg_output::command(HWND hdlg, int id, HWND hctrl, UINT code_notify)
{

	switch (id)
	{
		case IDC_OUT_Y1: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?400:401);
			break;

		case IDC_OUT_Y2:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?402:403);
			break;

		case IDC_OUT_Y3: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?404:405);
			break;

		case IDC_OUT_Y4:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?406:407);
			break;

		case IDC_OUT_Y5: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?408:409);
			break;

		case IDC_OUT_Y6:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?410:411);
			break;

		case IDC_OUT_Y7: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?412:413);
			break;

		case IDC_OUT_Y8:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?414:415);
			break;

		case IDC_OUT_Y9: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?416:417);
			break;

		case IDC_OUT_Y10:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?418:419);
			break;

		case IDC_OUT_Y11: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?420:421);
			break;

		case IDC_OUT_Y12:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?422:423);
			break;
		case IDC_OUT_Y13: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?424:425);
			break;

		case IDC_OUT_Y14:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?426:427);
			break;

		case IDC_OUT_Y15: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?428:429);
			break;

		case IDC_OUT_Y16:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?430:431);
			break;

		case IDC_OUT_Y17: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?432:433);
			break;

		case IDC_OUT_Y18:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?434:435);
			break;

		case IDC_OUT_Y19: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?436:437);
			break;

		case IDC_OUT_Y20:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?438:439);
			break;

		case IDC_OUT_Y21:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?440:441);
			break;

		case IDC_OUT_Y22: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?442:443);
			break;

		case IDC_OUT_Y23:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?444:445);
			break;

		case IDC_OUT_Y24: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?446:447);
			break;

		case IDC_OUT_Y25:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?448:449);
			break;

		case IDC_OUT_Y26: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?450:451);
			break;

		case IDC_OUT_Y27:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?452:453);
			break;

		case IDC_OUT_Y28: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?454:455);
			break;

		case IDC_OUT_Y29:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?456:457);
			break;
		case IDC_OUT_Y30: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?458:459);
			break;

		case IDC_OUT_Y31:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?460:461);
			break;

		case IDC_OUT_Y32: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?462:463);
			break;
		case IDC_OUT_Y33: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?464:465);
			break;

		case IDC_OUT_Y34:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?466:467);
			break;

		case IDC_OUT_Y35: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?468:469);
			break;

		case IDC_OUT_Y36:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?470:471);
			break;

		case IDC_OUT_Y37: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?472:473);
			break;

		case IDC_OUT_Y38:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?474:475);
			break;

		case IDC_OUT_Y39: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?476:477);
			break;

		case IDC_OUT_Y40:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?478:479);
			break;

		case IDC_OUT_Y41: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?480:481);
			break;

		case IDC_OUT_Y42:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?482:483);
			break;

		case IDC_OUT_Y43: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?484:485);
			break;

		case IDC_OUT_Y44:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?486:487);
			break;
		case IDC_OUT_Y45: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?488:489);
			break;

		case IDC_OUT_Y46:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?490:491);
			break;

		case IDC_OUT_Y47: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?492:493);
			break;

		case IDC_OUT_Y48:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?494:495);
			break;

		case IDC_OUT_Y49: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?496:497);
			break;

		case IDC_OUT_Y50:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?498:499);
			break;

		case IDC_OUT_Y51: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?500:501);
			break;

		case IDC_OUT_Y52:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?502:503);
			break;

		case IDC_OUT_Y53:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?504:505);
			break;

		case IDC_OUT_Y54: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?506:507);
			break;

		case IDC_OUT_Y55:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?508:509);
			break;

		case IDC_OUT_Y56: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?510:511);
			break;

		case IDC_OUT_Y57:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?512:513);
			break;

		case IDC_OUT_Y58: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?514:515);
			break;

		case IDC_OUT_Y59:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?516:517);
			break;

		case IDC_OUT_Y60: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?518:519);
			break;

		case IDC_OUT_Y61:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?520:521);
			break;
		case IDC_OUT_Y62: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?522:523);
			break;

		case IDC_OUT_Y63:
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?524:525);
			break;

		case IDC_OUT_Y64: 
			if (EV_AUTO != 0x00) break;
			BUTTON_VALUE = (GET_CHECKER(hdlg,id)?526:527);
			break;

}		
}


void ioStatusDlg_output::close(HWND hdlg)
{
	s_test::dinfo.show_io1_dlg = FALSE;
	IO_DISPLAY_FLAG = OFF;
	CheckMenuItem(s_test::hmenu, ID_IO, MF_UNCHECKED);
	EndDialog(hdlg, FALSE);
}

/*
DWORD CALLBACK ioStatusThread_ID1(LPVOID temp){
	HWND hdlg = (HWND) temp;
//	SET_CHECKER(HWND_IO_OUTPUT,IDC_OUT_Y1,);

	while(1){
// 		while(!IO_FUNCTION)
// 			Sleep(10);
		Sleep(10);
		SetInPutStatus_ID1();
//		IO_monitor_menu();
	}

	return FALSE;
}

void SetInPutStatus_ID1(){
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S1,S01);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S2,S02);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S3,S03);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S4,S04);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S5,S05);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S6,S06);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S7,S07);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S8,S08);

	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S9,S09);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S10,S10);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S11,S11);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S12,S12);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S13,S13);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S14,S14);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S15,S15);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S16,S16);

	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S17,S17);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S18,S18);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S19,S19);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S20,S20);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S20,S20);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S22,S22);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S23,S23);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S24,S24);

	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S25,S25);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S26,S26);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S27,S27);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S28,S28);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S29,S29);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S30,S30);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S31,S31);
	SET_CHECKER(HWND_IO_OUTPUT,IDC_IN_S32,S32);
}*/

//=== Motion Card related Input function ========================================================

int mn200_get_status(BYTE bLineNo, BYTE bDevNo)
{
	MOTION_IO MotionIO;
	int errcode;
	if(errcode=mn_get_mdio_status(bLineNo,bDevNo,&MotionIO) != SUCCESS)
	{
		return errcode;
	}	
	if (MotionIO.MEL==ON)
		return 0x0002;
	if (MotionIO.PEL==ON)
		return 0x0004;
	if(MotionIO.ALM=OFF)
		return 0x008;
	if(MotionIO.RDY=OFF)
		return 0x010;
	if (MotionIO.ORG==ON)
		return 0x0020;
	if (MotionIO.INP==ON)
		return 0x0040;
	if (MotionIO.SVON==ON)
		return 0x0400;
	return 0;
}

short mn_servo_motor_servo_on(BYTE bLineNo, BYTE bDevNo)
{
	short errcode;

	if((errcode = mn_servo_on(bLineNo, bDevNo,TURN_ON)) != SUCCESS){
		return(errcode);
	}
	if(bLineNo==0x00){
		mshm.CARD1_WR4_STATUS |= bDevNo;
	}
	else if(bLineNo==0x01){
		mshm.CARD2_WR4_STATUS |= bDevNo;
	}
	else if(bLineNo==0x02){
		mshm.CARD3_WR4_STATUS |= bDevNo;//£¿
	}
	return(SUCCESS);
}

short mn_servo_motor_servo_off(BYTE bLineNo, BYTE bDevNo)
{
	short errcode;

	if((errcode = mn_servo_on(bLineNo, bDevNo,TURN_OFF)) != SUCCESS){
		log("servo on.");
		return(errcode);
	}
	if(bLineNo==0x00){
		mshm.CARD1_WR4_STATUS &= (bDevNo^0xff);
	}
	else if(bLineNo==0x01){                           
		mshm.CARD2_WR4_STATUS &= (bDevNo^0xff);
	}
	else if(bLineNo==0x02){
		mshm.CARD3_WR4_STATUS &= (bDevNo^0xff);
	}
	HOME_FLAG = OFF;
	log("servo motor,home flag=%d",HOME_FLAG);
	return(SUCCESS);
}


//=== Read Digital Input Byte ===================================================================
/*
BYTE PISODIO_InputByte(WORD wBoardNo,WORD wPortNo)
{
DWORD dwDIVal;

	Ixud_ReadDI(wBoardNo, wPortNo,&dwDIVal);
	return((BYTE)(dwDIVal & 0xFF));
}

//=== Write Digital output Byte ===================================================================
void PISODIO_OutputByte(WORD wBoardNo, WORD wPortNo, DWORD dwDOVal)
{
	Ixud_WriteDO(wBoardNo, wPortNo, dwDOVal);
	return;
}*/
WORD MN200_RemoteInputWord(BYTE LineNo, BYTE DeviceNo, BYTE WordNo)
{
	WORD wDIVal;

	mn_get_di_word(LineNo, DeviceNo, WordNo, &wDIVal);
	return(wDIVal);
}

//=== Write Digital output Word ===================================================================
void MN200_RemoteOutputWord(BYTE LineNo, BYTE DeviceNo, BYTE WordNo, WORD wDOVal)
{
	mn_set_do_word(LineNo, DeviceNo, WordNo, wDOVal);
//	log("ineNo=%d, DeviceNo=%d, WordNo=%d, wDOVal=%d",LineNo,DeviceNo,WordNo,wDOVal);
	return;
}


void say_error(int err_no)
{
	WaitForSingleObject(Err_hMutex,INFINITE);
		ERROR_FLAG	= 1;	// Alert
	//	ERROR_CODE	= err_no; 
		EV_AUTO		= 0x00;
		while(ERROR_FLAG==1) Sleep(200);	
	ReleaseMutex(Err_hMutex);
	return;   
}

void initial_setting (void) 
{
	EV_AUTO				= 0;
	BUTTON_VALUE		= 0;
	ERROR_FLAG			= 0;
	ERROR_CODE			= 0;

	START_BUTTON_FLAG	= OFF;
	STOP_BUTTON_FLAG	= OFF;
	STEP_BUTTON_FLAG	= OFF;
	HOME_BUTTON_FLAG	= OFF;

	mshm.OUTPUT0_STATUS	= 0x00; //Output0_Initial_Status;
	mshm.OUTPUT1_STATUS	= 0x00; //Output1_Initial_Status;
	mshm.OUTPUT2_STATUS	= 0x00; //Output2_Initial_Status;
	mshm.OUTPUT3_STATUS	= 0x00; //Output3_Initial_Status;

	outpw((WORD)0, (WORD)0,(WORD)0, (DWORD)(mshm.OUTPUT0_STATUS));			
	outpw((WORD)0, (WORD)1,(WORD)0, (DWORD)(mshm.OUTPUT1_STATUS));			
	outpw((WORD)0, (WORD)2,(WORD)0, (DWORD)(mshm.OUTPUT2_STATUS));			
	outpw((WORD)0, (WORD)3,(WORD)0, (DWORD)(mshm.OUTPUT3_STATUS));			
	return;
}