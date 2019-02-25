#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
#include "test_white.h"
White_V white_v;
using namespace s_test;

PARTICLE_POS particle_dark[MAX_PARTICLE_CNT];
PARTICLE_POS particle_white[MAX_PARTICLE_CNT];
MARK_RED particle_red[MAX_MARK_CNT];
MARK_RED particle_red1[MAX_MARK_CNT];

int particle_dark_cnt;
int particle_white_cnt;
int particle_red_cnt=1;
int blemish_point[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];
int draw_particle = FALSE;
int draw_blemish  =FALSE;
#define IMG_RAW								0
#define IMG_BMP								1
#define IMG_JPG								2
#define IMG_PNG								3
#define IMG_TIFF							4
#define IMG_GIF								5
#define IMG_MIPIRAW							6
int parse_normal_img(const char *img_path, BYTE *img_buf, int buf_size, int img_format)
{
	int result = TRUE;
	FREE_IMAGE_FORMAT fif;
	char img_str[20] = {0};
	int img_size = 0;
	switch(img_format)
	{
	case IMG_JPG:	fif = FIF_JPEG; sprintf(img_str, "Jpeg");	  break;
	case IMG_PNG:	fif = FIF_PNG;  sprintf(img_str, "Png");	  break;
	case IMG_TIFF: fif = FIF_TIFF; sprintf(img_str, "Tif/Tiff"); break;
	case IMG_GIF:  fif = FIF_GIF;  sprintf(img_str, "Gif");	  break;
	default: result = FALSE; break;
	}
	if (result == TRUE){
		FreeImage_Initialise();
		FIBITMAP *fimg = FreeImage_Load(fif, img_path);
		BYTE * img = FreeImage_GetBits(fimg);
		IMG_WIDTH  = FreeImage_GetWidth(fimg);
		IMG_HEIGHT = FreeImage_GetHeight(fimg);
		IMG_DEPTH  = FreeImage_GetBPP(fimg)/8;
		img_size = (IMG_WIDTH*IMG_HEIGHT*IMG_DEPTH);

		if (fimg != NULL && IMG_WIDTH>0 && IMG_HEIGHT>0 && buf_size>img_size){
			log("Read %s:%s", img_str, img_path);
			log("%s W:%d, H:%d, %dbit", img_str, IMG_WIDTH, IMG_HEIGHT, IMG_DEPTH*8);
			memcpy(img_buf, img, img_size);
		}
		else{
			log("Read %s Fail.", img_str);
		}
		FreeImage_Unload(fimg);
		FreeImage_DeInitialise();
	}

	return result;
}

int fade_zone(BYTE *bmp_bin,BYTE *bmp_y,BYTE *bmp_y_copy,int w,int h)
{
	for (int i=0; i<w*h; i++){
		bmp_bin[i] = ((bmp_y[i]>120)?255:0);
	}

	sprintf(config_path,"%s\\image\\bin.bmp",dir);
	ai_img::save_bmp8_image(config_path,bmp_bin,w,h);
	log("save image:%s", config_path);

	for (int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			bmp_y_copy[i*w+j]=bmp_y[i*w+j];
		}
	}
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			if (bmp_bin[i*w+j]==255)
			{
				int a=i;
				int s=j;
				for (int b=0;b<=s;b++)
				{
					bmp_y_copy[a*w+b]=150;
				}
				break;
			}
		}
	}

	for(int i=0;i<h;i++)
	{
		for(int j=w-1;j>=0;j--)
		{
			if (bmp_bin[i*w+j]==255)
			{
				int a=i;
				int s=j;
				for (int b=s;b<w;b++)
				{
					bmp_y_copy[a*w+b]=150;
				}
				break;
			}
		}
	}
	sprintf(config_path,"%s\\image\\s.bmp",dir);
	ai_img::save_bmp8_image(config_path,bmp_y,w,h);
	log("save image:%s", config_path);

	sprintf(config_path,"%s\\image\\ss.bmp",dir);
	ai_img::save_bmp8_image(config_path,bmp_y_copy,w,h);
	log("save image:%s", config_path);
	return 0;
}

