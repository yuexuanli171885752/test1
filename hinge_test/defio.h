//=========================================================================================  
//	Project:	MN200_Dispenser
//	Program:	defio.h
//===========================================================================================

#define inpw 	MN200_RemoteInputWord
#define outpw 	MN200_RemoteOutputWord

//======================================================================================
// General definition for Digital input / Digital output
//======================================================================================
//INPUT1
#define INPUT1	inpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID1, (BYTE)0)

#define S01		(((INPUT1 & 0x0001)==0x00)?OFF:ON)		//DI_0
#define	S02		(((INPUT1 & 0x0002)==0x00)?OFF:ON)      //DI_1
#define	S03		(((INPUT1 & 0x0004)==0x00)?OFF:ON)      //DI_2
#define	S04 	(((INPUT1 & 0x0008)==0x00)?OFF:ON)	    //DI_3
#define	S05		(((INPUT1 & 0x0010)==0x00)?OFF:ON)      //DI_4
#define	S06		(((INPUT1 & 0x0020)==0x00)?OFF:ON)      //DI_5
#define S07		(((INPUT1 & 0x0040)==0x00)?OFF:ON)      //DI_6
#define S08		(((INPUT1 & 0x0080)==0x00)?OFF:ON)      //DI_7
#define S09 	(((INPUT1 & 0x0100)==0x00)?OFF:ON)      //DI_8  
#define S10		(((INPUT1 & 0x0200)==0x00)?OFF:ON)      //DI_9
#define S11		(((INPUT1 & 0x0400)==0x00)?OFF:ON)      //DI_10
#define S12		(((INPUT1 & 0x0800)==0x00)?OFF:ON)      //DI_11
#define S13		(((INPUT1 & 0x1000)==0x00)?OFF:ON)      //DI_12
#define S14		(((INPUT1 & 0x2000)==0x00)?OFF:ON)      //DI_13
#define S15		(((INPUT1 & 0x4000)==0x00)?OFF:ON)      //DI_14
#define S16		(((INPUT1 & 0x8000)==0x00)?OFF:ON)      //DI_15

//INPUT2
#define INPUT2	inpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID2, (BYTE)0)

#define S17		(((INPUT2 & 0x0001)==0x00)?OFF:ON)		//DI_0
#define	S18		(((INPUT2 & 0x0002)==0x00)?OFF:ON)      //DI_1
#define	S19		(((INPUT2 & 0x0004)==0x00)?OFF:ON)      //DI_2
#define	S20 	(((INPUT2 & 0x0008)==0x00)?OFF:ON)	    //DI_3
#define	S21		(((INPUT2 & 0x0010)==0x00)?OFF:ON)      //DI_4
#define	S22		(((INPUT2 & 0x0020)==0x00)?OFF:ON)      //DI_5
#define S23		(((INPUT2 & 0x0040)==0x00)?OFF:ON)      //DI_6
#define S24		(((INPUT2 & 0x0080)==0x00)?OFF:ON)      //DI_7
#define S25 	(((INPUT2 & 0x0100)==0x00)?OFF:ON)      //DI_8  
#define S26		(((INPUT2 & 0x0200)==0x00)?OFF:ON)      //DI_9
#define S27		(((INPUT2 & 0x0400)==0x00)?OFF:ON)      //DI_10
#define S28		(((INPUT2 & 0x0800)==0x00)?OFF:ON)      //DI_11
#define S29		(((INPUT2 & 0x1000)==0x00)?OFF:ON)      //DI_12
#define S30		(((INPUT2 & 0x2000)==0x00)?OFF:ON)      //DI_13
#define S31		(((INPUT2 & 0x4000)==0x00)?OFF:ON)      //DI_14
#define S32		(((INPUT2 & 0x8000)==0x00)?OFF:ON)      //DI_15

//INPUT3
#define INPUT3	inpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID3, (BYTE)0)

#define S33		(((INPUT3 & 0x0001)==0x00)?OFF:ON)		//DI_0
#define	S34		(((INPUT3 & 0x0002)==0x00)?OFF:ON)      //DI_1
#define	S35		(((INPUT3 & 0x0004)==0x00)?OFF:ON)      //DI_2
#define	S36 	(((INPUT3 & 0x0008)==0x00)?OFF:ON)	    //DI_3
#define	S37		(((INPUT3 & 0x0010)==0x00)?OFF:ON)      //DI_4
#define	S38		(((INPUT3 & 0x0020)==0x00)?OFF:ON)      //DI_5
#define S39		(((INPUT3 & 0x0040)==0x00)?OFF:ON)      //DI_6
#define S40		(((INPUT3 & 0x0080)==0x00)?OFF:ON)      //DI_7
#define S41 	(((INPUT3 & 0x0100)==0x00)?OFF:ON)      //DI_8  
#define S42		(((INPUT3 & 0x0200)==0x00)?OFF:ON)      //DI_9
#define S43		(((INPUT3 & 0x0400)==0x00)?OFF:ON)      //DI_10
#define S44		(((INPUT3 & 0x0800)==0x00)?OFF:ON)      //DI_11
#define S45		(((INPUT3 & 0x1000)==0x00)?OFF:ON)      //DI_12
#define S46		(((INPUT3 & 0x2000)==0x00)?OFF:ON)      //DI_13
#define S47		(((INPUT3 & 0x4000)==0x00)?OFF:ON)      //DI_14
#define S48		(((INPUT3 & 0x8000)==0x00)?OFF:ON)      //DI_15
//INPUT4
#define INPUT4	inpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID4, (BYTE)0)

