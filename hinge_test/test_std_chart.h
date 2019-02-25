#ifndef _TEST_STD_CHART_H_
#define _TEST_STD_CHART_H_

#define STD_FOCUS_CURVE_RANGE				100
#define SFR_ROI_NUM							26

typedef struct _tagTab
{
	HWND hchild;
	char title[100];
	DLGPROC proc;
	int idd;
	int is_enabled;
}TabInfo;

extern float y_ratio[GRAY_ROI_NUM-1];
extern double color_delta_val[COLOR_ROI_NUM];
typedef struct _tagStdChart
{
	//////////////////////////////////////////////////////////////////////////
	//fiducial mark
	int sb_to_img_cx;
	int sb_to_img_cy;
	int sb_size;
	int cl_thinkness;
	int cl_length;
	RECT scanbox[4];
	int cl_center_x[4];
	int cl_center_y[4];

	double cl_x_distance;
	double cl_y_distance;
	double chart_center_x;
	double chart_center_y;
	double chart_rotate_angle;

	int center_x_offset;
	int center_y_offset;

	//////////////////////////////////////////////////////////////////////////
	//sfr
	double freq1;
	double freq2;
	double spec1[4];
	double spec2[4];
	double roi_to_ccx[4];
	double roi_to_ccy[4];
	int roi_w[4];
	int roi_h[4];
	int is_field_a_on;
	int is_field_b_on;
	int is_field_c_on;
	int is_spec_2_on;
	int is_direct_h_on;
	int is_direct_v_on;
	double v_offset_x;
	double v_offset_y;
	int final_result;

	int is_tilt_test_on;
	int h_tilt_spec;
	int v_tilt_spec;
	int tilt_test_result;

	int target_value;
	int target_ignore_spec;

	int is_save_report;
	int is_save_raw_image;
	int is_save_bmp_image;
	int is_save_result_image;
	int is_save_curve_data;
	int is_save_curve_image;

	int	   sfr_test_result;
	double test_time;
	RECT   sfr_roi[SFR_ROI_NUM];
	int    sfr_result1[SFR_ROI_NUM];
	int    sfr_result2[SFR_ROI_NUM];
	double  sfr_val1[SFR_ROI_NUM];
	double  sfr_val2[SFR_ROI_NUM];
	int    is_use_mtf50p;

	double slope[SFR_ROI_NUM];
	double offset[SFR_ROI_NUM];

	//////////////////////////////////////////////////////////////////////////
	//optical center
	int is_oc_test_on;
	int oc_x_offset_spec;
	int oc_y_offset_spec;
	int oc_x_pos;
	int oc_y_pos;
	int oc_test_result;

	//////////////////////////////////////////////////////////////////////////
	//color
	int is_color_test_on;
	int color_box_on[6];
	int use_lab;
	int ref_r[6];
	int ref_g[6];
	int ref_b[6];
	float color_spec[6];
	int color_box_x[6];
	int color_box_y[6];
	int color_box_w;
	int color_box_h;
	float color_cb[6];
	float color_cr[6];
	int color_test_result[6];
	float color_delta[6];
	int color_reproduction_test_result;

	//////////////////////////////////////////////////////////////////////////
	//gray
	int is_gray_test_on;
	int gray_box_x[6];
	int gray_box_y[6];
	int gray_test_box_count;
	float gray_luminance_change_spec;
	int gray_pass_box_count_spec;
	int gray_pass_box_count;
	int gray_box_w;
	int gray_box_h;
	float gray_var_val[5];
	int gray_var_result[5];

	float gray_y_val[6];
	float gray_min_var_result;
	int gray_test_result;

	//////////////////////////////////////////////////////////////////////////
	//SN
	int is_sn_test_on;
	int sn_gray_box_num;
	float sn_spec;
	double sn_result;
	int sn_test_result;

	//////////////////////////////////////////////////////////////////////////
	//rotation
	int is_rotate_test_on;
	float rotate_spec;
	float rotate_result;
	int rotate_test_result;

}StdChart;

