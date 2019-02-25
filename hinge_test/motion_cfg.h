#ifndef _MOTION_CFG_H_
#define _MOTION_CFG_H_
#define  PRODUCT_NUM  1000
namespace t_flag
{
	extern int equipment_start;				
	extern int feed_request_product;			//上一站请求放料信息
	extern int feed_product;					//入料信息
	extern int feed_product;					//入料信息
	extern int feed_right_staus;				//待夹爪上转盘信息
	extern int feed_left_staus;					//等待新产品上料信息
	extern int feed_griper_staus;				//夹爪工作信息
	extern int feed_move_start;					//入料气缸位移信息

	extern int site1_task_product;				//一工位产品信息
	extern int site1_up_product;				//一工位上料
	extern int site1_down_product;				//一工位下料信息

	extern int site2_task_product;				//二工位产品信息
	extern int site2_test_end;
	extern int site2_test_start;

	extern int site3_task_product ;
	extern int site3_test_end;
	extern int site3_test_start;

	extern int site4_task_product;
	extern int site4_test_end;
	extern int site4_test_start;

	extern int table_task1_staus;				//圆盘旋转一工位信息
	extern int table_task2_staus;				//圆盘旋转二工位信息
	extern int table_task3_staus;				//圆盘旋转三工位信息
	extern int table_task4_staus;				//圆盘旋转四工位信息
	extern int table_feed_staus;				//圆盘旋转入料信息
	extern int table_gripper_staus;			//圆盘旋转夹爪位信息

	extern int table_staus_safe;				//转盘安全信息

	extern int carry_product_left_wait;			//物料输送左到位发送等待信息
	extern int carry_product_right_wait;		//物料输送右到位发送等待信息	
	extern int product_carry_left;			//放至物料输送到位信息
	extern int product_carry_right;			//物料输送夹取到位信息
	extern int pass_product;					//产品合格
	extern int ng_product;						//产品NG
	extern int pass_product_tray;				//合格托盘就位
	extern int ng_product_tray;					//NG托盘就位
	extern int ng_tray_reset ;					//NG托盘复位

	extern int product_code[PRODUCT_NUM];
	extern int site2_num;							//二工位站测试编号
	extern int site3_num;
	extern int site4_num;
	extern int carry_num;							//运输物料编号
}
#endif