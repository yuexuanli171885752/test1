#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
#include "inf_motion_aa.h"

MOTION_SHM mshm;
extern SlantedBW_SM FS;//FOCUS_SCAN struct
extern SlantedBW mysbw;
extern SlantedBW cam_sbw;
extern char err_msg[200][MAX_PATH];
extern int NG_SIGNAL;

SPEED_PAR SpeedPar101_jog, SpeedPar101_pos, SpeedPar101_home;
SPEED_PAR SpeedPar102_jog, SpeedPar102_pos, SpeedPar102_home;
SPEED_PAR SpeedPar103_jog, SpeedPar103_pos, SpeedPar103_home;
SPEED_PAR SpeedPar104_jog, SpeedPar104_pos, SpeedPar104_home;
SPEED_PAR SpeedPar105_jog, SpeedPar105_pos, SpeedPar105_home;

//#define USE_DEMO_DATA

#define ROI_CNT				5

#define CT					0
#define UL					1
#define UR					2
#define LL					3
#define LR					4

#define USE_CURVE_FITTING

#define MULTI_FACTOR			4
#define RETURN_BACK_DELAY		1000

#define MAX_TILT_ANLGE	2
#define MAX_XY_OFFSET	3

float my_a = 0.0f;
float my_b = 0.0f;
float my_c = 0.0f;

double lens_z_position;//lens高度检查

double demo_sfr[5][30] = {
	{12,18,20,30,32,40,45,53,60,65,68,70,69,65,60,56,49,46,40,36,33,27,20,16,11,9,8,8,8,8},
	{10,20,22,30,35,43,50,55,58,60,59,55,50,46,39,36,30,26,23,17,10,9,8,7,7,8,8,8,8,7},
	{20,22,30,35,43,50,55,58,60,59,55,50,46,39,36,30,26,23,17,10,9,8,7,7,8,8,8,8,7,7},
	{7,8,8,8,9,9,10,20,22,30,35,43,50,55,58,60,59,55,50,46,39,36,30,26,23,17,10,9,8,7},
	{8,8,8,9,9,10,20,22,30,35,43,50,55,58,60,59,55,50,46,39,36,30,26,23,17,10,9,8,7,7}
};

void get_rotation(double alfa,double beta,double &theta,double &fai){
	double a=-tan(alfa)/sqrt((tan(alfa))*(tan(alfa))+(tan(beta))*(tan(beta))+1);
	double b=-tan(beta)/sqrt((tan(alfa))*(tan(alfa))+(tan(beta))*(tan(beta))+1);
	double c=c=1/sqrt((tan(alfa))*(tan(alfa))+(tan(beta))*(tan(beta))+1);
	double k;
	k=sqrt(1.0/2.0);
	double a1,b1,c1;
	a1=k*a+k*b;
	b1=-k*a+k*b;
	c1=c;
	theta=atan(a1/c1);
	fai=atan(b1/c1);
}

void get_best_fit_line(const double *x, const double *y, int n, double &slope, double &offset)
{
	double x_mean=0, y_mean=0, numerator=0, denominator=0;

	for(int i=0; i<n; i++)  x_mean+=x[i],y_mean+=y[i];

	x_mean /= (double)n;
	y_mean /= (double)n;

	for(int i=0; i<n; i++){
		numerator	+= y[i]*(x[i] - x_mean);
		denominator += x[i]*(x[i] - x_mean);
	}

	slope  = numerator/denominator;
	offset = y_mean - x_mean*slope;
}

int move_step_z(double step_val)
{
	int move_pulse = 0;
	if (step_val<0) move_pulse = (int)(step_val*SM102_PositionRatio-0.5);
	else			move_pulse = (int)(step_val*SM102_PositionRatio+0.5);

	int err = SM102_relative_short_move(move_pulse);
	if (err != 0x00){
		log("move sm203 err=%s", err_msg[err]);
		return err;
	}
	Sleep(AA_FS_DELAY);

	return err;
}

