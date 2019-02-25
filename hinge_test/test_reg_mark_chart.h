#ifndef _TEST_REG_MARK_CHART_H_
#define _TEST_REG_MARK_CHART_H_

#define SLANTED_BW_NUM						5
#define FIELD_NUM							3
#define TF_MAX_CNT							50



typedef struct _tagSlantedBW
{
	char sn[300]; 
	int sn_cnt;
	int ct;

	int SfrCheckResult;
	int test_result;
	int pre_result;
	int white_result;
	int dispensing_result;
	int B_UVResult;
	int A_UVResult;

	int use_field_a;
	int use_field_b;

	int use_mtf50p;
	int mtfxp;	
	int sfrDiff_spec;
	int coorDiff_spec;
	double eCoorDiff;
	double eSfrDiff;
	double freq_val1;
	double freq_val2;
	int oc_limit;
	float tilt_limit;
	double tilt_x;
	double tilt_y;
	double tilt_x_pos;
	double tilt_y_pos;
	
	int mirror[4];
	int binary_threhold;
	int spec1[FIELD_NUM];
	int spec2[FIELD_NUM];
	int use_spec2;
	int roi_w[FIELD_NUM];
	int roi_h[FIELD_NUM];
	int search_w[FIELD_NUM];
	int search_h[FIELD_NUM];
	int start_x[SLANTED_BW_NUM];
	int start_y[SLANTED_BW_NUM];
	int roi_enable[4];
	int pattern_size[FIELD_NUM];
	int roi_distance[FIELD_NUM];

	int find_target[SLANTED_BW_NUM];
	RECT search_rt[SLANTED_BW_NUM];
	RECT roi_rt[SLANTED_BW_NUM][4];
	int center_x[SLANTED_BW_NUM];
	int center_y[SLANTED_BW_NUM];

	double oc_x[SLANTED_BW_NUM];
	double oc_y[SLANTED_BW_NUM];
	double oc_slope[SLANTED_BW_NUM][4];
	double oc_offset[SLANTED_BW_NUM][4];


	int edge_x[SLANTED_BW_NUM][4];
	int edge_y[SLANTED_BW_NUM][4];
	double edge_v_x[SLANTED_BW_NUM][4][200];
	double edge_v_y[SLANTED_BW_NUM][4][200];

	double edge_h_x[SLANTED_BW_NUM][4][200];
	double edge_h_y[SLANTED_BW_NUM][4][200];

	double sfr_val1[SLANTED_BW_NUM][4];
	double sfr_val2[SLANTED_BW_NUM][4];
	int sfr_result[SLANTED_BW_NUM][4];
	int sfr_chartsize[4];

	double slope[SLANTED_BW_NUM][4];
	double offset[SLANTED_BW_NUM][4];
	double esf[SLANTED_BW_NUM][4][2000];
	double lsf[SLANTED_BW_NUM][4][2000];
	double freq[SLANTED_BW_NUM][4][1000];
	double amp[SLANTED_BW_NUM][4][1000];

	int is_save_bmp;
	int is_save_result_bmp;
	int is_save_sfr_curve;
	
}SlantedBW;



#endif