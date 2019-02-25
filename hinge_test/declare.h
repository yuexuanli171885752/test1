//=========================================================================================  
//	Project:		AI-200t-NEW
//	Program:		Declare.h
//	date:			2015-2-19  
//===========================================================================================
#ifndef EXTERN
#define EXTERN extern
#endif

//=== declare goble value ======================
EXTERN int  number;
EXTERN char buffer[100];
EXTERN int	errbuf; 
EXTERN int	initial_step;
EXTERN LARGE_INTEGER	Frequency;
EXTERN unsigned short	wTotalBoards, wInitialCode, wRetVal, PEXP32C32_BoardNo;

//=== Motion Card related Input function ====================================================
EXTERN int mn200_get_status(BYTE bLineNo, BYTE bDevNo);
EXTERN short mn_servo_motor_servo_on(BYTE bLineNo, BYTE bDevNo);
EXTERN short mn_servo_motor_servo_off(BYTE bLineNo, BYTE bDevNo);

//=== I/O Card related Input function =======================================================
// EXTERN BYTE PISODIO_InputByte(WORD wBoardNo,WORD wPortNo);
// EXTERN void PISODIO_OutputByte(WORD wBoardNo, WORD wPortNo, DWORD dwDOVal);
EXTERN void MN200_RemoteOutputWord(BYTE LineNo, BYTE DeviceNo, BYTE WordNo, WORD wDOVal);
EXTERN WORD MN200_RemoteInputWord(BYTE LineNo, BYTE DeviceNo, BYTE WordNo);

//=======================================================================
// subroutine declaration
//=======================================================================
//*** SM101 AC servo motor
EXTERN int	SM101_home_search(void);
EXTERN int	SM101_motor_stop(void);
EXTERN int	SM101_encoder_reset(void);
EXTERN int	SM101_jog_cw(void);
EXTERN int	SM101_jog_ccw(void);
EXTERN int	SM101_relative_movement(int target, int speed, int acceleration);
EXTERN int	SM101_move_to_home_position(void);
EXTERN int  SM101_move_to_lens_loading_position(void);
EXTERN int	SM101_relative_short_move(int MoveDistance);
EXTERN int  SM101_move_to_load_position1(void);
EXTERN int	SM101_move_to_load_position2(void);
EXTERN int	SM101_move_to_load_position3(void);
EXTERN int	SM101_move_to_load_position4(void);
EXTERN int	SM101_move_to_load_position5(void);
EXTERN int  SM101_move_to_load_position6(void);
EXTERN void read_SM101_position(void);
EXTERN int  SM101_move_to_load_position7(void);
EXTERN int  SM101_initialization(void);

//*** SM102 AC servo motor
EXTERN int	SM102_home_search(void);
EXTERN int	SM102_motor_stop(void);
EXTERN int	SM102_encoder_reset(void);
EXTERN int	SM102_jog_cw(void);
EXTERN int	SM102_jog_ccw(void);
EXTERN int	SM102_relative_movement(int target, int speed, int acceleration);
EXTERN int	SM102_move_to_home_position(void);
EXTERN int  SM102_move_to_lens_loading_position(void);
EXTERN int	SM102_relative_short_move(int MoveDistance);
EXTERN int	SM102_move_to_stack1_position(void);
EXTERN int	SM102_move_to_stack2_position(void);
EXTERN int	SM102_move_to_stack3_position(void);
EXTERN int  SM102_move_to_load_position1(void);
EXTERN int	SM102_move_to_load_position2(void);
EXTERN int	SM102_move_to_load_position3(void);
EXTERN int	SM102_move_to_load_position4(void);
EXTERN int	SM102_move_to_load_position5(void);
EXTERN int  SM102_move_to_load_position6(void);
EXTERN void read_SM102_position(void);
EXTERN int  SM102_move_to_load_position7(void);
EXTERN int  SM102_initialization(void);