int aa_move_center(void)
{
	double move_x = 0;
	double move_y = 0;
	int pulse = 0;
	char buf[100] = {0};
	int cnt;
	move_x = AA_TARGET_CENTER_X-mysbw.oc_x[0];
	move_y = AA_TARGET_CENTER_Y-mysbw.oc_y[0];
	cnt = 0;
	while(1){
		if(cnt++>3)
			break;
		AA_MOVE_X_MM = move_x*PIXEL_SIZE;
		AA_MOVE_Y_MM = move_y*PIXEL_SIZE;
		log("move x=%fmm,y=%fmm",AA_MOVE_X_MM,AA_MOVE_Y_MM);
		if (fabs(AA_MOVE_X_MM)<MAX_XY_OFFSET){//spec平移保护~1mm
			if (AA_MOVE_X_MM<0)	pulse = (int)(AA_MOVE_X_MM*SM102_PositionRatio-0.5);
			else				pulse = (int)(AA_MOVE_X_MM*SM102_PositionRatio+0.5);
			SM102_relative_short_move(pulse);
		}
		else{
			log( "x=%f out of %dmm range.", AA_MOVE_X_MM, MAX_XY_OFFSET);
			return AA_MOVE_CENTER_OVER_RANGE;
		}

		if (fabs(AA_MOVE_Y_MM)<MAX_XY_OFFSET){
			if (AA_MOVE_Y_MM<0)	pulse = (int)(AA_MOVE_Y_MM*SM103_PositionRatio-0.5);
			else				pulse = (int)(AA_MOVE_Y_MM*SM103_PositionRatio+0.5);
			SM103_relative_short_move(pulse);
		}
		else{
			log( "y=%f out of %dmm range.", AA_MOVE_Y_MM, MAX_XY_OFFSET);
			return AA_MOVE_CENTER_OVER_RANGE;		
		}
		for(int i=0;i<ROI_CNT;i++){
			if(mysbw.find_target[i] == TRUE)
				continue;
			else{
				log("can find target No.%d.",i);
				return AA_FIND_TARGET_ERR;
			}
		}
		Sleep(50);
		move_x = AA_TARGET_CENTER_X-mysbw.oc_x[0];
		move_y = AA_TARGET_CENTER_Y-mysbw.oc_y[0];
		if(abs(move_x)<1 && move_y<1){
			log("move x, y to center OK.");
			break;
		}
	}	
	if(cnt>4)
		return AA_MOVE_CENTER_OVER_RANGE;
	else
		return 0;
}

/*
int aa_move_offset2(void)//偏心产品
{
	double move_x = 0;
	double move_y = 0;
	double cl_ct_x = mysbw.oc_x[0];
	double cl_ct_y = mysbw.oc_y[0];
	double img_center_x = 330;
	double img_center_y = 220;
	move_x = img_center_x-cl_ct_x;
	move_y = img_center_y-cl_ct_y;

	AA_MOVE_X_MM = move_x*PIXEL_SIZE;
	AA_MOVE_Y_MM = move_y*PIXEL_SIZE;
	log("move x=%fmm,y=%fmm",AA_MOVE_X_MM,AA_MOVE_Y_MM);

	int pulse = 0;
	char buf[100] = {0};
	const double spec = MAX_XY_OFFSET;
	if (fabs(AA_MOVE_X_MM)<spec){//spec平移保护~2mm
		if (AA_MOVE_X_MM<0)	pulse = (int)(AA_MOVE_X_MM*SM105_PositionRatio-0.5);
		else				pulse = (int)(AA_MOVE_X_MM*SM105_PositionRatio+0.5);
		SM105_relative_short_move(pulse);
	}
	else{
		// 		sprintf(buf, "x=%f out of %dmm range.", AA_MOVE_X_MM, MAX_XY_OFFSET);
		// 		MessageBox(HWND_MOTOR_MOTION, buf, "warning", 0);
		log( "x=%f out of %dmm range.", AA_MOVE_X_MM, MAX_XY_OFFSET);
		return 1;
	}

	if (fabs(AA_MOVE_Y_MM)<spec){
		if (AA_MOVE_Y_MM<0)	pulse = (int)(AA_MOVE_Y_MM*SM203_PositionRatio-0.5);
		else				pulse = (int)(AA_MOVE_Y_MM*SM203_PositionRatio+0.5);
		SM203_relative_short_move(pulse);
	}
	else{
		log( "x=%f out of %dmm range.", AA_MOVE_X_MM, MAX_XY_OFFSET);
		//lens_z_position=0;
		return 1;

	}
	for(int i=0;i<ROI_CNT;i++){
		if(mysbw.find_target[i] == TRUE)
			continue;
		else{
			log("can find target No.%d.",i);
			return 1;
		}
	}

	log("move x, y to center.");

	return 0;
}
*/