int particle_proc(BYTE *bmp_y, int img_w, int img_h, float limit_low, float limit_up, 
	float &result_low, float &result_up, int &dark_cnt, int &white_cnt, 
	PARTICLE_POS *particle_dark, PARTICLE_POS *particle_white){
		int index = 0;
		float sum_val = 0;
		float avg_val = 0.000f;
		float target_percent = 0.000f;

		float max_val = 0.000;
		float min_val = 1.000;

		int over_limit = FALSE;

		for (int j=0; j<img_h-STEP_H; j += STEP_H){
			for (int i=0; i<img_w-STEP_W; i += STEP_W){
				index = j*img_w+i;
				sum_val = 0;
				for (int n=0; n<STEP_H; n++){
					for (int m=0; m<STEP_W; m++){
						sum_val += bmp_y[index + n*img_w+m];
					}
				}
				avg_val = (float)sum_val/(STEP_W*STEP_H);
				for (int n=0; n<STEP_H; n++){
					for (int m=0; m<STEP_W; m++){
						target_percent = bmp_y[index + n*img_w+m]/avg_val;
						if (target_percent<min_val)	min_val = target_percent;
						if (target_percent>max_val)	max_val = target_percent;
						if (over_limit == FALSE){
							if (target_percent<limit_low){
								particle_dark[dark_cnt].val = bmp_y[index + n*img_w+m];
								particle_dark[dark_cnt].x = i+m; 
								particle_dark[dark_cnt].y = img_h-1-(j+n);
								dark_cnt++;	
								if (dark_cnt>=MAX_PARTICLE_CNT){
									//return FALSE;
									over_limit = TRUE;
								}
							}
							else if(target_percent>limit_up){
								particle_white[white_cnt].val = bmp_y[index + n*img_w+m];
								particle_white[white_cnt].x = i+m;
								particle_white[white_cnt].y = img_h-1-(j+n);						
								white_cnt++;
								if (white_cnt>=MAX_PARTICLE_CNT){
									//	return FALSE;
									over_limit = TRUE;
								}
							}
						}


					}
				}
			}
		}

		result_low = min_val;
		result_up  = max_val;

		if (over_limit) return FALSE;

		return TRUE;
}
void shrink_image(const BYTE *source, BYTE *image, int cx, int cy, int &sub_w, int &sub_h,int shrink_size)
{
	if(shrink_size<1) shrink_size = 1;
	int x = cx/shrink_size;
	int y = cy/shrink_size;
	int offset_x = x%8;
	int offset_y = y%6;
	sub_w = x - offset_x;
	sub_h = y - offset_y;
//	log("cx=%d,cy=%d",sub_h,sub_w);


	int sum = 0;
	log("sum=%d",200+source[(15*shrink_size+5)*cx + 20*shrink_size+3]);
	for (int j=0; j<sub_h; j++){
		for (int i=0; i<sub_w; i++){
			sum = 0;
			for (int m=0; m<shrink_size; m++){
				for (int n=0; n<shrink_size; n++){
					sum += source[(j*shrink_size+m)*cx + i*shrink_size+n];
				}
			}
			image[j*sub_w+i] = sum/(shrink_size*shrink_size);

		}
	}
}

int median(int x1, int x2, int x3)
{
	int x[3] = {x1, x2, x3};
	int temp = 0;
	for (int i=0; i<3; i++){
		for (int j=1; j<3; j++){
			if (x[j-1]>x[j]){
				temp = x[j-1];
				x[j-1] = x[j];
				x[j] = temp;
			}
		}
	}

	return x[1];
}

int mysum(int x1, int x2, int x3)
{
	return (x1+x2+x3);
}
                    //shrink_y_buf, scan_y_buf, sub_w, sub_h, scan_distance, scan_threshold, use_x, use_y