#define S49		(((INPUT4 & 0x0001)==0x00)?OFF:ON)		//DI_0
#define	S50		(((INPUT4 & 0x0002)==0x00)?OFF:ON)      //DI_1
#define	S51		(((INPUT4 & 0x0004)==0x00)?OFF:ON)      //DI_2
#define	S52 	(((INPUT4 & 0x0008)==0x00)?OFF:ON)	    //DI_3
#define	S53		(((INPUT4 & 0x0010)==0x00)?OFF:ON)      //DI_4
#define	S54		(((INPUT4 & 0x0020)==0x00)?OFF:ON)      //DI_5
#define S55		(((INPUT4 & 0x0040)==0x00)?OFF:ON)      //DI_6
#define S56		(((INPUT4 & 0x0080)==0x00)?OFF:ON)      //DI_7
#define S57 	(((INPUT4 & 0x0100)==0x00)?OFF:ON)      //DI_8  
#define S58		(((INPUT4 & 0x0200)==0x00)?OFF:ON)      //DI_9
#define S59		(((INPUT4 & 0x0400)==0x00)?OFF:ON)      //DI_10
#define S60		(((INPUT4 & 0x0800)==0x00)?OFF:ON)      //DI_11
#define S61		(((INPUT4 & 0x1000)==0x00)?OFF:ON)      //DI_12
#define S62		(((INPUT4 & 0x2000)==0x00)?OFF:ON)      //DI_13
#define S63		(((INPUT4 & 0x4000)==0x00)?OFF:ON)      //DI_14
#define S64		(((INPUT4 & 0x8000)==0x00)?OFF:ON)      //DI_15
//INPUT5
#define INPUT5	inpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID5, (BYTE)0)

#define S65		(((INPUT5 & 0x0001)==0x00)?OFF:ON)		//DI_0
#define	S66		(((INPUT5 & 0x0002)==0x00)?OFF:ON)      //DI_1
#define	S67		(((INPUT5 & 0x0004)==0x00)?OFF:ON)      //DI_2
#define	S68 	(((INPUT5 & 0x0008)==0x00)?OFF:ON)	    //DI_3
#define	S69		(((INPUT5 & 0x0010)==0x00)?OFF:ON)      //DI_4
#define	S70		(((INPUT5 & 0x0020)==0x00)?OFF:ON)      //DI_5
#define S71		(((INPUT5 & 0x0040)==0x00)?OFF:ON)      //DI_6
#define S72		(((INPUT5 & 0x0080)==0x00)?OFF:ON)      //DI_7
#define S73 	(((INPUT5 & 0x0100)==0x00)?OFF:ON)      //DI_8  
#define S74		(((INPUT5 & 0x0200)==0x00)?OFF:ON)      //DI_9
#define S75		(((INPUT5 & 0x0400)==0x00)?OFF:ON)      //DI_10
#define S76		(((INPUT5 & 0x0800)==0x00)?OFF:ON)      //DI_11
#define S77		(((INPUT5 & 0x1000)==0x00)?OFF:ON)      //DI_12
#define S78		(((INPUT5 & 0x2000)==0x00)?OFF:ON)      //DI_13
#define S79		(((INPUT5 & 0x4000)==0x00)?OFF:ON)      //DI_14
#define S80		(((INPUT5 & 0x8000)==0x00)?OFF:ON)      //DI_15