/*
int focus_scan(void){
	int timeout = 0;	
	int find_peak[ROI_CNT] = {0};
	int all_find_peak = FALSE;		
	int err = 0;
	int total_cnt = 0;
	int first_peak_index =AA_FS_MAX_CNT;	
	int returned_back[ROI_CNT] = {FALSE};	
	int step_pulse = (int)(AA_FS_STEP_UNIT*SM203_PositionRatio);	
	int cycle_num=0;
	int num=0;
	int move_pulse=0;
	double max_val[ROI_CNT] = {0.0};
	double start_val[5][ROI_CNT] = {0.0};
	double min_sfr_spec = 20.0;
	double find_peak_spec = AA_PEAK_DIFF_SPEC;
	double peak_min_spec  = AA_PEAK_MIN_GUIDE;//int

	FS.fs_index = 0;
	mysbw.sfr_val1[0][AA_SELECT_ROI]=0.0;

	while(1){
		if (EMERGENCY_STOP_FLAG == TRUE){
			err = 1;
			lens_z_position=0;
			log("Emergency stop!");
			return MACHINE_EMERGENCY_STOP_WARNING;
		}
		FS.find_target_num = mysbw.find_target[0]
							+mysbw.find_target[1]
							+mysbw.find_target[2]
							+mysbw.find_target[3]
							+mysbw.find_target[4];
		if(FS.find_target_num<5 && cycle_num<5){//最大5步 步数可编辑 gai..
			err=move_step_z(AA_FS_STEP_UNIT);
			Sleep(AA_FS_DELAY);
			cycle_num++;
		}
		else{
			err=move_step_z(-2*AA_FS_STEP_UNIT);
			Sleep(AA_FS_DELAY);
			break;
		}
	}
	if(FS.find_target_num!=5){///////////////////////////////////////////////////////////////////gai..
		log("roi recognizing fail");
		//SetDlgItemText(HWND_MAIN,IDC_RESULT_TILT_XY1,"NG");
		NG_SIGNAL=1;
		log("Tilt 1 NG=%d",NG_SIGNAL);
		lens_z_position=0;
		return 1;//err
	}
	else log("find all roi.");
	////////////////////识别确认
	float input_sfr[ROI_CNT][30] = {0};//gai..
	for (int z=0; z<AA_FS_MAX_CNT; z++){
		if (EMERGENCY_STOP_FLAG == TRUE){
			err = 1;
			log("Emergency stop!");
			lens_z_position=0;
			return 1;
		}
		FS.fs_z_pos[z] = z*AA_FS_STEP_UNIT;

		err = move_step_z(AA_FS_STEP_UNIT);
		Sleep(AA_FS_DELAY);
		if (err != 0x00){
			return 1;
			log("move step z fail.");
		}

		//////////////////////////////////////////////////////////////////////////
		all_find_peak = TRUE;
		lens_z_position=0;
		int roi_num=AA_SELECT_ROI;
		for (int m=0; m<ROI_CNT; m++){
			FS.oc_x[z][m] = mysbw.oc_x[m];
			FS.oc_y[z][m] = mysbw.oc_y[m];
		}
		FS.sfr_val[AA_SELECT_ROI][0][z] = mysbw.sfr_val1[0][AA_SELECT_ROI];//(mysbw.sfr_val1[0][0]+mysbw.sfr_val1[0][0])/2;//fs curve
		FS.sfr_val[AA_SELECT_ROI][1][z] = mysbw.sfr_val1[1][2];//(mysbw.sfr_val1[1][0]+mysbw.sfr_val1[1][0])/2;
		FS.sfr_val[AA_SELECT_ROI][2][z] = mysbw.sfr_val1[2][0];//(mysbw.sfr_val1[2][0]+mysbw.sfr_val1[2][0])/2;
		FS.sfr_val[AA_SELECT_ROI][3][z] = mysbw.sfr_val1[3][2];//(mysbw.sfr_val1[3][0]+mysbw.sfr_val1[3][0])/2;
		FS.sfr_val[AA_SELECT_ROI][4][z] = mysbw.sfr_val1[4][0];//(mysbw.sfr_val1[4][0]+mysbw.sfr_val1[4][0])/2;

		input_sfr[0][z] = (float)mysbw.sfr_val1[0][AA_SELECT_ROI];//(mysbw.sfr_val1[0][0]+mysbw.sfr_val1[0][0])/2;//curve fitting
		input_sfr[1][z] = (float)mysbw.sfr_val1[1][2];//(mysbw.sfr_val1[1][0]+mysbw.sfr_val1[1][0])/2;
		input_sfr[2][z] = (float)mysbw.sfr_val1[2][0];//(mysbw.sfr_val1[2][0]+mysbw.sfr_val1[2][0])/2;
		input_sfr[3][z] = (float)mysbw.sfr_val1[3][2];//(mysbw.sfr_val1[3][0]+mysbw.sfr_val1[3][0])/2;
		input_sfr[4][z] = (float)mysbw.sfr_val1[4][0];//(mysbw.sfr_val1[4][0]+mysbw.sfr_val1[4][0])/2;

			
		log("[%d],%.2f,%.2f, %.2f, %.2f, %.2f", z, 
			FS.sfr_val[AA_SELECT_ROI][0][z],
			FS.sfr_val[AA_SELECT_ROI][1][z],
			FS.sfr_val[AA_SELECT_ROI][2][z],
			FS.sfr_val[AA_SELECT_ROI][3][z],
			FS.sfr_val[AA_SELECT_ROI][4][z]);
		FS.fs_index = z;

		for (int m=0; m<ROI_CNT; m++){
			if (FS.sfr_val[AA_SELECT_ROI][m][z]>=max_val[m]){
				max_val[m] = FS.sfr_val[AA_SELECT_ROI][m][z];
				FS.fs_peak_index[m] = z;
			}

			if((max_val[m]-FS.sfr_val[AA_SELECT_ROI][m][z])>find_peak_spec && 
				max_val[m]>peak_min_spec && find_peak[m] == FALSE){
					find_peak[m] = TRUE;
					log("[%d]roi=%d find peak!", z, m);
					int index = FS.fs_peak_index[m];
					if (FS.sfr_val[AA_SELECT_ROI][m][index] == FS.sfr_val[AA_SELECT_ROI][m][index-1]){
						FS.fs_peak_index[m] -= 1;
					}
			}

			if (find_peak[m] == FALSE)	all_find_peak = FALSE;
		}//each roi

		if (z == -1) continue;
		total_cnt++;

		if (all_find_peak == TRUE ){
			log("find all peak.");
			break;
		}
	}

	if (err != 0x00||all_find_peak != TRUE){
		return 1;
	}


#ifdef USE_CURVE_FITTING				
	for (int i=0; i<5; i++){
		log("roi[%d] peak_index=%d, oc_x=%f, oc_y=%f", i, FS.fs_peak_index[i], 
			FS.oc_x[FS.fs_peak_index[i]][i],	
			FS.oc_y[FS.fs_peak_index[i]][i]);
	}

	float output_sfr[30] = {0};
	double fitted_curve_peak = 0;
	for (int i=0; i<ROI_CNT; i++){
		ai_math::get_curve_fitting(MULTI_FACTOR, input_sfr[i], total_cnt, output_sfr);
		fitted_curve_peak = 0;
		for (int j=0; j<total_cnt; j++){
			FS.sfr_val[AA_SELECT_ROI][i][j] = output_sfr[j];
			if (FS.sfr_val[AA_SELECT_ROI][i][j]>=fitted_curve_peak){
				fitted_curve_peak = FS.sfr_val[AA_SELECT_ROI][i][j];
				FS.fs_peak_index[i] = j;
			}
			}
	}
#endif

	double center_pos = (total_cnt-1-FS.fs_peak_index[0])*AA_FS_STEP_UNIT;
	double offset_pos = AA_FS_BACK_OFFSET*AA_FS_STEP_UNIT;

	if (AA_FS_STEP_UNIT<0)
		move_pulse = -(int)((center_pos+offset_pos)*SM203_PositionRatio-0.5);
	else 
		move_pulse = -(int)((center_pos+offset_pos)*SM203_PositionRatio+0.5);
	err = SM203_relative_short_move(move_pulse);
	if (err != 0x00){
		log("move sm104 err=%s", err_msg[err]);
		return 1;
	}
	log("back move :%fmm", center_pos+offset_pos);

	for (int i=0; i<5; i++){
		log("roi[%d] peak_index=%d, oc_x=%f, oc_y=%f", i, FS.fs_peak_index[i], 
			FS.oc_x[FS.fs_peak_index[i]][i],	
			FS.oc_y[FS.fs_peak_index[i]][i]);
		if (FS.fs_peak_index[i]<first_peak_index){
			first_peak_index = FS.fs_peak_index[i];
		}
	}
	calculate_tilt_x_y(AA_FS_STEP_UNIT);
	log("fisrt_peak:%d, center_peak:%d", first_peak_index, FS.fs_peak_index[0]);
	if (AA_FS_AUTO_CALIBRATION)//是否需要矫正
		err = calibrate_tilt_angle();
	else{
		return 1;
	}
	if (FS.tilt_angle[0] >AA_FS_TILT_LIMIT || FS.tilt_angle[1]>AA_FS_TILT_LIMIT){
		log("tilt angle over spec, return false.");
		return 1;	
	}
	if(!err)
		return 1;
	else{
		return 0;
		log("focus scan ok, no calibration.");
	}
}*/