void scan_image(const BYTE *source, BYTE *dest, int cx, int cy, int distance, int threshold, int use_x, int use_y)
{
	int avg_l = 0;
	int avg_r = 0;
	int avg_c = 0;
	int avg_i = 0;
// 	BYTE *bmp_large_x = new BYTE[3*cx,cy];
// 	BYTE *bmp_large_y = new BYTE[cx,3*cy];
	/*BYTE *dest = new BYTE[cx*cy];*/

	int sum=0;
	for(int i=0;i<cy;i++){
		for(int j=0;j<distance;j++){
			bmp_large_x[i*(cx+2*cx)+j+cx-distance]=source[i*cx];
			bmp_large_x[(i+1)*(cx+2*cx)-cx+j]=source[(i+1)*cx-1];
			sum++;
			
// 			log("source[%d]=%d,%d",(i+1)*cx-1,source[(i+1)*cx-1],sum);
//  			log("bmpx[%d]=%d",(i+1)*(cx+2*cx)-cx+j,bmp_large_x[(i+1)*(cx+2*cx)-cx+j]);
//  			
 			/*log("bmpx[%d]=%d",1025,bmp_large_x[1025]);*/
		}
	}//À©³ä±ß½ç
//	log("step1 done.");

	for(int i=0;i<cy;i++){
		for(int j=0;j<cx;j++){
			bmp_large_x[i*(cx+2*cx)+cx+j]=source[i*cx+j];
		//	log("large_num=%d,source_num=%d",i*(cx+2*cx)+cx+j,i*cx+j);
		}
	}//¸³Öµ
//	log("step2 done.");
	if (use_x){
		for (int j=1; j<cy-1; j++){
			for (int i=cx; i<cx+cx; i++){
				if(bmp_large_x[(j-1)*(cx+2*cx)+i-distance]!=0&&bmp_large_x[(j-1)*(cx+2*cx)+i+distance]!=0&&bmp_large_x[(j-1)*(cx+2*cx)+i]!=0){
					avg_l = median(bmp_large_x[(j-1)*(cx+2*cx)+i-distance], bmp_large_x[j*(cx+2*cx)+i-distance], bmp_large_x[(j+1)*(cx+2*cx)+i-distance]);
					avg_r = median(bmp_large_x[(j-1)*(cx+2*cx)+i+distance], bmp_large_x[j*(cx+2*cx)+i+distance], bmp_large_x[(j+1)*(cx+2*cx)+i+distance]);
					avg_c = /*mysum(bmp_large[(j-1)*cx+i],*/ bmp_large_x[j*(cx+2*cx)+i]/*, bmp_large[(j+1)*cx+i])*/;   //IXY

					avg_i = (avg_l+avg_r)-avg_c/*source[j*cx+i]*/*2;
					
					dest[(j-1)*cx+i-cx] = avg_i/*abs(avg_i)*/>=threshold?1/*source[j*cx+i]*/:255;
					

					/*log("dest_num=%d",j*cx+i-cx);*/
					/*if (avg_i>=threshold||avg_i<-threshold)
					{
						dest[(j-1)*cx+i-cx]=255;
					}
					else
						dest[(j-1)*cx+i-cx]=0;*/


				}
			}
		}

	}
//	log("step3 done.");
// 	for(int i=0;i<MAX_IMAGE_H;i++){
// 		for(int j=0;j<MAX_IMAGE_W;j++){
// 			bmp_large[i*MAX_IMAGE_H+j]=0;
// 		}
// 	}//clear

	for(int i=0;i<cx;i++){
		for(int j=0;j<distance;j++){
			bmp_large_y[(j+cy-distance)*cx+i]=source[i];
			bmp_large_y[(j+cy+cy)*cx+i]=source[cx*(cy-1)+i];
		}
	}
	for(int i=0;i<cy;i++){
		for(int j=0;j<cx;j++){
			bmp_large_y[(i+cy)*cx+j]=source[i*cx+j];
		}
	}
//	log("step4 done.");
	if (use_y){
		int avg_b = 0;
		int avg_t = 0;
		int offset = 0;

		for (int i=1; i<cx-1; i++){
			for (int j=cy; j<cy-offset+cy; j++){
				if(bmp_large_y[(j-distance)*cx+i]!=0&&bmp_large_y[j*cx+i]!=0&&bmp_large_y[(j+distance)*cx+i]!=0){
					/*log("ok.t=%d,c=%d,b=%d",bmp_large_y[(j-distance)*cx+i],bmp_large_y[j*cx+i],bmp_large_y[(j+distance)*cx+i]);*/

  					avg_b = median(bmp_large_y[(j-distance)*cx+i-1], bmp_large_y[(j-distance)*cx+i], bmp_large_y[(j-distance)*cx+i+1]);
  					avg_t = median(bmp_large_y[(j+distance)*cx+i-1], bmp_large_y[(j+distance)*cx+i], bmp_large_y[(j+distance)*cx+i+1]);		
					avg_c = /*mysum(bmp_large[(j)*cx+i-1], */bmp_large_y[j*cx+i]/*, bmp_large[(j)*cx+i+1])*/;
			//		avg_i = (avg_b+avg_t) - source[j*cx+i]*2;
					avg_i = (avg_b+avg_t)- avg_c*2;
					
					if (dest[(j-cy)*cx+i] != 1){
						dest[(j-cy)*cx+i] =  (avg_i)>=threshold?1:255;
					}
					
					/*if (avg_i>=threshold||avg_i<-threshold)
					{
						dest[(j-cy)*cx+i]=255;
					}
					else
						dest[(j-cy)*cx+i]=0;*/
				}
// 				else
// 					log("err.t=%d,c=%d,b=%d",bmp_large_y[(j-distance)*cx+i],bmp_large_y[j*cx+i],bmp_large_y[(j+distance)*cx+i]);
			}
		}
	}
//	log("scan1 done.");
// 	delete [] bmp_large_x;
// 	delete [] bmp_large_y;
// 	
	log("scan done.");
}

