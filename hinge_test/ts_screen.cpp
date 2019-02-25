#include "interface.h"
#include "inf_motion_aa.h"
#include "test_reg_mark_chart.h"

/*按钮反馈*/
extern MOTION_SHM mshm;
extern SlantedBW mysbw;


DWORD Tscreen(LPVOID lpvThreadParm)
{
	int cnt = 0;
	int err = 0;
	int timeout = 0;
	while (1){
		Sleep(50);
		number = BUTTON_VALUE;
		if (number == 0x00) continue;
		else{
			BUTTON_VALUE = 0x00;
		}
		MANUAL_MOVING = ON;
		HomeMachine_StopFlag = FALSE;
		SetDlgItemText(HWND_MOTOR_MOTION, IDC_STATUS2, "");
		log("init:button_val:%d", number);
		switch (number){
//MOTOR 101
		case 110: SM101_SERVO_ON;  MANUAL_ERROR = OFF;    break;
		case 111: SM101_SERVO_OFF; MANUAL_ERROR = OFF;    break;
		case 113: MANUAL_ERROR = SM101_encoder_reset();   break;	
		case 114: MANUAL_ERROR = SM101_jog_cw();		  break;
		case 115: MANUAL_ERROR = SM101_jog_ccw();		  break;
		case 116: MANUAL_ERROR = OFF; SM101_motor_stop(); break;
		case 117: MANUAL_ERROR = SM101_relative_short_move(1*SM101_SHORT_MOVE_DISTANCE);  break;
		case 118: MANUAL_ERROR = SM101_relative_short_move(-1*SM101_SHORT_MOVE_DISTANCE); break;
//MOTOR 102
		case 120: SM102_SERVO_ON;  MANUAL_ERROR = OFF;	  break;
		case 121: SM102_SERVO_OFF; MANUAL_ERROR = OFF;	  break;
		case 123: MANUAL_ERROR = SM102_encoder_reset();	  break;	
		case 124: MANUAL_ERROR = SM102_jog_cw();		  break;
		case 125: MANUAL_ERROR = SM102_jog_ccw();		  break;
		case 126: MANUAL_ERROR = OFF; SM102_motor_stop(); break;
		case 127: MANUAL_ERROR = SM102_relative_short_move(1*SM102_SHORT_MOVE_DISTANCE);  break;
		case 128: MANUAL_ERROR = SM102_relative_short_move(-1*SM102_SHORT_MOVE_DISTANCE); break;
//MOTOR 103
		case 130: SM103_SERVO_ON; MANUAL_ERROR = OFF;	  break;
		case 131: SM103_SERVO_OFF; MANUAL_ERROR = OFF;	  break;
		case 133: MANUAL_ERROR = SM103_encoder_reset();	  break;
		case 134: MANUAL_ERROR = SM103_jog_cw();		  break;
		case 135: MANUAL_ERROR = SM103_jog_ccw();		  break;
		case 136: SM103_motor_stop();MANUAL_ERROR = OFF;  break;
		case 137: MANUAL_ERROR = SM103_relative_short_move(1*SM103_SHORT_MOVE_DISTANCE);  break;
		case 138: MANUAL_ERROR = SM103_relative_short_move(-1*SM103_SHORT_MOVE_DISTANCE); break;

//MOTOR 201
		case 150: SM105_SERVO_ON;  MANUAL_ERROR = OFF;	  break;
		case 151: SM105_SERVO_OFF; MANUAL_ERROR = OFF; 	  break;
		case 153: MANUAL_ERROR = SM105_encoder_reset();   break;			
		case 154: MANUAL_ERROR = SM105_jog_cw();		  break;
		case 155: MANUAL_ERROR = SM105_jog_ccw();		  break;
		case 156: SM105_motor_stop(); MANUAL_ERROR = OFF; break;
		case 157: MANUAL_ERROR = SM105_relative_short_move(1*SM105_SHORT_MOVE_DISTANCE);  break;
		case 158: MANUAL_ERROR = SM105_relative_short_move(-1*SM105_SHORT_MOVE_DISTANCE); break;

//MOTOR 104
		case 180: SM104_SERVO_ON; MANUAL_ERROR = OFF;	  break;
		case 181: SM104_SERVO_OFF; MANUAL_ERROR = OFF;	  break;
		case 183: MANUAL_ERROR = SM104_encoder_reset();	  break;
		case 184: MANUAL_ERROR = SM104_jog_cw();		  break;
		case 185: MANUAL_ERROR = SM104_jog_ccw();		  break;
		case 186: SM104_motor_stop();MANUAL_ERROR = OFF;  break;
		case 187: MANUAL_ERROR = SM104_relative_short_move(+1*SM104_SHORT_MOVE_DISTANCE); break;
		case 188: MANUAL_ERROR = SM104_relative_short_move(-1*SM104_SHORT_MOVE_DISTANCE); break;//  not use this axis

		case 190://pick lens
			if(HOME_FLAG == FALSE){
				log("motor have not home, aa fail.");
				set_status(APP_NOTHOME);
				break;
			}
			//---------------------------------------p1
			MANUAL_ERROR = SM103_move_to_load_position1();
			if(!MANUAL_ERROR)
				MANUAL_ERROR = SM102_move_to_load_position1();
				if(!MANUAL_ERROR)
					MANUAL_ERROR = SM101_move_to_load_position1();
					if(!MANUAL_ERROR)
						MANUAL_ERROR = SM104_move_to_load_position1();
						if(!MANUAL_ERROR)
							MANUAL_ERROR = SM105_move_to_load_position2();

			if(!MANUAL_ERROR)
				break;		
			else{
				log("pick lens err.");
				break;
			}

		case 191://load lens
			if(HOME_FLAG == FALSE){
				log("motor have not home, aa fail.");
				set_status(APP_NOTHOME);
				break;
			}
			//---------------------------------------p1
			MANUAL_ERROR = SM103_move_to_load_position1();
			if(!MANUAL_ERROR){
				MANUAL_ERROR = SM104_move_to_load_position2();
				if(!MANUAL_ERROR){
					MANUAL_ERROR = SM101_move_to_load_position3();
					if(!MANUAL_ERROR){
						MANUAL_ERROR = SM102_move_to_load_position3();
						if(!MANUAL_ERROR){
							MANUAL_ERROR = SM105_move_to_load_position4();
						}
					}
				}
			}			
			if(!MANUAL_ERROR)
				break;		
			else{
				log("load lens err.");
				break;
			}


//AA分解动作
		case 200: MANUAL_ERROR = aa_move_center(); break;// **************************
/*		case 201: MANUAL_ERROR = focus_scan(); break;*/
		case 202: MANUAL_ERROR = search_z_process(); break;
		case 203: MANUAL_ERROR = calibrate_tilt_angle(); break;
		/*case 204: 		
			MANUAL_ERROR = fs_check(); 
			if(!MANUAL_ERROR)
				SetDlgItemText(HWND_MAIN,IDC_PROCESS_CHECK1,"OK");
			else
				SetDlgItemText(HWND_MAIN,IDC_PROCESS_CHECK1,"NG");
			break;*/
/*
		case 205: MANUAL_ERROR = aa_move_offset(); break;
		case 206: MANUAL_ERROR = aa_move_offset2();break;*/
		/*case 207: 
			SYSTEMTIME st1,st2;
			GetLocalTime(&st1);
			log("===Start AA===");
			SetDlgItemText(HWND_MAIN, IDC_PROCESS_PICK_LENS,	  "...");
			SetDlgItemText(HWND_MAIN, IDC_PROCESS_LOAD_LENS,	  "...");
			SetDlgItemText(HWND_MAIN, IDC_PROCESS_AA_MOTION,	  "...");
			SetDlgItemText(HWND_MAIN, IDC_PROCESS_CHECK1,		  "...");
			SetDlgItemText(HWND_MAIN, IDC_PROCESS_UV,			  "...");
			SetDlgItemText(HWND_MAIN, IDC_PROCESS_CHECK2,		  "...");
			SM101_SERVO_OFF;
			MANUAL_ERROR = search_z_process();
			if(!MANUAL_ERROR){ 
				MANUAL_ERROR = aa_move_center(); 
				if(!MANUAL_ERROR){
					MANUAL_ERROR = focus_scan();
					if(!MANUAL_ERROR){
						MANUAL_ERROR = aa_move_center(); 
						if(!MANUAL_ERROR){
							MANUAL_ERROR = fs_check();						
							if(!MANUAL_ERROR){
								MANUAL_ERROR = aa_move_center(); 
								if(!MANUAL_ERROR){
									log("aa ok.");
									SetDlgItemText(HWND_MAIN,IDC_PROCESS_AA_MOTION,"OK");
								}								
							}						
						}						
					}									
				}				
			}			


			if (MANUAL_ERROR != 0x00){
				SetDlgItemText(HWND_MAIN,IDC_PROCESS_AA_MOTION,"NG");
				log("***AA Final Error***");
				EXECUTE_ERROR = TRUE;
			}
			EXECUTE_CMD = FALSE;
			GetLocalTime(&st2);
			if(st2.wSecond<st1.wSecond)
				mysbw.ct = st2.wSecond+60-st1.wSecond;
			else
				mysbw.ct = st2.wSecond-st1.wSecond;
			ai_win::set_dlg_item_int(HWND_MAIN,IDC_REPORT_CycleTime,mysbw.ct);
			//log("")
			log("===End AA===");
			break;
*/

		case 208:
			{
				if(HOME_FLAG == FALSE){
					set_status(APP_NOTHOME);
					break;
				}
				EXECUTE_ERROR = FALSE;
				SM103_TRIGGER = SM103_MOVE_TO_HOME_POSITION;
				SM101_TRIGGER = SM101_MOVE_TO_HOME_POSITION;
				SM102_TRIGGER = SM102_MOVE_TO_HOME_POSITION;
				SM104_TRIGGER = SM104_MOVE_TO_HOME_POSITION;
				SM105_TRIGGER = SM105_MOVE_TO_HOME_POSITION;
				
				Sleep(500);
				
				int timeout = 2000;
				while ((SM104_TRIGGER != 0 || SM103_TRIGGER != 0 || SM101_TRIGGER != 0 || SM102_TRIGGER != 0  ) && timeout-->0){
					Sleep(10);
				}

				if (timeout <= 0x00){
					log("***Move to home Error***");
					EXECUTE_ERROR = TRUE;
					if (SM104_TRIGGER != OFF)	   log("Line x(SM104) error:%d", SM104_TRIGGER_NOT_READY_ERR);
					else if (SM103_TRIGGER != OFF) log("Line y(SM103) error:%d", SM103_TRIGGER_NOT_READY_ERR);
					else if (SM101_TRIGGER != OFF) log("Line z(SM101) error:%d", SM101_TRIGGER_NOT_READY_ERR);
					else if (SM102_TRIGGER != OFF) log("Tilt x(SM102) error:%d", SM102_TRIGGER_NOT_READY_ERR);
				
				}
				EXECUTE_CMD = FALSE;
			}
			break;

	/*	case 209:
			if(HOME_FLAG == FALSE){
				set_status(APP_NOTHOME);
				break;
			}
			MANUAL_ERROR = SM203_move_to_home_position();
			log("manual moving.");*/
			

// 		case 210:
// 			if(HOME_FLAG == FALSE){
// 				set_status(APP_NOTHOME);
// 				break;
// 			}
// 			MANUAL_ERROR = SM202_move_to_load_position2();//Z 上升
// 			break;

		case 210: //move to line x home pos
			if ((err = CHART_Z_MOVE_TO_HOME_POS)!= 0x00)	log("Chart Z(SM101) move to home pos fail.");
			break;
			
		case 211:
			if ((err = LINE_T_MOVE_TO_HOME_POS)!= 0x00)	log("Line T(SM102) move to home pos fail.");
			break;

		case 212:
			if ((err = LINE_Y_MOVE_TO_HOME_POS)!= 0x00)	log("Line Y(SM103) move to home pos fail.");
			break;

		case 213:
			if ((err = LINE_X_MOVE_TO_HOME_POS)!= 0x00)	log("Tilt X(SM104) move to home pos fail.");
			break;

		case 214:
			if ((err = TABLE_U_MOVE_TO_HOME_POS)!= 0x00)	log("Table U(SM105) move to home pos fail.");
			break;

		case 221:
			EMERGENCY_STOP_FLAG = FALSE;
			HomeMachine_StopFlag = FALSE;
			EXECUTE_HOME = TRUE;
			timeout = 10000;
			while (EXECUTE_HOME == TRUE && timeout-->0){
				Sleep(10);
			}
			if (timeout<0){
				log("execute home time out.");
				EXECUTE_ERROR = TRUE;
			//	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_HOME_ALL);
			}
/*			else log("Current still homing.");*/
			break;

/*
		case 222:
			if ((err = SM101_move_to_loading_position()) != 0){
				log("***sm202 move loading Error***");
				EXECUTE_ERROR = TRUE;
			}
			EXECUTE_CMD = FALSE;
			break;*/

		case 223:
			{
				double move_x = 0;
				double move_y = 0;
				double cl_ct_x = mysbw.oc_x[0];
				double cl_ct_y = mysbw.oc_y[0];
				double img_center_x = AA_TARGET_CENTER_X/*2052*//* IMG_WIDTH/2*/;
				double img_center_y = AA_TARGET_CENTER_Y/*1531*//*IMG_HEIGHT/2*/;
				
				log("ct_x:%f, ct_y:%f, tar_x:%f, tar_y:%f", cl_ct_x, cl_ct_y, img_center_x, img_center_y);
//				log("motion_ang_x:%f, motion_ang_y:%f", AA_MOTION_X_ANGLE, AA_MOTION_Y_ANGLE);

				move_x = img_center_x-cl_ct_x;
				move_y = img_center_y-cl_ct_y;

				log("move pixel x=%f, y=%f", move_x, move_y);
				log("img_center_x=%f, y=%f ,cl_ct_x=%f,y=%f", img_center_x, img_center_y,cl_ct_x,cl_ct_y);

				ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_CALC_DIFF_CENTER_X, move_x, 4);
				ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_CALC_DIFF_CENTER_Y, move_y, 4);
				ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_REPORT_CENTER_X, move_x, 4);
				ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_REPORT_CENTER_Y, move_y, 4);

				AA_MOVE_X_MM = move_x*PIXEL_SIZE;
				AA_MOVE_Y_MM = move_y*PIXEL_SIZE;

				ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_MOVE_X_MM, move_x*PIXEL_SIZE, 4);
				ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_MOVE_Y_MM, move_y*PIXEL_SIZE, 4);
			}
			break;
		case 225:
			if(err=motor_product() != SUCCESS) return err;
			break;
		case 226:			
			if(err=motor_ng_product() != SUCCESS) return err;
			break;
		case 227:
			if(err=motor_home_product() != SUCCESS) return err;
			break;

		case 228: MANUAL_ERROR=SM105_turn90_cw(); break;
		case 229: MANUAL_ERROR=SM105_turn90_ccw();  break;

		case 230:
			if(err=motion_product() != SUCCESS) return err;
			break;
		case 231:
			if(err=motion_ng_product() != SUCCESS) return err;
			break;

		case 232:
			if (err=SM102_move_to_stack1_position() != SUCCESS) return err;
			break;
		case 233:
			if (err=SM102_move_to_stack2_position() != SUCCESS) return err;
			break;
		case 234:
			if (err=SM102_move_to_stack3_position() != SUCCESS) return err;
			break;
			
		case 238:ng_reset_led();break;
		case 239:pass_reset_led();break;


		case 240: MANUAL_ERROR=left_gripper_motion();	    break;
		case 241: MANUAL_ERROR=all_gripper_motion();	 
					log("all_gripper_motion err=%d",MANUAL_ERROR);break;
		case 242: MANUAL_ERROR=right_gripper_motion();	    break;

		case 250: MANUAL_ERROR=first_cylinder_cfg(1);		break;
		case 251: MANUAL_ERROR=first_cylinder_cfg(2);		break;
		case 252: MANUAL_ERROR=second_cylinder_cfg(1);	    break;
		case 253: MANUAL_ERROR=second_cylinder_cfg(2);	    break;
		case 254: MANUAL_ERROR=third_cylinder_cfg(1);	    break;
		case 255: MANUAL_ERROR=third_cylinder_cfg(2);	    break;
		case 256: MANUAL_ERROR=fourth_cylinder_cfg(1);		break;
		case 257: MANUAL_ERROR=fourth_cylinder_cfg(2);		break;
		case 258: MANUAL_ERROR=dark_box_cfg(1);			    break;
		case 259: MANUAL_ERROR=dark_box_cfg(2);			    break;
		case 260: MANUAL_ERROR=light_box_cfg(1);			break;
		case 261: MANUAL_ERROR=light_box_cfg(2);			break;
		case 262: MANUAL_ERROR=feed_cylinder_cfg(1);		break;
		case 263: MANUAL_ERROR=feed_cylinder_cfg(2);		break;
		case 264: MANUAL_ERROR=gripper_move_cfg(1);			break;
		case 265: MANUAL_ERROR=gripper_move_cfg(2);			break;
		case 266: MANUAL_ERROR=product_cylinder_cfg(1);		break;
		case 267: MANUAL_ERROR=product_cylinder_cfg(2);		break;
		case 268: MANUAL_ERROR=gripper_support_cfg(1);		break;
		case 269: MANUAL_ERROR=gripper_support_cfg(2);		break;
		case 270: MANUAL_ERROR=gripper_cylinder_cfg(1);		break;
		case 271: MANUAL_ERROR=gripper_cylinder_cfg(2);		break;
		case 272: MANUAL_ERROR=product_gripper_support_cfg(1);break;
		case 273: MANUAL_ERROR=product_gripper_support_cfg(2);break;
		case 274: MANUAL_ERROR=product_gripper_cfg(1);break;
		case 275: MANUAL_ERROR=product_gripper_cfg(2);break;
		case 276: MANUAL_ERROR=pass_tray_cylinder(1);break;
		case 277: MANUAL_ERROR=pass_tray_cylinder(2);break;			
		case 278: MANUAL_ERROR=stack_cylinder(1);break;
		case 279: MANUAL_ERROR=stack_cylinder(2);break;
		case 280: MANUAL_ERROR=tray_block_cylinder(1);break;
		case 281: MANUAL_ERROR=tray_block_cylinder(2);break;
		case 282: MANUAL_ERROR=ng_tray_cylinder(1);break;
		case 283: MANUAL_ERROR=ng_tray_cylinder(2);break;

		case 399://  IO monitor menu 
			IO_monitor_menu(number);
			break;

		default:
			if (number>399 && number<528)
			{
				IO_monitor_menu(number);
				break;
			}
			else break;

			MANUAL_MOVING = FALSE;
			log("recv: button_val =%d", number);
		}
	
	}


	return FALSE;
}