/*
int fs_check(void){
	int timeout = 0;	
	int find_peak[ROI_CNT] = {0};
	int all_find_peak = FALSE;		
	int err = 0;
	int total_cnt = 0;
	int first_peak_index =AA_FS_MAX_CNT;	
	int check_trend[ROI_CNT] = {FALSE};
	int returned_back[ROI_CNT] = {FALSE};	
	int step_pulse = (int)(AA_FS_STEP_UNIT*SM203_PositionRatio);	
	int cycle_num=0;
	int num=0;
	int move_pulse=0;
	double max_val[ROI_CNT] = {0.0};
	double start_val[5][ROI_CNT] = {0.0};
	double min_sfr_spec = 20.0;
	double find_peak_spec = AA_PEAK_DIFF_SPEC;
	double peak_min_spec  = AA_PEAK_MIN_GUIDE;//int

	FS.fs_index = 0;
	mysbw.sfr_val1[0][AA_SELECT_ROI]=0.0;

	while(1){
		if (EMERGENCY_STOP_FLAG == TRUE){
			err = 1;
			lens_z_position=0;
			log("Emergency stop!");
			return 1;
		}
		FS.find_target_num = mysbw.find_target[0]
		+mysbw.find_target[1]
		+mysbw.find_target[2]
		+mysbw.find_target[3]
		+mysbw.find_target[4];
		if(FS.find_target_num<5 && cycle_num<5){//最大5步 步数可编辑 gai..
			err=move_step_z(AA_FS_STEP_UNIT);
			Sleep(AA_FS_DELAY);
			cycle_num++;

		}
		else{
			//err=move_step_z(-5*AA_FS_STEP_UNIT);
			Sleep(AA_FS_DELAY);
			break;
			//return 1;
		}
	}
	if(FS.find_target_num!=5){///////////////////////////////////////////////////////////////////gai..
		log("roi recognizing fail");
		NG_SIGNAL=1;
		log("NG=%d",NG_SIGNAL);
		lens_z_position=0;
		return 1;//err
	}
	else log("find all roi.");
	////////////////////识别确认
	float input_sfr[ROI_CNT][30] = {0};//gai..
	for (int z=0; z<AA_FS_MAX_CNT; z++){
		if (EMERGENCY_STOP_FLAG == TRUE){
			err = 1;
			log("Emergency stop!");
			lens_z_position=0;
			return 1;
		}
		FS.fs_z_pos[z] = z*AA_FS_STEP_UNIT;

		err = move_step_z(AA_FS_STEP_UNIT);
		Sleep(AA_FS_DELAY);
		if (err != 0x00){
			return 1;
			log("move step z fail.");
		}

		//////////////////////////////////////////////////////////////////////////
		all_find_peak = TRUE;
		lens_z_position=0;
		int roi_num=AA_SELECT_ROI;
		for (int m=0; m<ROI_CNT; m++){
			FS.oc_x[z][m] = mysbw.oc_x[m];
			FS.oc_y[z][m] = mysbw.oc_y[m];
		}
		FS.sfr_val[AA_SELECT_ROI][0][z] = mysbw.sfr_val1[0][AA_SELECT_ROI];
		FS.sfr_val[AA_SELECT_ROI][1][z] = mysbw.sfr_val1[1][2];
		FS.sfr_val[AA_SELECT_ROI][2][z] = mysbw.sfr_val1[2][0];
		FS.sfr_val[AA_SELECT_ROI][3][z] = mysbw.sfr_val1[3][2];
		FS.sfr_val[AA_SELECT_ROI][4][z] = mysbw.sfr_val1[4][0];

		input_sfr[0][z] = (float)mysbw.sfr_val1[0][AA_SELECT_ROI];
		input_sfr[1][z] = (float)mysbw.sfr_val1[1][2];
		input_sfr[2][z] = (float)mysbw.sfr_val1[2][0];
		input_sfr[3][z] = (float)mysbw.sfr_val1[3][2];
		input_sfr[4][z] = (float)mysbw.sfr_val1[4][0];

// 		FS.sfr_val[AA_SELECT_ROI][0][z] = (mysbw.sfr_val1[0][0]+mysbw.sfr_val1[0][2])/2;//fs curve
// 		FS.sfr_val[AA_SELECT_ROI][1][z] = (mysbw.sfr_val1[1][0]+mysbw.sfr_val1[1][2])/2;
// 		FS.sfr_val[AA_SELECT_ROI][2][z] = (mysbw.sfr_val1[2][0]+mysbw.sfr_val1[2][2])/2;
// 		FS.sfr_val[AA_SELECT_ROI][3][z] = (mysbw.sfr_val1[3][0]+mysbw.sfr_val1[3][2])/2;
// 		FS.sfr_val[AA_SELECT_ROI][4][z] = (mysbw.sfr_val1[4][0]+mysbw.sfr_val1[4][2])/2;
// 
// 		input_sfr[0][z] = (float)(mysbw.sfr_val1[0][0]+mysbw.sfr_val1[0][2])/2;//curve fitting
// 		input_sfr[1][z] = (float)(mysbw.sfr_val1[1][0]+mysbw.sfr_val1[1][2])/2;
// 		input_sfr[2][z] = (float)(mysbw.sfr_val1[2][0]+mysbw.sfr_val1[2][2])/2;
// 		input_sfr[3][z] = (float)(mysbw.sfr_val1[3][0]+mysbw.sfr_val1[3][2])/2;
// 		input_sfr[4][z] = (float)(mysbw.sfr_val1[4][0]+mysbw.sfr_val1[4][2])/2;

		FS.fs_index = z;
		log("[%d],%.2f,%.2f, %.2f, %.2f, %.2f", z, 
			FS.sfr_val[AA_SELECT_ROI][0][z],
			FS.sfr_val[AA_SELECT_ROI][1][z],
			FS.sfr_val[AA_SELECT_ROI][2][z],
			FS.sfr_val[AA_SELECT_ROI][3][z],
			FS.sfr_val[AA_SELECT_ROI][4][z]);
		

		for (int m=0; m<ROI_CNT; m++){
			if (FS.sfr_val[AA_SELECT_ROI][m][z]>=max_val[m]){
				max_val[m] = FS.sfr_val[AA_SELECT_ROI][m][z];
				FS.fs_peak_index[m] = z;
			}

			if((max_val[m]-FS.sfr_val[AA_SELECT_ROI][m][z])>find_peak_spec && 
				max_val[m]>peak_min_spec && find_peak[m] == FALSE){
					find_peak[m] = TRUE;
					log("[%d]roi=%d find peak!", z, m);
					int index = FS.fs_peak_index[m];
					if (FS.sfr_val[AA_SELECT_ROI][m][index] == FS.sfr_val[AA_SELECT_ROI][m][index-1]){
						FS.fs_peak_index[m] -= 1;
					}
			}

			if (find_peak[m] == FALSE)	all_find_peak = FALSE;
		}//each roi

		if (z == -1) continue;
		total_cnt++;

		if (all_find_peak == TRUE ){
			log("find all peak.");
			break;
		}
	}

	if (err != 0x00||all_find_peak != TRUE){
		return 1;
	}


#ifdef USE_CURVE_FITTING				
	for (int i=0; i<5; i++){
		log("roi[%d] peak_index=%d, oc_x=%f, oc_y=%f", i, FS.fs_peak_index[i], 
			FS.oc_x[FS.fs_peak_index[i]][i],	
			FS.oc_y[FS.fs_peak_index[i]][i]);
	}

	float output_sfr[30] = {0};
	double fitted_curve_peak = 0;
	for (int i=0; i<ROI_CNT; i++){
		ai_math::get_curve_fitting(MULTI_FACTOR, input_sfr[i], total_cnt, output_sfr);
		fitted_curve_peak = 0;
		for (int j=0; j<total_cnt; j++){
			FS.sfr_val[AA_SELECT_ROI][i][j] = output_sfr[j];
			if (FS.sfr_val[AA_SELECT_ROI][i][j]>=fitted_curve_peak){
				fitted_curve_peak = FS.sfr_val[AA_SELECT_ROI][i][j];
				FS.fs_peak_index[i] = j;
			}
		}
	}
#endif

	double center_pos = (total_cnt-1-FS.fs_peak_index[0])*AA_FS_STEP_UNIT;

	if (AA_FS_STEP_UNIT<0)
		move_pulse = -(int)(center_pos*SM203_PositionRatio-0.5);
	else 
		move_pulse = -(int)(center_pos*SM203_PositionRatio+0.5);
	err = SM203_relative_short_move(move_pulse);
	if (err != 0x00){
		log("move sm203 err=%s", err_msg[err]);
		return 1;
	}
	log("back move :%fmm", center_pos);

	for (int i=0; i<5; i++){
		log("roi[%d] peak_index=%d, oc_x=%f, oc_y=%f", i, FS.fs_peak_index[i], 
			FS.oc_x[FS.fs_peak_index[i]][i],	
			FS.oc_y[FS.fs_peak_index[i]][i]);
		if (FS.fs_peak_index[i]<first_peak_index){
			first_peak_index = FS.fs_peak_index[i];
		}
	}
	calculate_tilt_x_y(AA_FS_STEP_UNIT);
	log("fisrt_peak:%d, center_peak:%d", first_peak_index, FS.fs_peak_index[0]);

	if (mysbw.tilt_x >AA_FS_TILT_LIMIT || mysbw.tilt_y>AA_FS_TILT_LIMIT){
		log("tilt angle over spec, return false.");
		return 1;	
	}
	else{
		return 0;
		log("fs check down.");
	}
}*/
//=============================================================================================