int filter_close_image(const BYTE *source, BYTE *dest, int cx, int cy, int pattern_x, int pattern_y, int close_val)
{
	int sum = 0;
	int sub_sum = 0;
	int avg = 0, sub_avg = 0;
	int w= pattern_x;
	int h= pattern_y;

	BYTE *dest_t = new BYTE[cx*cy];

	memset(dest_t, 255, cx*cy);
	int min_val = 255;

	int pixel_count = 0;

	for (int j=h; j<cy-h; j+=h){
		for (int i=w; i<cx-w; i+=w){
			
			pixel_count = 0;
			for (int n=j; n<j+h*2; n++){
				for (int m=i; m<i+w*2; m++){
					if (source[n*cx+m] <= 1) pixel_count++;
				}
			}

			if (pixel_count>close_val){
				for (int n=j; n<j+h*2; n++){
					for (int m=i; m<i+w*2; m++){
						dest_t[n*cx+m] =source[n*cx+m];
					}
				}
			}
		}
	}

	sum = 0;
	for (int i=0; i<cx*cy; i++){
		if (dest_t[i] == 255){
			dest[i*3] = 255;
			dest[i*3+1] = 255;
			dest[i*3+2] = 255;
		}
		else{
			dest[i*3] = 0;
			dest[i*3+1] = 0;
			dest[i*3+2] = 255;
		}
		sum += dest[i*3];
	}
	avg = sum/(cx*cy);
	delete [] dest_t;

	return avg;



}