//*** SM103 AC servo motor
EXTERN int	SM103_home_search(void);
EXTERN int	SM103_motor_stop(void);
EXTERN int	SM103_encoder_reset(void);
EXTERN int	SM103_jog_cw(void);
EXTERN int	SM103_jog_ccw(void);
EXTERN int	SM103_relative_movement(int target, int speed, int acceleration);
EXTERN int	SM103_move_to_home_position(void);
EXTERN int  SM103_move_to_pass_position(void);
EXTERN int  SM103_move_to_ng_position(void);
EXTERN int  SM103_move_product_home_position(void);
EXTERN int  SM103_move_to_lens_loading_position(void);
EXTERN int	SM103_relative_short_move(int MoveDistance);
EXTERN int  SM103_move_to_load_position1(void);
EXTERN int	SM103_move_to_load_position2(void);
EXTERN int	SM103_move_to_load_position3(void);
EXTERN int	SM103_move_to_load_position4(void);
EXTERN int	SM103_move_to_load_position5(void);
EXTERN int  SM103_move_to_load_position6(void);
EXTERN void read_SM103_position(void);
EXTERN int  SM103_move_to_load_position7(void);
EXTERN int  SM103_initialization(void);

//*** SM104 AC servo motor
EXTERN int	SM104_home_search(void);
EXTERN int	SM104_motor_stop(void);
EXTERN int	SM104_encoder_reset(void);
EXTERN int	SM104_jog_cw(void);
EXTERN int	SM104_jog_ccw(void);
EXTERN int	SM104_relative_movement(int target, int speed, int acceleration);
EXTERN int	SM104_move_to_home_position(void);
EXTERN int  SM104_move_to_pass_position(void);
EXTERN int  SM104_move_to_ng_position(void);
EXTERN int  SM104_move_product_home_position(void);
EXTERN int  SM104_move_to_lens_loading_position(void);
EXTERN int	SM104_relative_short_move(int MoveDistance);
EXTERN int  SM104_move_to_load_position1(void);
EXTERN int	SM104_move_to_load_position2(void);
EXTERN int	SM104_move_to_load_position3(void);
EXTERN int	SM104_move_to_load_position4(void);
EXTERN int	SM104_move_to_load_position5(void);
EXTERN int  SM104_move_to_load_position6(void);
EXTERN void read_SM104_position(void);
EXTERN int  SM104_move_to_load_position7(void);
EXTERN int  SM104_initialization(void);

//*** SM201 AC servo motor
EXTERN int	SM105_home_search(void);
EXTERN int	SM105_motor_stop(void);
EXTERN int	SM105_encoder_reset(void);
EXTERN int	SM105_jog_cw(void);
EXTERN int	SM105_jog_ccw(void);
EXTERN int  SM105_turn90_cw(void);
EXTERN int  SM105_turn90_ccw(void);
EXTERN int	SM105_relative_movement(int target, int speed, int acceleration);
EXTERN int	SM105_move_to_home_position(void);
EXTERN int  SM105_move_to_loading_position(void);
EXTERN int	SM105_relative_short_move(int MoveDistance);
EXTERN int	SM105_move_to_load_position1(void);
EXTERN int  SM105_move_to_load_position2(void);
EXTERN int  SM105_move_to_load_position3(void);
EXTERN int  SM105_move_to_load_position4(void);
EXTERN int  SM105_move_to_load_position5(void);
EXTERN int  SM105_move_to_load_position6(void);
EXTERN int	SM105_initialization(void);

//***** read motor limit sensor status **********************
EXTERN int  SM101_LMT_POSITIVE(void);
EXTERN int  SM101_LMT_NEGATIVE(void);

EXTERN int  SM102_LMT_POSITIVE(void);
EXTERN int  SM102_LMT_NEGATIVE(void);

EXTERN int  SM103_LMT_POSITIVE(void);
EXTERN int  SM103_LMT_NEGATIVE(void);

EXTERN int  SM104_LMT_POSITIVE(void);
EXTERN int  SM104_LMT_NEGATIVE(void);

EXTERN int  SM105_LMT_POSITIVE(void);
EXTERN int  SM105_LMT_NEGATIVE(void);