int calculate_tilt_x_y(double step_unit)
{
	int result = TRUE;
	int ct_index = FS.fs_peak_index[CT];
	int ul_index = FS.fs_peak_index[UL];
	int ur_index = FS.fs_peak_index[UR];
	int ll_index = FS.fs_peak_index[LL];
	int lr_index = FS.fs_peak_index[LR];

	//

	double x[3] = {0};
	double y[3] = {0};
	double xx = 0;
	double yy = 0;
	double slope[2]  = {0};
	double offset[2] = {0};
	double psize = /*FS.pixel_size*/PIXEL_SIZE;
	double ul_ct = 0;
	double lr_ct = 0;
	double ll_ct = 0;
	double ur_ct = 0;

	xx    = pow((FS.oc_x[lr_index][LR]-FS.oc_x[ct_index][CT]), (double)2);
	yy    = pow((FS.oc_y[lr_index][LR]-FS.oc_y[ct_index][CT]), (double)2);
	ul_ct = sqrt(xx+yy);

	xx    = pow((FS.oc_x[ul_index][UL]-FS.oc_x[ct_index][CT]), (double)2);
	yy    = pow((FS.oc_y[ul_index][UL]-FS.oc_y[ct_index][CT]), (double)2);
	lr_ct = sqrt(xx+yy);

	xx    = pow((FS.oc_x[ul_index][LL]-FS.oc_x[ct_index][CT]), (double)2);
	yy    = pow((FS.oc_y[ul_index][LL]-FS.oc_y[ct_index][CT]), (double)2);
	ll_ct = sqrt(xx+yy);
	xx    = pow((FS.oc_x[ul_index][UR]-FS.oc_x[ct_index][CT]), (double)2);
	yy    = pow((FS.oc_y[ul_index][UR]-FS.oc_y[ct_index][CT]), (double)2);
	ur_ct = sqrt(xx+yy);

	log("ul_ct=%.4f, lr_ct=%.4f", ul_ct, lr_ct);
	log("ll_ct=%.4f, ul_ct=%.4f", ll_ct, ul_ct);
	//////////////////////////////////////////////////////////////////////////

	x[1] = ul_ct*psize/**sqrt((double)2)*/;
	y[1] = (FS.fs_peak_index[CT]-FS.fs_peak_index[UL])*step_unit;
	log("dia-1:x[1]=%.4f, y[1]=%.4f", x[1], y[1]);
	x[2] = x[1] +lr_ct*psize/**sqrt((double)2)*/;
	y[2] = (FS.fs_peak_index[LR]-FS.fs_peak_index[UL])*step_unit;
	log("dia-1:x[2]=%.4f, y[2]=%.4f", x[2], y[2]);
	get_best_fit_line(x, y, 3, slope[0], offset[0]);

	x[1] = ll_ct*psize/**sqrt((double)2)*/;
	y[1] = (FS.fs_peak_index[CT]-FS.fs_peak_index[UR])*step_unit;
	log("dia-2:x[1]=%.4f, y[1]=%.4f", x[1], y[1]);
	x[2] = x[1] +ur_ct*psize/**sqrt((double)2)*/;
	y[2] = (FS.fs_peak_index[LL]-FS.fs_peak_index[UR])*step_unit;
	log("dia-2:x[2]=%.4f, y[2]=%.4f", x[2], y[2]);
	get_best_fit_line(x, y, 3, slope[1], offset[1]);

	FS.tilt_angle[0] = atan(slope[0])*180.0/PI;
	log("dia-1:angle=%.4f, slope=%.4f", FS.tilt_angle[0], slope[0]);
	ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_TILT_X_ANGLE, FS.tilt_angle[0], 4);
	FS.tilt_angle[1] = atan(slope[1])*180/PI;
	log("dia-1:angle=%.4f, slope=%.4f", FS.tilt_angle[1], slope[1]);
 	ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_TILT_Y_ANGLE, FS.tilt_angle[1], 4);

	double new_angle_x = 0;//45 trans
	double new_angle_y = 0;
	if(CHOOSE_XY)
		get_rotation(atan(slope[0]), atan(slope[1]), new_angle_x, new_angle_y);
	else
		get_rotation(atan(slope[1]), atan(slope[0]), new_angle_x, new_angle_y);

	log("my_a:%.2f, my_b:%.2f, my_c:%.2f", my_a, my_b, my_c);
	if(SYMBOL1){
		if(SYMBOL2){
			FS.tilt_angle[0] = new_angle_x*180.0/PI;
			FS.tilt_angle[1] = new_angle_y*180.0/PI;
		}
		else{
			FS.tilt_angle[0] = new_angle_x*180.0/PI;
			FS.tilt_angle[1] = -new_angle_y*180.0/PI;
		}
	}
	else{
		if(SYMBOL2){
			FS.tilt_angle[0] = -new_angle_x*180.0/PI;
			FS.tilt_angle[1] = new_angle_y*180.0/PI;
		}
		else{
			FS.tilt_angle[0] = -new_angle_x*180.0/PI;
			FS.tilt_angle[1] = -new_angle_y*180.0/PI;
		}
	}
	mysbw.tilt_x = FS.tilt_angle[0];
	mysbw.tilt_y = FS.tilt_angle[1];
	log("new x tilt:%.4f, y tilt:%.4f", FS.tilt_angle[0], FS.tilt_angle[1]);
	if(abs(mysbw.tilt_x)>mysbw.tilt_limit||abs(mysbw.tilt_y)>mysbw.tilt_limit){//gai..
		mysbw.test_result = FALSE;
	}
	return result;
}