int find_redpoint(BYTE *buf,int w,int h,int &red_cnt,MARK_RED *particle_red,int &white_time)
{

	int i,j,r,g,b;
	int m=0;

	/**********************************/	//(x1,y1)
	for(int m=0;m<=76;m=m+4)
	{
		bool ischeck = false;                              
		
		for (i=0;i<h;i++)
		{
			for (j=0;j<w;j++)
			{
				b=buf[3*(i*w+j)];
				g=buf[3*(i*w+j)+1];
				r=buf[3*(i*w+j)+2];

				if (b!=255&&buf[3*(i*w+j)+3]!=255&&buf[3*(i*w+j)+6]!=255&&buf[3*(i*w+j)+9]!=255&&buf[3*(i*w+j)+12]!=255)
				{
					particle_red[red_cnt+m].x=j;
					particle_red[red_cnt+m].y=i;
		            
					ischeck  = true;
					white_time++;
					
					break;
				}

			}	
			if(ischeck)
			{
				break;
			}
		}
		

		/**********************************/   //(x2,y2)
		for (i=particle_red[red_cnt+m].y;i<(particle_red[red_cnt+m].y+100);i++)
		{
			for (j=(particle_red[red_cnt+m].x-50);j<(particle_red[red_cnt+m].x+50);j++)
			{
				b=buf[3*(i*w+j)];
				g=buf[3*(i*w+j)+1];
				r=buf[3*(i*w+j)+2];
				if (b!=255&&buf[3*(i*w+j)+3]!=255&&buf[3*(i*w+j)+6]!=255&&buf[3*(i*w+j)+9]!=255&&buf[3*(i*w+j)+12]!=255)
				{
					particle_red[red_cnt+m+1].x=j;
					particle_red[red_cnt+m+1].y=i;


				}	
			}	

		}

		/**********************************/	 //(x3,y3)

		bool ischeck1 = false;
		for (j=(particle_red[red_cnt+m].x-50);j<(particle_red[red_cnt+m].x+50);j++)
		{
			for (i=particle_red[red_cnt+m].y;i<(particle_red[red_cnt+m].y+100);i++)
			{
				b=buf[3*(i*w+j)];
				g=buf[3*(i*w+j)+1];
				r=buf[3*(i*w+j)+2];
				if (b!=255&&buf[3*(i*w+j)+3]!=255&&buf[3*(i*w+j)+6]!=255&&buf[3*(i*w+j)+9]!=255&&buf[3*(i*w+j)+12]!=255)
				{

					particle_red[red_cnt+2+m].x=j;
					particle_red[red_cnt+2+m].y=i;
					
					ischeck1  = true;
					break;	
				}	

			}	
			if(ischeck1)
			{
				break;
			}


		}

		
		/**********************************/ //(x4,y4)
		for (j=(particle_red[red_cnt+m].x-50);j<(particle_red[red_cnt+m].x+50);j++)
		{
			for (i=particle_red[red_cnt+m].y;i<(particle_red[red_cnt+m].y+100);i++)
			{
				b=buf[3*(i*w+j)];
				g=buf[3*(i*w+j)+1];
				r=buf[3*(i*w+j)+2];
				if (b!=255&&buf[3*(i*w+j)+3]!=255&&buf[3*(i*w+j)+6]!=255&&buf[3*(i*w+j)+9]!=255&&buf[3*(i*w+j)+12]!=255)
				{
					particle_red[red_cnt+m+3].x=j;
					particle_red[red_cnt+m+3].y=i;
					


				}	
			}
			
		}

		/*for (i=1;i<5;i++)
		{
			if (ischeck==FALSE&&ischeck1==FALSE)
			{
				particle_red[i].x=0;
				particle_red[i].y=0;
			}

		}*/

		

		/**********************************/ //Í¿°×
		for (i=particle_red[red_cnt+m].y;i<(particle_red[red_cnt+1+m].y+10);i++)
		{
			for (j=(particle_red[red_cnt+2+m].x);j<(particle_red[red_cnt+3+m].x+3);j++)
			{
				b=buf[3*(i*w+j)];
				g=buf[3*(i*w+j)+1];
				r=buf[3*(i*w+j)+2];
				if (b!=255)
				{
		            buf[3*(i*w+j)]=255;
					buf[3*(i*w+j)+1]=255;
					buf[3*(i*w+j)+2]=255;


				}	
			}	
			
		}	
	}
	return 0;
}