//***** general subroutine **********************
EXTERN int  mc_init (void);
EXTERN void initial_setting (void); 
EXTERN int  initial_sequence (void);
EXTERN void lamp_display(int status);
EXTERN int	Motion_Motor_Init(void);
EXTERN void DelayMs(int DelayTimeMs);

//==== subroutine declaration for IO function ==========
EXTERN void say_error(int err_no);
EXTERN int	safety_check(void);
EXTERN int	emergency_stop_check(void);
EXTERN int	safety_sensor_check(void);
EXTERN int	manual_motion_server(int MotionCommand);
EXTERN int	Device_memory_and_test_result_transfer_to_next_para(void);

//--------------------------------------------------------------
EXTERN int	all_sites_pogo_socket_move_down(void);
EXTERN int	all_sites_pogo_socket_at_down_position_check(void);
EXTERN int	index_table_at_good_position_for_pogo_socket_check(void);
//--------------------------------------------------------------
EXTERN int	load_unload_site_pogo_socket_move_up(void);
EXTERN int	load_unload_site_pogo_socket_move_down(void);
EXTERN int	load_unload_site_manual_test_one_cycle(void);
EXTERN int	load_unload_site_manual_test_stop(void);

//--------------------------------------------------------------
EXTERN int	focus_adjust_site_pogo_socket_move_up(void);
EXTERN int	focus_adjust_site_pogo_socket_move_down(void);
EXTERN int  focus_adjust_site_cylinder_move_left(void);
EXTERN int  focus_adjust_site_cylinder_move_right(void);
EXTERN int	focus_adjust_site_manual_test_one_cycle(void);
EXTERN int	focus_adjust_site_manual_test_stop(void);
EXTERN int	focus_adjust_site_unload_all_unit(void);
EXTERN int	focus_adjust_unit_motion_server(int MotionCommand);

//--------------------------------------------------------------
EXTERN int	af_test_site_pogo_socket_move_up(void);
EXTERN int	af_test_site_pogo_socket_move_down(void);
EXTERN int	af_test_site_cylinder_move_left(void);
EXTERN int	af_test_site_cylinder_move_right(void);
EXTERN int	image_test_site_manual_test_one_cycle(void);
EXTERN int	image_test_site_manual_test_stop(void);
extern int	af_test_unit_motion_server(int MotionCommand);

//--------------------------------------------------------------
EXTERN int	image_site_pogo_socket_move_up(void);
EXTERN int	image_site_pogo_socket_move_down(void);
EXTERN int  image_site_cylinder_move_left(void);
EXTERN int  image_site_cylinder_move_right(void);
EXTERN int  image_site_dark_move_up(void);
EXTERN int  image_site_dark_move_down(void);
EXTERN int	dispense_site_manual_test_one_cycle(void);
EXTERN int	dispense_site_manual_test_stop(void);
EXTERN int	image_test_unit_motion_server(int MotionCommand);
EXTERN int	dispense_unit_dispensing_procedure(void);
EXTERN int	dispenser_unit_single_point_dispensing(int index);
EXTERN int	dispense_unit_UV_curing_procedure(void);
EXTERN int	Dispense_Z_load_cell_limit_search(void);
EXTERN int	Dispenser_needle_cleaning_cycle(void);

//==== subroutine declaration for synchro-move subroutines ==========
EXTERN int focus_two_axes_xy_synchro_move_to_home_position(void);
EXTERN int focus_two_axes_xy_synchro_move_to_vision_position(void);
EXTERN int focus_two_axes_xy_synchro_offset_move_from_vision_position(void);
EXTERN int focus_two_axes_xy_synchro_move_to_laser_position(void);
EXTERN int focus_two_axes_xy_synchro_move_to_cone_position(void);

EXTERN int focus_three_axes_xyr_synchro_move_to_home_position(void);
EXTERN int focus_three_axes_xyr_synchro_move_to_vision_position(void);
EXTERN int focus_three_axes_xyr_synchro_move_to_cone_position(void);
EXTERN int focus_three_axes_xyr_synchro_offset_move_from_vision_position(void);
EXTERN int focus_three_axes_xyr_synchro_offset_move_from_cone_position(void);