//==========================================================================================
#define S01_Status		(((mshm.INPUT0_STATUS & 0x0001)==0x00)?OFF:ON)	
#define S02_Status		(((mshm.INPUT0_STATUS & 0x0002)==0x00)?OFF:ON)	
#define S03_Status		(((mshm.INPUT0_STATUS & 0x0004)==0x00)?OFF:ON)	
#define S04_Status		(((mshm.INPUT0_STATUS & 0x0008)==0x00)?OFF:ON)	
#define S05_Status		(((mshm.INPUT0_STATUS & 0x0010)==0x00)?OFF:ON)	
#define S06_Status		(((mshm.INPUT0_STATUS & 0x0020)==0x00)?OFF:ON)	
#define S07_Status		(((mshm.INPUT0_STATUS & 0x0040)==0x00)?OFF:ON)	
#define S08_Status		(((mshm.INPUT0_STATUS & 0x0080)==0x00)?OFF:ON)	
#define S09_Status		(((mshm.INPUT0_STATUS & 0x0100)==0x00)?OFF:ON)	
#define S10_Status		(((mshm.INPUT0_STATUS & 0x0200)==0x00)?OFF:ON)	
#define S11_Status		(((mshm.INPUT0_STATUS & 0x0400)==0x00)?OFF:ON)	
#define S12_Status		(((mshm.INPUT0_STATUS & 0x0800)==0x00)?OFF:ON)	
#define S13_Status		(((mshm.INPUT0_STATUS & 0x1000)==0x00)?OFF:ON)	
#define S14_Status		(((mshm.INPUT0_STATUS & 0x2000)==0x00)?OFF:ON)	
#define S15_Status		(((mshm.INPUT0_STATUS & 0x4000)==0x00)?OFF:ON)	
#define S16_Status		(((mshm.INPUT0_STATUS & 0x8000)==0x00)?OFF:ON)	
//==========================================================================================
#define S17_Status		(((mshm.INPUT1_STATUS & 0x0001)==0x00)?OFF:ON)	
#define S18_Status		(((mshm.INPUT1_STATUS & 0x0002)==0x00)?OFF:ON)	
#define S19_Status		(((mshm.INPUT1_STATUS & 0x0004)==0x00)?OFF:ON)	
#define S20_Status		(((mshm.INPUT1_STATUS & 0x0008)==0x00)?OFF:ON)	
#define S21_Status		(((mshm.INPUT1_STATUS & 0x0010)==0x00)?OFF:ON)	
#define S22_Status		(((mshm.INPUT1_STATUS & 0x0020)==0x00)?OFF:ON)	
#define S23_Status		(((mshm.INPUT1_STATUS & 0x0040)==0x00)?OFF:ON)	
#define S24_Status		(((mshm.INPUT1_STATUS & 0x0080)==0x00)?OFF:ON)	
#define S25_Status		(((mshm.INPUT1_STATUS & 0x0100)==0x00)?OFF:ON)	
#define S26_Status		(((mshm.INPUT1_STATUS & 0x0200)==0x00)?OFF:ON)	
#define S27_Status		(((mshm.INPUT1_STATUS & 0x0400)==0x00)?OFF:ON)	
#define S28_Status		(((mshm.INPUT1_STATUS & 0x0800)==0x00)?OFF:ON)	
#define S29_Status		(((mshm.INPUT1_STATUS & 0x1000)==0x00)?OFF:ON)	
#define S30_Status		(((mshm.INPUT1_STATUS & 0x2000)==0x00)?OFF:ON)	
#define S31_Status		(((mshm.INPUT1_STATUS & 0x4000)==0x00)?OFF:ON)	
#define S32_Status		(((mshm.INPUT1_STATUS & 0x8000)==0x00)?OFF:ON)
//==========================================================================================
#define S33_Status		(((mshm.INPUT2_STATUS & 0x0001)==0x00)?OFF:ON)	
#define S34_Status		(((mshm.INPUT2_STATUS & 0x0002)==0x00)?OFF:ON)	
#define S35_Status		(((mshm.INPUT2_STATUS & 0x0004)==0x00)?OFF:ON)	
#define S36_Status		(((mshm.INPUT2_STATUS & 0x0008)==0x00)?OFF:ON)	
#define S37_Status		(((mshm.INPUT2_STATUS & 0x0010)==0x00)?OFF:ON)	
#define S38_Status		(((mshm.INPUT2_STATUS & 0x0020)==0x00)?OFF:ON)	
#define S39_Status		(((mshm.INPUT2_STATUS & 0x0040)==0x00)?OFF:ON)	
#define S40_Status		(((mshm.INPUT2_STATUS & 0x0080)==0x00)?OFF:ON)	
#define S41_Status		(((mshm.INPUT2_STATUS & 0x0100)==0x00)?OFF:ON)	
#define S42_Status		(((mshm.INPUT2_STATUS & 0x0200)==0x00)?OFF:ON)	
#define S43_Status		(((mshm.INPUT2_STATUS & 0x0400)==0x00)?OFF:ON)	
#define S44_Status		(((mshm.INPUT2_STATUS & 0x0800)==0x00)?OFF:ON)	
#define S45_Status		(((mshm.INPUT2_STATUS & 0x1000)==0x00)?OFF:ON)	
#define S46_Status		(((mshm.INPUT2_STATUS & 0x2000)==0x00)?OFF:ON)	
#define S47_Status		(((mshm.INPUT2_STATUS & 0x4000)==0x00)?OFF:ON)	
#define S48_Status		(((mshm.INPUT2_STATUS & 0x8000)==0x00)?OFF:ON)
//==========================================================================================
#define S49_Status		(((mshm.INPUT3_STATUS & 0x0001)==0x00)?OFF:ON)	
#define S50_Status		(((mshm.INPUT3_STATUS & 0x0002)==0x00)?OFF:ON)	
#define S51_Status		(((mshm.INPUT3_STATUS & 0x0004)==0x00)?OFF:ON)	
#define S52_Status		(((mshm.INPUT3_STATUS & 0x0008)==0x00)?OFF:ON)	
#define S53_Status		(((mshm.INPUT3_STATUS & 0x0010)==0x00)?OFF:ON)	
#define S54_Status		(((mshm.INPUT3_STATUS & 0x0020)==0x00)?OFF:ON)	
#define S55_Status		(((mshm.INPUT3_STATUS & 0x0040)==0x00)?OFF:ON)	
#define S56_Status		(((mshm.INPUT3_STATUS & 0x0080)==0x00)?OFF:ON)	
#define S57_Status		(((mshm.INPUT3_STATUS & 0x0100)==0x00)?OFF:ON)	
#define S58_Status		(((mshm.INPUT3_STATUS & 0x0200)==0x00)?OFF:ON)	
#define S59_Status		(((mshm.INPUT3_STATUS & 0x0400)==0x00)?OFF:ON)	
#define S60_Status		(((mshm.INPUT3_STATUS & 0x0800)==0x00)?OFF:ON)	
#define S61_Status		(((mshm.INPUT3_STATUS & 0x1000)==0x00)?OFF:ON)	
#define S62_Status		(((mshm.INPUT3_STATUS & 0x2000)==0x00)?OFF:ON)	
#define S63_Status		(((mshm.INPUT3_STATUS & 0x4000)==0x00)?OFF:ON)	
#define S64_Status		(((mshm.INPUT3_STATUS & 0x8000)==0x00)?OFF:ON)
//==========================================================================================
#define S65_Status		(((mshm.INPUT4_STATUS & 0x0001)==0x00)?OFF:ON)	
#define S66_Status		(((mshm.INPUT4_STATUS & 0x0002)==0x00)?OFF:ON)	
#define S67_Status		(((mshm.INPUT4_STATUS & 0x0004)==0x00)?OFF:ON)	
#define S68_Status		(((mshm.INPUT4_STATUS & 0x0008)==0x00)?OFF:ON)	
#define S69_Status		(((mshm.INPUT4_STATUS & 0x0010)==0x00)?OFF:ON)	
#define S70_Status		(((mshm.INPUT4_STATUS & 0x0020)==0x00)?OFF:ON)	
#define S71_Status		(((mshm.INPUT4_STATUS & 0x0040)==0x00)?OFF:ON)	
#define S72_Status		(((mshm.INPUT4_STATUS & 0x0080)==0x00)?OFF:ON)	
#define S73_Status		(((mshm.INPUT4_STATUS & 0x0100)==0x00)?OFF:ON)	
#define S74_Status		(((mshm.INPUT4_STATUS & 0x0200)==0x00)?OFF:ON)	
#define S75_Status		(((mshm.INPUT4_STATUS & 0x0400)==0x00)?OFF:ON)	
#define S76_Status		(((mshm.INPUT4_STATUS & 0x0800)==0x00)?OFF:ON)	
#define S77_Status		(((mshm.INPUT4_STATUS & 0x1000)==0x00)?OFF:ON)	
#define S78_Status		(((mshm.INPUT4_STATUS & 0x2000)==0x00)?OFF:ON)	
#define S79_Status		(((mshm.INPUT4_STATUS & 0x4000)==0x00)?OFF:ON)	
#define S80_Status		(((mshm.INPUT4_STATUS & 0x8000)==0x00)?OFF:ON)
// DEFINE INPUT
#define  SECOND_CYLINDER_DOWN_SENSOR							  S01
#define  SECOND_CYLINDER_UP_SENSOR								  S02
#define  THIRD_CYLINDER_DOWN_SENSOR								  S03
#define  THIRD_CYLINDER_UP_SENSOR								  S04
#define	 FOURTH_CYLINDER_DOWN_SENSOR							  S05	
#define  FOURTH_CYLINDER_UP_SENSOR								  S06
#define  FIRST_CYLINDER_DOWN_SENSOR								  S07
#define  FIRST_CYLINDER_UP_SENSOR								  S08