int calibrate_tilt_angle()
{
	int result = TRUE;
	const double spec = MAX_TILT_ANLGE;
	int err = 0;
	if (fabs(FS.tilt_angle[0])<spec){
		err = SM102_relative_short_move(-(int)(FS.tilt_angle[0]*SM102_PositionRatio));
		log("sm102 move angle:%f",FS.tilt_angle[0]);
		if (err != 0x00){
			log("move sm102 err=%s", err_msg[err]);
			return FALSE;
		}
		log("Tilt X angle:%.3f calibrated", FS.tilt_angle[0]);
		SetDlgItemText(HWND_MOTOR_MOTION, IDC_TILT_X_ANGLE, "0.000");
		SetDlgItemText(HWND_MAIN, IDC_REPORT_X_ANGLE, "0.000");
	}
	else log("move tilt x over spec:%f", spec);


	if (fabs(FS.tilt_angle[1])<spec){
		err = SM101_relative_short_move((int)(FS.tilt_angle[1]*SM101_PositionRatio));
		if (err != 0x00){
			log("move sm101 err=%s", err_msg[err]);
			return FALSE;
		}
		log("Tilt Y angle:%.3f calibrated", FS.tilt_angle[1]);
		SetDlgItemText(HWND_MOTOR_MOTION, IDC_TILT_Y_ANGLE, "0.000");
		SetDlgItemText(HWND_MAIN, IDC_REPORT_Y_ANGLE, "0.000");
	}
	else log("move tilt y over spec:%f", spec);

	return result;
}