EXTERN int dispense_xy_synchro_move_to_home_position(void);
EXTERN int dispense_xy_synchro_move_to_vision_position(void);
EXTERN int dispense_xy_synchro_move_vision_center(void);
EXTERN int dispense_xy_synchro_move_to_laser_position(void);
EXTERN int dispense_xy_synchro_move_to_needle_position(void);
EXTERN int dispense_xy_synchro_move_to_cure_center_position(void);
EXTERN int dispense_xy_synchro_move_to_cure_index_position(int index);
EXTERN int dispense_xy_synchro_move_to_touch_position(void);
EXTERN int dispense_xy_synchro_move_to_glass_position(void);
EXTERN int dispense_xy_synchro_move_to_clean_position(void);
EXTERN int dispense_xy_synchro_move_to_dispense_index_position(int index);
EXTERN int dispense_xy_synchro_move_to_vision_on_glass_position(void);
EXTERN int dispense_xy_synchro_move_to_laser_on_glass_position(void);
EXTERN int dispense_xy_synchro_offset_move_from_vision_position(void);

//==== subroutine declaration for Focus Teach Menu ===============
EXTERN void FocusSetupMenu(void);

//==== subroutine declaration for Dispense Teach Menu ===============
EXTERN void DispenseSetupMenu(void);

//==== subroutine declaration for Motors Teach Menu ===============
EXTERN void MotorsSetupMenu(void);
EXTERN void SM101_focus_x_motor_setup_menu(int ButtonNumber);
EXTERN void SM102_focus_y_motor_setup_menu(int ButtonNumber);
EXTERN void SM103_focus_z_motor_setup_menu(int ButtonNumber);
EXTERN void SM104_focus_r_motor_setup_menu(int ButtonNumber);
EXTERN void SM105_focus_r_motor_setup_menu(int ButtonNumber);

//==== subroutine declaration for IO monitor menu ================
EXTERN void IO_monitor_menu(int Number);
EXTERN void IO_port0_monitor(int Number);
EXTERN void IO_port1_monitor(int Number);
EXTERN void IO_port2_monitor(int Number);
EXTERN void IO_port3_monitor(int Number);

EXTERN int  under_running_machine_suddenly_stop_procedure(void);

//==== main ==============================================
extern HANDLE	Motors1_hMutex;
extern HANDLE	Motors2_hMutex;
extern HANDLE	Motors3_hMutex;
extern HANDLE	Err_hMutex;

//==== thread definition for TS-task =====================
DWORD Tscreen		(LPVOID lpvThreadParm);
DWORD Iokernel		(LPVOID lpvThreadParm);
DWORD Readcntr		(LPVOID lpvThreadParm);
DWORD Lamp			(LPVOID lpvThreadParm);
DWORD ErrProcs		(LPVOID lpvThreadParm);
DWORD BuzzerProcess	(LPVOID lpvThreadParm);
DWORD Monitor		(LPVOID lpvThreadParm);

// *** thread definition for Motions-task
DWORD TriggerSM101	(LPVOID lpvThreadParm);
DWORD TriggerSM102	(LPVOID lpvThreadParm);
DWORD TriggerSM103	(LPVOID lpvThreadParm);
DWORD TriggerSM104	(LPVOID lpvThreadParm);
DWORD TriggerSM105	(LPVOID lpvThreadParm);


DWORD FocusCheckTask		(LPVOID lpvThreadParm);
DWORD AFTestTask			(LPVOID lpvThreadParm);
DWORD ImageTestTask			(LPVOID lpvThreadParm);
DWORD CentralRotateTask		(LPVOID lpvThreadParm);
DWORD ManualCommandServer	(LPVOID lpvThreadParm);


//DWORD Trans_carrier(LPVOID lpvThreadParm);
//DWORD product_bin(LPVOID lpvThreadParm);
DWORD WINAPI power_supply_link_thread(LPVOID lpvThreadParm);