#define  FEED_LEFT_GRIPPER_OPEN_SENSOR							  S09
#define  FEED_LEFT_GRIPPER_CLOSE_SENSOR							  S10

#define  FEED_RIGHT_GRIPPER_OPEN_SENSOR							  S11
#define  FEED_RIGHT_GRIPPER_CLOSE_SENSOR						  S12

#define  DARK_BOX_UP_SENSOR										  S13
#define  DARK_BOX_DOWN_SENSOR									  S14
#define  LIGHT_BOX_UP_SENSOR									  S15
#define  LIGHT_BOX_DOWN_SENSOR									  S16

#define  FEED_CYLINDER_LEFT_SENSOR								  S17
#define  FEED_CYLINDER_RIGHT_SENSOR								  S18

#define  FEED_GRIPPER_CYLINDER_LEFT_SENSOR						  S19
#define  FEED_GRIPPER_CYLINDER_RIGHT_SENSOR						  S20

#define  FEED_GRIPPER_UP_SENSOR									  S21
#define  FEED_GRIPPER_DOWN_SENSOR								  S22

#define  PRODUCT_CYLINDER_LEFT_SENSOR							  S23
#define  PRODUCT_CYLINDER_RIGHT_SENSOR							  S24

#define  PRODUCT_GRIPPER_OPEN_SENSOR							  S25
#define  PRODUCT_GRIPPER_CLOSE_SENSOR							  S26

#define  PRODUCT_GRIPPER_UP_SENSOR								  S33
#define  PRODUCT_GRIPPER_DOWN_SENSOR							  S34

#define  NG_TRAY_CYLINDER_FORWARD_SENSOR						  S39
#define  NG_TRAY_CYLINDER_REVERSE_SENSOR						  S40	

#define  STACK_CYLINDER_UP_SENSOR								  S46
#define  STACK_CYLINDER_DOWN_SENSOR								  S45

#define  STACK_BLOCK_LEFT_OPEN_SENSOR							  S41
#define  STACK_BLOCK_LEFT_CLOSE_SENSOR							  S42
#define  STACK_BLOCK_RIGHT_OPEN_SENSOR							  S43
#define  STACK_BLOCK_RIGHT_CLOSE_SENSOR							  S44

#define  EMERGENCY_STOP_SENSOR									  S59
#define  EQUIPMENT_START_SENSOR									  S60
#define  STOP_SENSOR											  S61
#define  RESET_SENSOR											  S62

#define  PRODUCT_TRAY_UP_SENSOR									  S72
#define  PRODUCT_TRAY_DOWN_SENSOR								  S71

 
//=== OUTPUT1 ==============================================================================
#define OUTPUT1			outpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID1, (BYTE)0, (WORD)(mshm.OUTPUT0_STATUS

#define	Y01_ON			OUTPUT1 |= 0x0001))	
#define	Y01_OFF			OUTPUT1 &= 0xfffe))    

#define Y02_ON			OUTPUT1 |= 0x0002))	
#define Y02_OFF			OUTPUT1 &= 0xfffd))

#define Y03_ON			OUTPUT1 |= 0x0004))	
#define Y03_OFF			OUTPUT1 &= 0xfffb))

#define Y04_ON			OUTPUT1 |= 0x0008))	
#define Y04_OFF			OUTPUT1 &= 0xfff7))

#define Y05_ON			OUTPUT1 |= 0x0010))	
#define Y05_OFF			OUTPUT1 &= 0xffef))