int search_z_process(void){
	int err		  = 0;
	int cycle_num = 0;

	while(1){	
		if (EMERGENCY_STOP_FLAG == TRUE){
			err = 1;
			log("Emergency stop!");
			lens_z_position=0;
			return 1;
		}
		log("move step z start.");
		if((mysbw.find_target[0] == FALSE ||mysbw.sfr_val1[0][AA_SELECT_ROI]<(0.3*mysbw.spec1[0])) && cycle_num<15){//最大15步

			err=move_step_z(5*AA_FS_STEP_UNIT);
			log("step_z = %f",AA_FS_STEP_UNIT);
			Sleep(AA_FS_DELAY);
			cycle_num++;
			log("search z position. %d %.2f,%.2f",cycle_num,mysbw.sfr_val1[0][AA_SELECT_ROI],(0.3*mysbw.spec1[0]));
		}
		else{//move center
			if(mysbw.find_target[0]!=TRUE){
				log("Move center, center recognizing fail");
			//	SetDlgItemText(HWND_MAIN,IDC_RESULT_MOVE_CENTER,"NG");
				NG_SIGNAL=1;
				log("NG=%d",NG_SIGNAL);
				lens_z_position=0;
				return 1;//全部跳出
			}
			else{
				log("break cycle.search z.");
				Sleep(500);
				break;
			}
		}
	}

	if(mysbw.sfr_val1[0][AA_SELECT_ROI]>(0.6*mysbw.spec1[0]))//防止越过peak点
		err=move_step_z(-3*AA_FS_STEP_UNIT);
	return 0;
}

