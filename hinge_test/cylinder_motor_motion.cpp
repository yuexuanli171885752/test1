#include "interface.h"
#include "inf_motion_aa.h"


int motor_product(void)
{
	int errcode=0;

	errcode=SM103_move_to_pass_position();
	errcode=SM104_move_to_pass_position();

	return 0;
}
int motion_product(void)
{
	int errcode=0;

	errcode=SM103_move_to_pass_position();
	errcode=SM104_move_to_pass_position();

	SM103_COUNT++;
	if (SM103_COUNT == 6)
	{
		SM104_COUNT++;
		SM103_COUNT=0;
	}
	return 0;
}

int motor_ng_product(void)
{
	int errcode = 0;

	if(errcode=SM103_move_to_ng_position()!=SUCCESS)   return errcode;
	if(errcode=SM104_move_to_ng_position()!=SUCCESS)   return errcode;

	return 0;
}
int motion_ng_product(void)
{
	int errcode = 0;

	if(errcode=SM103_move_to_ng_position()!=SUCCESS)   return errcode;
	if(errcode=SM104_move_to_ng_position()!=SUCCESS)   return errcode;

	SM103_NG_COUNT++;
	if (SM103_NG_COUNT == 3)
	{
		SM104_NG_COUNT++;
		SM103_NG_COUNT=0;
	}
	return 0;
}
int motor_home_product(void)
{
	int errcode = 0;

	if(errcode=SM103_move_product_home_position()!=SUCCESS)   return errcode;
	if(errcode=SM104_move_product_home_position()!=SUCCESS)   return errcode;

	return 0;
}