#define Y06_ON			OUTPUT1 |= 0x0020))	
#define Y06_OFF			OUTPUT1 &= 0xffdf))

#define Y07_ON			OUTPUT1 |= 0x0040))	
#define Y07_OFF			OUTPUT1 &= 0xffbf))

#define Y08_ON			OUTPUT1 |= 0x0080))	
#define Y08_OFF			OUTPUT1 &= 0xff7f))

#define	Y09_ON			OUTPUT1 |= 0x0100))	
#define	Y09_OFF			OUTPUT1 &= 0xfeff)) 

#define	Y10_ON			OUTPUT1 |= 0x0200))	
#define	Y10_OFF			OUTPUT1 &= 0xfdff)) 

#define	Y11_ON			OUTPUT1 |= 0x0400))	
#define	Y11_OFF			OUTPUT1 &= 0xfbff)) 

#define	Y12_ON			OUTPUT1 |= 0x0800))	
#define	Y12_OFF			OUTPUT1 &= 0xf7ff)) 

#define	Y13_ON			OUTPUT1 |= 0x1000))	
#define	Y13_OFF			OUTPUT1 &= 0xefff)) 

#define	Y14_ON			OUTPUT1 |= 0x2000))	
#define	Y14_OFF			OUTPUT1 &= 0xdfff)) 

#define	Y15_ON			OUTPUT1 |= 0x4000))	
#define	Y15_OFF			OUTPUT1 &= 0xbfff))	

#define	Y16_ON			OUTPUT1 |= 0x8000))	
#define	Y16_OFF			OUTPUT1 &= 0x7fff))	
//=== OUTPUT2 ==============================================================================
#define OUTPUT2			outpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID2, (BYTE)0, (WORD)(mshm.OUTPUT1_STATUS

#define	Y17_ON			OUTPUT2 |= 0x0001))	
#define	Y17_OFF			OUTPUT2 &= 0xfffe))    

#define Y18_ON			OUTPUT2 |= 0x0002))	
#define Y18_OFF			OUTPUT2 &= 0xfffd))

#define Y19_ON			OUTPUT2 |= 0x0004))	
#define Y19_OFF			OUTPUT2 &= 0xfffb))

#define Y20_ON			OUTPUT2 |= 0x0008))	
#define Y20_OFF			OUTPUT2 &= 0xfff7))

#define Y21_ON			OUTPUT2 |= 0x0010))	
#define Y21_OFF			OUTPUT2 &= 0xffef))

#define Y22_ON			OUTPUT2 |= 0x0020))	
#define Y22_OFF			OUTPUT2 &= 0xffdf))

#define Y23_ON			OUTPUT2 |= 0x0040))	
#define Y23_OFF			OUTPUT2 &= 0xffbf))

#define Y24_ON			OUTPUT2 |= 0x0080))	
#define Y24_OFF			OUTPUT2 &= 0xff7f))

#define	Y25_ON			OUTPUT2 |= 0x0100))	
#define	Y25_OFF			OUTPUT2 &= 0xfeff)) 

#define	Y26_ON			OUTPUT2 |= 0x0200))	
#define	Y26_OFF			OUTPUT2 &= 0xfdff)) 

#define	Y27_ON			OUTPUT2 |= 0x0400))	
#define	Y27_OFF			OUTPUT2 &= 0xfbff)) 

#define	Y28_ON			OUTPUT2 |= 0x0800))	
#define	Y28_OFF			OUTPUT2 &= 0xf7ff)) 

#define	Y29_ON			OUTPUT2 |= 0x1000))	
#define	Y29_OFF			OUTPUT2 &= 0xefff)) 

#define	Y30_ON			OUTPUT2 |= 0x2000))	
#define	Y30_OFF			OUTPUT2 &= 0xdfff)) 

#define	Y31_ON			OUTPUT2 |= 0x4000))	
#define	Y31_OFF			OUTPUT2 &= 0xbfff))	

#define	Y32_ON			OUTPUT2 |= 0x8000))	
#define	Y32_OFF			OUTPUT2 &= 0x7fff))	
//=== OUTPUT3 ==============================================================================
#define OUTPUT3			outpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID3, (BYTE)0, (WORD)(mshm.OUTPUT2_STATUS

#define	Y33_ON			OUTPUT3 |= 0x0001))	
#define	Y33_OFF			OUTPUT3 &= 0xfffe))    

#define Y34_ON			OUTPUT3 |= 0x0002))	
#define Y34_OFF			OUTPUT3 &= 0xfffd))

#define Y35_ON			OUTPUT3 |= 0x0004))	
#define Y35_OFF			OUTPUT3 &= 0xfffb))

#define Y36_ON			OUTPUT3 |= 0x0008))	
#define Y36_OFF			OUTPUT3 &= 0xfff7))

#define Y37_ON			OUTPUT3 |= 0x0010))	
#define Y37_OFF			OUTPUT3 &= 0xffef))

#define Y38_ON			OUTPUT3 |= 0x0020))	
#define Y38_OFF			OUTPUT3 &= 0xffdf))

#define Y39_ON			OUTPUT3 |= 0x0040))	
#define Y39_OFF			OUTPUT3 &= 0xffbf))

#define Y40_ON			OUTPUT3 |= 0x0080))	
#define Y40_OFF			OUTPUT3 &= 0xff7f))

#define	Y41_ON			OUTPUT3 |= 0x0100))	
#define	Y41_OFF			OUTPUT3 &= 0xfeff)) 