/*
int aa_move_offset(void){
	int err = 0;
	err = SM202_relative_short_move((int)(AA_FINAL_X_OFFSET*SM202_PositionRatio+0.5));
	log("move x offset:%d", MANUAL_ERROR);
	err = SM201_relative_short_move((int)(AA_FINAL_Y_OFFSET*SM201_PositionRatio+0.5));
	log("move y offset:%d", MANUAL_ERROR);
	err = SM203_relative_short_move((int)(AA_FINAL_Z_OFFSET*SM203_PositionRatio+0.5));
	log("move z offset:%d", MANUAL_ERROR);
	if(err){
		log("move offset err.");
		return err;
	}
	else
		return 0;
}*/


double calculate_theta_c_angle(double x, double y, double tx, double ty)
{
	double ang = 0;
	double dx = tx-x;
	double dy = ty-y;

	if (dy==0 && dx>0)  ang = 90;
	if (dy==0 && dx<0)  ang = 270;
	if (dx==0 && dy>0)  ang = 0;
	if (dx==0 && dy<0)  ang = 180;
	if (dx==0 && dy==0) ang = 0;

	ang = fabs(atan(dx/dy)*180/PI);

	if (dx>0 && dy>0)		ang = ang;
	else if (dx>0 && dy<0)	ang = 180-ang;
	else if (dx<0 && dy<0)	ang = 180+ang;
	else if (dx<0 && dy>0)	ang = 360-ang;

	return ang;
}