INIEdit white_image_config[] =
{
	{IDC_SHRINK_SIZE,	        DLGTYPE_INPUT_INT,		&white_v.shrink_size,					"shrink_size"},
	{IDC_SCAN_DISTANCE_VAL,	    DLGTYPE_INPUT_INT,		&white_v.scan_distance,					"scan_distance"},
	{IDC_SCAN_THRESHOLD_VAL,    DLGTYPE_INPUT_INT,		&white_v.scan_threshold,				"scan_threshold"},
	{IDC_FILTER_CLOSE_VAL,      DLGTYPE_INPUT_INT,		&white_v.filter_close,					"filter_close"},
	{IDC_PARTICLE_UP_LIMIT,	    DLGTYPE_INPUT_FLOAT,	&white_v.particle_up_limit,				"particle_up_limit"},
	{IDC_PARTICLE_LOW_LIMIT,    DLGTYPE_INPUT_FLOAT,	&white_v.particle_low_limit,			"particle_low_limit"},
	{IDC_PARTICLE_TEST_ON,		DLGTYPE_CHECK_BOOL,		&white_v.is_particle_test_on,           "is_particle_test_on"	         },
	{IDC_BLEMISH_TEST_ON,		DLGTYPE_CHECK_BOOL,		&white_v.is_blemish_test_on,		     "is_blemish_test_on"            },
	{IDC_SCAN_DIRECTION_X,		DLGTYPE_CHECK_BOOL,		&white_v.is_scan_directionx_test_on,    "is_scan_directionx_test_on"	 },
	{IDC_SCAN_DIRECTION_Y,		DLGTYPE_CHECK_BOOL,		&white_v.is_scan_directiony_test_on,	 "is_scan_directiony_test_on" },
	

	{NULL, NULL, NULL, NULL}
};

class WhiteDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual void close(HWND hdlg);

private:
	char path[MAX_PATH];
	int is_initial_finished;

};

BOOL WhiteDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	is_initial_finished = FALSE;

	sprintf(path, "%s\\recipe\\%s\\white.ini", s_test::dir, s_test::recipe_name);
	ai_win::load_keys(path, "setting", hdlg, white_image_config, sizeof(white_image_config)/sizeof(white_image_config[0]));


	

	/*SetDlgItemInt(hdlg, IDC_OC_RESULT,	 white_v.oc_test_result, FALSE);
	SetDlgItemInt(hdlg, IDC_OC_X_RESULT, white_v.oc_x_result,	  FALSE);
	SetDlgItemInt(hdlg, IDC_OC_Y_RESULT, white_v.oc_y_result,	  FALSE);

	SET_CHECKER(hdlg, IDC_SAVE_BMP_IMAGE,    white_v.is_save_bmp_image);
	SET_CHECKER(hdlg, IDC_SAVE_RAW_IMAGE,    white_v.is_save_raw_image);
	SET_CHECKER(hdlg, IDC_SAVE_RESULT_IMAGE, white_v.is_save_result_image);*/

	is_initial_finished = TRUE;

	return TRUE;
}

void WhiteDlg::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{
	switch (id){
	case IDC_SAVE:
		ai_win::save_keys(path, "setting", HWND_WHITE, white_image_config, 100);
		ai_win::load_keys(path, "setting", HWND_WHITE, white_image_config, 100);
		
		break;
	}

	switch (code_notify){
	case EN_CHANGE:
	case BN_CLICKED:
		if (is_initial_finished){
			ai_win::save_keys(path, "setting", hdlg, white_image_config, sizeof(white_image_config)/sizeof(INIEdit));
		}
		break;
	}
}

void WhiteDlg::close(HWND hdlg)
{
	DestroyWindow(hdlg);
	HWND_WHITE = NULL;
}

WhiteDlg white;
BOOL CALLBACK white_test_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG,	white.init	);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,		white.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,		white.close  );
	}

	return FALSE;
}