#define	Y42_ON			OUTPUT3 |= 0x0200))	
#define	Y42_OFF			OUTPUT3 &= 0xfdff)) 

#define	Y43_ON			OUTPUT3 |= 0x0400))	
#define	Y43_OFF			OUTPUT3 &= 0xfbff)) 

#define	Y44_ON			OUTPUT3 |= 0x0800))	
#define	Y44_OFF			OUTPUT3 &= 0xf7ff)) 

#define	Y45_ON			OUTPUT3 |= 0x1000))	
#define	Y45_OFF			OUTPUT3 &= 0xefff)) 

#define	Y46_ON			OUTPUT3 |= 0x2000))	
#define	Y46_OFF			OUTPUT3 &= 0xdfff)) 

#define	Y47_ON			OUTPUT3 |= 0x4000))	
#define	Y47_OFF			OUTPUT3 &= 0xbfff))	

#define	Y48_ON			OUTPUT3 |= 0x8000))	
#define	Y48_OFF			OUTPUT3 &= 0x7fff))	
//==OUTPUT4=====================================================================================
#define OUTPUT4			outpw((BYTE)LINE_NO_0, (BYTE)DEVICE_IO_ID4, (BYTE)0, (WORD)(mshm.OUTPUT3_STATUS

#define	Y49_ON			OUTPUT4 |= 0x0001))	
#define	Y49_OFF			OUTPUT4 &= 0xfffe))    

#define Y50_ON			OUTPUT4 |= 0x0002))	
#define Y50_OFF			OUTPUT4 &= 0xfffd))

#define Y51_ON			OUTPUT4 |= 0x0004))	
#define Y51_OFF			OUTPUT4 &= 0xfffb))

#define Y52_ON			OUTPUT4 |= 0x0008))	
#define Y52_OFF			OUTPUT4 &= 0xfff7))

#define Y53_ON			OUTPUT4 |= 0x0010))	
#define Y53_OFF			OUTPUT4 &= 0xffef))

#define Y54_ON			OUTPUT4 |= 0x0020))	
#define Y54_OFF			OUTPUT4 &= 0xffdf))

#define Y55_ON			OUTPUT4 |= 0x0040))	
#define Y55_OFF			OUTPUT4 &= 0xffbf))

#define Y56_ON			OUTPUT4 |= 0x0080))	
#define Y56_OFF			OUTPUT4 &= 0xff7f))

#define	Y57_ON			OUTPUT4 |= 0x0100))	
#define	Y57_OFF			OUTPUT4 &= 0xfeff)) 

#define	Y58_ON			OUTPUT4 |= 0x0200))	
#define	Y58_OFF			OUTPUT4 &= 0xfdff)) 

#define	Y59_ON			OUTPUT4 |= 0x0400))	
#define	Y59_OFF			OUTPUT4 &= 0xfbff)) 

#define	Y60_ON			OUTPUT4 |= 0x0800))	
#define	Y60_OFF			OUTPUT4 &= 0xf7ff)) 

#define	Y61_ON			OUTPUT4 |= 0x1000))	
#define	Y61_OFF			OUTPUT4 &= 0xefff)) 

#define	Y62_ON			OUTPUT4 |= 0x2000))	
#define	Y62_OFF			OUTPUT4 &= 0xdfff)) 

#define	Y63_ON			OUTPUT4 |= 0x4000))	
#define	Y63_OFF			OUTPUT4 &= 0xbfff))	

#define	Y64_ON			OUTPUT4 |= 0x8000))	
#define	Y64_OFF			OUTPUT4 &= 0x7fff))	
//========================================================================================
// DEFINE OUTPUT CHECK

#define Y01_CHECK		(((mshm.OUTPUT0_STATUS & 0x0001)==0x0001) ? ON:OFF)
#define Y02_CHECK		(((mshm.OUTPUT0_STATUS & 0x0002)==0x0002) ? ON:OFF)
#define Y03_CHECK		(((mshm.OUTPUT0_STATUS & 0x0004)==0x0004) ? ON:OFF)
#define Y04_CHECK		(((mshm.OUTPUT0_STATUS & 0x0008)==0x0008) ? ON:OFF)
#define Y05_CHECK		(((mshm.OUTPUT0_STATUS & 0x0010)==0x0010) ? ON:OFF)
#define Y06_CHECK		(((mshm.OUTPUT0_STATUS & 0x0020)==0x0020) ? ON:OFF)
#define Y07_CHECK		(((mshm.OUTPUT0_STATUS & 0x0040)==0x0040) ? ON:OFF)
#define Y08_CHECK		(((mshm.OUTPUT0_STATUS & 0x0080)==0x0080) ? ON:OFF)
#define Y09_CHECK		(((mshm.OUTPUT0_STATUS & 0x0100)==0x0100) ? ON:OFF)
#define Y10_CHECK		(((mshm.OUTPUT0_STATUS & 0x0200)==0x0200) ? ON:OFF)
#define Y11_CHECK		(((mshm.OUTPUT0_STATUS & 0x0400)==0x0400) ? ON:OFF)
#define Y12_CHECK		(((mshm.OUTPUT0_STATUS & 0x0800)==0x0800) ? ON:OFF)
#define Y13_CHECK		(((mshm.OUTPUT0_STATUS & 0x1000)==0x1000) ? ON:OFF)
#define Y14_CHECK		(((mshm.OUTPUT0_STATUS & 0x2000)==0x2000) ? ON:OFF)
#define Y15_CHECK		(((mshm.OUTPUT0_STATUS & 0x4000)==0x4000) ? ON:OFF)
#define Y16_CHECK		(((mshm.OUTPUT0_STATUS & 0x8000)==0x8000) ? ON:OFF)
//==========================================================================================