/*
namespace std_chart
{
	extern float test_time;

	extern int is_color_on;
	extern int color_x_distance[COLOR_ROI_NUM];
	extern int color_y_distance[COLOR_ROI_NUM];
	extern int color_roi_w;
	extern int color_roi_h;
	extern int ref_r[COLOR_ROI_NUM];
	extern int ref_g[COLOR_ROI_NUM];
	extern int ref_b[COLOR_ROI_NUM];
	extern float ref_h[COLOR_ROI_NUM];
	extern float ref_s[COLOR_ROI_NUM];

	extern double color_spec[COLOR_ROI_NUM];
	extern double color2_spec[COLOR_ROI_NUM];
	extern float  color_h[COLOR_ROI_NUM];
	extern float  color_s[COLOR_ROI_NUM];

	extern int color_delta_result[COLOR_ROI_NUM];
	extern int color_result;

	extern int is_gray_on;
	extern int gray_x_distance[GRAY_ROI_NUM];
	extern int gray_y_distance[GRAY_ROI_NUM];
	extern int gray_roi_w;
	extern int gray_roi_h;
	extern int gray_change_spec[GRAY_ROI_NUM-1];
	extern float gray_change_val[GRAY_ROI_NUM-1];
	extern int gray_comp_result[GRAY_ROI_NUM-1];
	extern int gray_result;


	extern int is_fov_on;
	extern int fov_x_distance[FOV_ROI_NUM];
	extern int fov_y_distance[FOV_ROI_NUM];
	extern int fov_roi_w;
	extern int fov_roi_h;
	extern int fov_roi_ww;
	extern int fov_roi_hh;
	extern int fov_h;			//
	extern int fov_v;			//
	extern int fov_h_val;
	extern int fov_v_val;
	extern int fov_h_factor;
	extern int fov_v_factor;
	extern int fov_h_limit;
	extern int fov_v_limit;
	extern int fov_result;

	extern int is_wb_on;
	extern int wb_select_roi_num;
	extern double wb_up_limit;
	extern double wb_spec[GRAY_ROI_NUM];
	extern float wb_val_rg[GRAY_ROI_NUM];
	extern float wb_val_bg[GRAY_ROI_NUM];
	extern int wb_test_result[GRAY_ROI_NUM];
	extern int wb_result;
	extern float wb;

	extern int is_snr_on;
	extern int snr_select_roi_num;
	extern double snr_low_limit;
	extern double snr_val[GRAY_ROI_NUM];
	extern int snr_test_result[GRAY_ROI_NUM];
	extern int snr_result;
	extern double snr_val_0;

	extern int is_oc_on;						//try 4
	extern double oc_up_limit;
	extern int oc_target_x;
	extern int oc_target_y;
	extern int oc_x_result;
	extern int oc_y_result;
	extern int oc_result;

	extern int color_roi_x[COLOR_ROI_NUM];
	extern int color_roi_y[COLOR_ROI_NUM];
	extern int gray_roi_x[GRAY_ROI_NUM];
	extern int gray_roi_y[GRAY_ROI_NUM];
	extern int fov_roi_x[FOV_ROI_NUM];
	extern int fov_roi_y[FOV_ROI_NUM];

	extern int is_save_image;
	extern int chart_result;
	extern int bin_threshold;
	extern int pattern_size;
	extern int target_distance;
	extern int oc_roi_size;

	extern double oc_a_x_offset;
	extern double oc_a_y_offset;
	extern double oc_b_x_offset;
	extern double oc_b_y_offset;
	extern double oc_c_x_offset;
	extern double oc_c_y_offset;
	extern double oc_d_x_offset;
	extern double oc_d_y_offset;
	extern char report_path[MAX_PATH];
}*/

typedef struct _tagCurveNode
{
	double center;
	double edge[4];
	int x_distance;
	_tagCurveNode *next;
}CurveNode, *PCurveNode;




#endif