int blemish_test(HWND hdlg, BYTE *bmp_y, int img_w, int img_h)
{
	for (int i=1;i<2000;i++)
	{ 
		particle_red[i].x=0;
		particle_red[i].y=0;
		particle_red1[i].x=0;
		particle_red1[i].y=0;
	}
	white_v.sub_w = img_w;
	white_v.sub_h = img_h;
	white_v.close_pattern_x=4;
	white_v.close_pattern_y=3;

	shrink_image(bmp_y, shrink_y_buf, img_w, img_h, white_v.sub_w, white_v.sub_h,1);

	int use_x = white_v.is_scan_directionx_test_on;
	int use_y = white_v.is_scan_directiony_test_on;
	memset(scan_y_buf, 255, white_v.sub_w*white_v.sub_h);
	scan_image(shrink_y_buf, scan_y_buf, white_v.sub_w,  white_v.sub_h,  white_v.scan_distance,  white_v.scan_threshold, use_x, use_y);

	filter_close_image(scan_y_buf, filter_close_buf, white_v.sub_w, white_v.sub_h, white_v.close_pattern_x, white_v.close_pattern_y, white_v.filter_close);
	
	find_redpoint(filter_close_buf,img_w,img_h,particle_red_cnt,particle_red,white_v.white_time);
	//log("white_")
	int result = TRUE;
	white_v.blemish_test_result = result;
	draw_blemish = TRUE;
	return result;
}

int particle_test(HWND hdlg, BYTE *bmp_y, int img_w, int img_h)
{
	particle_dark_cnt = 0;
	particle_white_cnt = 0;
	for (int i=0; i<MAX_PARTICLE_CNT; i++){
		particle_dark[i].x = 0;
		particle_dark[i].y = 0;
		particle_dark[i].val = 0;

		particle_white[i].x = 0;
		particle_white[i].y = 0;
		particle_white[i].val = 0;
	}
	
	fade_zone(bmp_bin,bmp_y,bmp_y_buf_copy,img_w,img_h);
	int result = particle_proc(bmp_y_buf_copy, img_w, img_h, white_v.particle_low_limit, white_v.particle_up_limit,
		white_v.particle_low_result, white_v.particle_up_result, particle_dark_cnt, particle_white_cnt, 
		particle_dark, particle_white);
	
	draw_particle = TRUE;
	


	return 0;
}

BOOL draw_blemish_position(HDC memdc,MARK_RED *particle_red)
{
	char buffer[100] = {0};

	HFONT hfont	 = CreateFont(24, 0,0,0,20,0,0,0,ANSI_CHARSET,3,2,1, VARIABLE_PITCH | FF_ROMAN, "ºÚÌå");  //2M camera
	HFONT old_font = (HFONT)SelectObject(memdc, hfont);
	SetTextColor(memdc, COLOR_BLUE);


	HPEN pen		= CreatePen(PS_SOLID, 0, COLOR_GREEN);//»ñÈ¡¾ØÐÎ»­±ÊÉèÖÃ
	HPEN old_pen = (HPEN)SelectObject(memdc, pen);
	if (draw_blemish == TRUE){
	int i=1;
	int number=1;
	for (i=1;i<=(1+(white_v.white_time-1)*4);i=i+4)
	{

		particle_red1[i].x  =particle_red[i].x  ;
		particle_red1[i].y  =particle_red[i].y  ;
		particle_red1[i+1].x=particle_red[i+1].x;
		particle_red1[i+1].y=particle_red[i+1].y;
		particle_red1[i+2].x=particle_red[i+2].x;
		particle_red1[i+2].y=particle_red[i+2].y;
		particle_red1[i+3].x=particle_red[i+3].x;
		particle_red1[i+3].y=particle_red[i+3].y;


		particle_red1[i].y    =IMG_HEIGHT-particle_red1[i].y;
		particle_red1[i+1].y  =IMG_HEIGHT-particle_red1[i+1].y;
		particle_red1[i+2].y  =IMG_HEIGHT-particle_red1[i+2].y;
		particle_red1[i+3].y  =IMG_HEIGHT-particle_red1[i+3].y;


		MoveToEx(memdc,particle_red1[i+2].x, particle_red1[i].y, NULL);
		LineTo(memdc,  particle_red1[i+3].x, particle_red1[i].y);       
		MoveToEx(memdc,particle_red1[i+2].x, particle_red1[i+1].y-5, NULL);
		LineTo(memdc,  particle_red1[i+3].x, particle_red1[i+1].y-5);
		MoveToEx(memdc,particle_red1[i+2].x, particle_red1[i].y, NULL);
		LineTo(memdc,  particle_red1[i+2].x, particle_red1[i+1].y-5);
		MoveToEx(memdc,particle_red1[i+3].x, particle_red1[i].y, NULL);
		LineTo(memdc,  particle_red1[i+3].x, particle_red1[i+1].y-5);

		sprintf(buffer, "#%d", number);
		TextOut(memdc, particle_red1[i+2].x, particle_red1[i].y, buffer, strlen(buffer));
/*		log("#%d,(%d,%d),(%d,%d),(%d,%d),(%d,%d)",number,
			particle_red1[i+2].x,particle_red1[i+1].y-5,
			particle_red1[i+2].x, particle_red1[i].y,
			particle_red1[i+3].x, particle_red1[i].y,
			particle_red1[i+3].x, particle_red1[i+1].y-5);*/
		number++;


	}
 }
	DeleteObject(pen);
	DeleteObject(hfont);
	return TRUE;
}