#define Y17_CHECK		(((mshm.OUTPUT1_STATUS & 0x0001)==0x0001) ? ON:OFF)
#define Y18_CHECK		(((mshm.OUTPUT1_STATUS & 0x0002)==0x0002) ? ON:OFF)
#define Y19_CHECK		(((mshm.OUTPUT1_STATUS & 0x0004)==0x0004) ? ON:OFF)
#define Y20_CHECK		(((mshm.OUTPUT1_STATUS & 0x0008)==0x0008) ? ON:OFF)
#define Y21_CHECK		(((mshm.OUTPUT1_STATUS & 0x0010)==0x0010) ? ON:OFF)
#define Y22_CHECK		(((mshm.OUTPUT1_STATUS & 0x0020)==0x0020) ? ON:OFF)
#define Y23_CHECK		(((mshm.OUTPUT1_STATUS & 0x0040)==0x0040) ? ON:OFF)
#define Y24_CHECK		(((mshm.OUTPUT1_STATUS & 0x0080)==0x0080) ? ON:OFF)
#define Y25_CHECK		(((mshm.OUTPUT1_STATUS & 0x0100)==0x0100) ? ON:OFF)
#define Y26_CHECK		(((mshm.OUTPUT1_STATUS & 0x0200)==0x0200) ? ON:OFF)
#define Y27_CHECK		(((mshm.OUTPUT1_STATUS & 0x0400)==0x0400) ? ON:OFF)
#define Y28_CHECK		(((mshm.OUTPUT1_STATUS & 0x0800)==0x0800) ? ON:OFF)
#define Y29_CHECK		(((mshm.OUTPUT1_STATUS & 0x1000)==0x1000) ? ON:OFF)
#define Y30_CHECK		(((mshm.OUTPUT1_STATUS & 0x2000)==0x2000) ? ON:OFF)
#define Y31_CHECK		(((mshm.OUTPUT1_STATUS & 0x4000)==0x4000) ? ON:OFF)
#define Y32_CHECK		(((mshm.OUTPUT1_STATUS & 0x8000)==0x8000) ? ON:OFF)
//==========================================================================================
#define Y33_CHECK		(((mshm.OUTPUT2_STATUS & 0x0001)==0x0001) ? ON:OFF)
#define Y34_CHECK		(((mshm.OUTPUT2_STATUS & 0x0002)==0x0002) ? ON:OFF)
#define Y35_CHECK		(((mshm.OUTPUT2_STATUS & 0x0004)==0x0004) ? ON:OFF)
#define Y36_CHECK		(((mshm.OUTPUT2_STATUS & 0x0008)==0x0008) ? ON:OFF)
#define Y37_CHECK		(((mshm.OUTPUT2_STATUS & 0x0010)==0x0010) ? ON:OFF)
#define Y38_CHECK		(((mshm.OUTPUT2_STATUS & 0x0020)==0x0020) ? ON:OFF)
#define Y39_CHECK		(((mshm.OUTPUT2_STATUS & 0x0040)==0x0040) ? ON:OFF)
#define Y40_CHECK		(((mshm.OUTPUT2_STATUS & 0x0080)==0x0080) ? ON:OFF)
#define Y41_CHECK		(((mshm.OUTPUT2_STATUS & 0x0100)==0x0100) ? ON:OFF)
#define Y42_CHECK		(((mshm.OUTPUT2_STATUS & 0x0200)==0x0200) ? ON:OFF)
#define Y43_CHECK		(((mshm.OUTPUT2_STATUS & 0x0400)==0x0400) ? ON:OFF)
#define Y44_CHECK		(((mshm.OUTPUT2_STATUS & 0x0800)==0x0800) ? ON:OFF)
#define Y45_CHECK		(((mshm.OUTPUT2_STATUS & 0x1000)==0x1000) ? ON:OFF)
#define Y46_CHECK		(((mshm.OUTPUT2_STATUS & 0x2000)==0x2000) ? ON:OFF)
#define Y47_CHECK		(((mshm.OUTPUT2_STATUS & 0x4000)==0x4000) ? ON:OFF)
#define Y48_CHECK		(((mshm.OUTPUT2_STATUS & 0x8000)==0x8000) ? ON:OFF)
//==========================================================================================

#define Y49_CHECK		(((mshm.OUTPUT3_STATUS & 0x0001)==0x0001) ? ON:OFF)
#define Y50_CHECK		(((mshm.OUTPUT3_STATUS & 0x0002)==0x0002) ? ON:OFF)
#define Y51_CHECK		(((mshm.OUTPUT3_STATUS & 0x0004)==0x0004) ? ON:OFF)
#define Y52_CHECK		(((mshm.OUTPUT3_STATUS & 0x0008)==0x0008) ? ON:OFF)
#define Y53_CHECK		(((mshm.OUTPUT3_STATUS & 0x0010)==0x0010) ? ON:OFF)
#define Y54_CHECK		(((mshm.OUTPUT3_STATUS & 0x0020)==0x0020) ? ON:OFF)
#define Y55_CHECK		(((mshm.OUTPUT3_STATUS & 0x0040)==0x0040) ? ON:OFF)
#define Y56_CHECK		(((mshm.OUTPUT3_STATUS & 0x0080)==0x0080) ? ON:OFF)
#define Y57_CHECK		(((mshm.OUTPUT3_STATUS & 0x0100)==0x0100) ? ON:OFF)
#define Y58_CHECK		(((mshm.OUTPUT3_STATUS & 0x0200)==0x0200) ? ON:OFF)
#define Y59_CHECK		(((mshm.OUTPUT3_STATUS & 0x0400)==0x0400) ? ON:OFF)
#define Y60_CHECK		(((mshm.OUTPUT3_STATUS & 0x0800)==0x0800) ? ON:OFF)
#define Y61_CHECK		(((mshm.OUTPUT3_STATUS & 0x1000)==0x1000) ? ON:OFF)
#define Y62_CHECK		(((mshm.OUTPUT3_STATUS & 0x2000)==0x2000) ? ON:OFF)
#define Y63_CHECK		(((mshm.OUTPUT3_STATUS & 0x4000)==0x4000) ? ON:OFF)
#define Y64_CHECK		(((mshm.OUTPUT3_STATUS & 0x8000)==0x8000) ? ON:OFF)



