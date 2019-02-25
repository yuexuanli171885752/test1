#ifndef _MOTION_CFG_H_
#define _MOTION_CFG_H_
#define  PRODUCT_NUM  1000
namespace t_flag
{
	extern int equipment_start;				
	extern int feed_request_product;			//��һվ���������Ϣ
	extern int feed_product;					//������Ϣ
	extern int feed_product;					//������Ϣ
	extern int feed_right_staus;				//����צ��ת����Ϣ
	extern int feed_left_staus;					//�ȴ��²�Ʒ������Ϣ
	extern int feed_griper_staus;				//��צ������Ϣ
	extern int feed_move_start;					//��������λ����Ϣ

	extern int site1_task_product;				//һ��λ��Ʒ��Ϣ
	extern int site1_up_product;				//һ��λ����
	extern int site1_down_product;				//һ��λ������Ϣ

	extern int site2_task_product;				//����λ��Ʒ��Ϣ
	extern int site2_test_end;
	extern int site2_test_start;

	extern int site3_task_product ;
	extern int site3_test_end;
	extern int site3_test_start;

	extern int site4_task_product;
	extern int site4_test_end;
	extern int site4_test_start;

	extern int table_task1_staus;				//Բ����תһ��λ��Ϣ
	extern int table_task2_staus;				//Բ����ת����λ��Ϣ
	extern int table_task3_staus;				//Բ����ת����λ��Ϣ
	extern int table_task4_staus;				//Բ����ת�Ĺ�λ��Ϣ
	extern int table_feed_staus;				//Բ����ת������Ϣ
	extern int table_gripper_staus;			//Բ����ת��צλ��Ϣ

	extern int table_staus_safe;				//ת�̰�ȫ��Ϣ

	extern int carry_product_left_wait;			//����������λ���͵ȴ���Ϣ
	extern int carry_product_right_wait;		//���������ҵ�λ���͵ȴ���Ϣ	
	extern int product_carry_left;			//�����������͵�λ��Ϣ
	extern int product_carry_right;			//�������ͼ�ȡ��λ��Ϣ
	extern int pass_product;					//��Ʒ�ϸ�
	extern int ng_product;						//��ƷNG
	extern int pass_product_tray;				//�ϸ����̾�λ
	extern int ng_product_tray;					//NG���̾�λ
	extern int ng_tray_reset ;					//NG���̸�λ

	extern int product_code[PRODUCT_NUM];
	extern int site2_num;							//����λվ���Ա��
	extern int site3_num;
	extern int site4_num;
	extern int carry_num;							//�������ϱ��
}
#endif