BOOL draw_defect_position(HDC memdc)
{
	SetBkMode(memdc, TRANSPARENT);
	SelectObject(memdc, GetStockObject(NULL_BRUSH));
	HPEN pen		 = CreatePen(PS_SOLID, 0, RGB(255, 255, 0));
	HPEN old_pen  = (HPEN)SelectObject(memdc, pen);

	Rectangle(memdc, white_v.start_x, white_v.start_y, white_v.end_x, white_v.end_y);		

	if (draw_particle == TRUE){
		log("start draw on img.");
		int start_draw_x = 0;
		int start_draw_y = 0;
		int box_size = 15;
		DeleteObject(pen);
		SelectObject(memdc, (HPEN)old_pen);
		pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
		old_pen  = (HPEN)SelectObject(memdc, pen);
		for (int i=0; i<particle_dark_cnt; i++){
			Rectangle(memdc, start_draw_x+particle_dark[i].x - box_size, start_draw_y+particle_dark[i].y-box_size,
				start_draw_x+particle_dark[i].x+box_size, start_draw_y+particle_dark[i].y+box_size);
		}
		pen = CreatePen(PS_SOLID, 8, RGB(255, 255, 0));
		old_pen  = (HPEN)SelectObject(memdc, pen);
		for (int i=0; i<particle_white_cnt; i++){
			Rectangle(memdc, start_draw_x+particle_white[i].x - box_size, start_draw_y+particle_white[i].y-box_size,
				start_draw_x+particle_white[i].x+box_size, start_draw_y+particle_white[i].y+box_size);
		}
	}


	DeleteObject(pen);
	return TRUE;
}

int init_white_test(void)
{
	char path[MAX_PATH] = {0};
	sprintf(path, "%s\\recipe\\%s\\white.ini", s_test::dir, s_test::recipe_name);

	ai_win::load_keys(path, "setting", HWND_WHITE, white_image_config, 100);

	return TRUE;
}
int exec_white_test(BYTE *bmp, int w, int h)
{

	white_v.is_particle_draw_on = FALSE;
	white_v.is_blemish_draw_on = FALSE;
	white_v.white_time=0;


	BYTE *bmp_y = new BYTE[w*h];
	int result =TRUE;
	cimg.bmp24_to_y8(bmp,w,h,NULL,bmp_y);
	if (white_v.is_particle_test_on){
		result = particle_test(HWND_WHITE, bmp_y, w, h);
		white_v.is_particle_draw_on = TRUE;
	}

	if (white_v.is_blemish_test_on){
		result = blemish_test(HWND_WHITE, bmp_y, w, h);
		white_v.is_blemish_draw_on = TRUE;
	}

	
	

	delete [] bmp_y;

	return result;
}
void draw_white_test(HDC memdc)
{
	if (white_v.is_particle_draw_on) draw_defect_position(memdc);
	if (white_v.is_blemish_draw_on)  draw_blemish_position(memdc,particle_red);

}