/*
typedef struct _tag_Dlg
{
	HWND handler;
	DLGPROC process;
	int idd;
	int idc;
}Dlg_Info;*/

#define ID_IO_IN					0
#define ID_IO_OUT					1



#define FEED_CYLINDER_LEFT_ON						  Y01_ON
#define FEED_CYLINDER_LEFT_OFF						  Y01_OFF

#define FEED_CYLINDER_RIGHT_ON						  Y02_ON
#define FEED_CYLINDER_RIGHT_OFF						  Y02_OFF

#define FEED_GRIPPER_CYLINDER_OPEN_ON				  Y03_ON
#define FEED_GRIPPER_CYLINDER_OPEN_OFF				  Y03_OFF

#define FEED_GRIPPER_CYLINDER_CLOSE_ON				  Y04_ON
#define FEED_GRIPPER_CYLINDER_CLOSE_OFF				  Y04_OFF

#define FEED_GRIPPER_MOVE_LEFT_ON					  Y05_ON
#define FEED_GRIPPER_MOVE_LEFT_OFF					  Y05_OFF

#define FEED_GRIPPER_MOVE_RIGHT_ON					  Y06_ON
#define FEED_GRIPPER_MOVE_RIGHT_OFF					  Y06_OFF

#define PRODUCT_GRIPPER_OPEN_ON						  Y07_ON
#define PRODUCT_GRIPPER_OPEN_OFF					  Y07_OFF

#define PRODUCT_GRIPPER_CLOSE_ON					  Y08_ON
#define PRODUCT_GRIPPER_CLOSE_OFF					  Y08_OFF

#define NG_TRAY_FORWARD_ON							  Y09_ON
#define NG_TRAY_FORWARD_OFF							  Y09_OFF
#define NG_TRAY_REVERSE_ON							  Y10_ON
#define NG_TRAY_REVERSE_OFF							  Y10_OFF

#define PRODUCT_TRAY_DOWN_ON						  Y11_ON
#define PRODUCT_TRAY_DOWN_OFF						  Y11_OFF
#define PRODUCT_TRAY_UP_ON						      Y12_ON
#define PRODUCT_TRAY_UP_OFF						      Y12_OFF

#define SECOND_CYLINDER_UP							  Y13_ON
#define SECOND_CYLINDER_DOWN						  Y13_OFF
#define THIRD_CYLINDER_UP							  Y14_ON
#define THIRD_CYLINDER_DOWN							  Y14_OFF
#define FOURTH_CYLINDER_UP							  Y15_ON
#define FOURTH_CYLINDER_DOWN						  Y15_OFF
#define FIRST_CYLINDER_UP							  Y16_ON
#define FIRST_CYLINDER_DOWN							  Y16_OFF

#define DARK_BOX_DOWN								  Y17_ON
#define DARK_BOX_UP									  Y17_OFF
#define LIGHT_BOX_DOWN								  Y18_ON
#define LIGHT_BOX_UP								  Y18_OFF

#define PRODUCT_CYLINDER_RIGHT						  Y19_ON
#define PRODUCT_CYLINDER_LEFT						  Y19_OFF

#define PRODUCT_GRIPPER_DOWN						  Y20_ON
#define PRODUCT_GRIPPER_UP							  Y20_OFF

#define STACK_BLOCK_CLOSE							  Y21_ON
#define STACK_BLOCK_OPEN							  Y21_OFF

#define STACK_CYLINDER_UP							  Y22_ON
#define STACK_CYLINDER_DOWN							  Y22_OFF

#define ALL_CYLINDER_OPEN							  Y24_ON
#define ALL_CYLINDER_CLOSE                            Y24_OFF


#define EQUIPMENT_LED_START							  Y25_ON
#define EQUIPMENT_LED_END							  Y25_OFF
													  
#define COMPUTER_LED_START							  Y26_ON
#define COMPUTER_LED_END							  Y26_OFF

#define RESET_LED_START								  Y27_ON
#define RESET_LED_END								  Y27_OFF

#define RED_LED_START								  Y28_ON
#define RED_LED_END									  Y28_OFF

#define YELLOW_LED_START							  Y29_ON
#define YELLOW_LED_END								  Y29_OFF

#define GREEN_LED_START								  Y30_ON
#define GREEN_LED_END								  Y30_OFF

#define BUZZER_OPEN									  Y31_ON
#define BUZZER_CLOSE								  Y31_OFF

#define FEED_GRIPPER_UP_ON							  Y41_ON
#define FEED_GRIPPER_UP_OFF							  Y41_OFF

#define FEED_GRIPPER_DOWN_ON						  Y42_ON
#define FEED_GRIPPER_DOWN_OFF						  Y42_OFF
