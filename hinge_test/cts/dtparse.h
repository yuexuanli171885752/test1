#ifndef __DTPARSE_H__
#define __DTPARSE_H__

#include<atlstr.h>
#include<vector>
using namespace std;

#include "cts\cts.h"


/**************************************************************************************** 
*
* SENSOR�ĳ�ʼ����������
*
****************************************************************************************/

typedef struct _SensorTab
{
	/// @brief SENSOR���
	USHORT width;          ///<SENSOR���
	/// @brief SENSOR�߶�
	USHORT height;         ///<SENSOR�߶�
	/// @brief SENSOR��������
	BYTE type;             ///<SENSOR��������
	/// @brief SENSOR��RESET��PWDN��������
	BYTE pin;              ///<SENSOR��RESET��PWDN��������
	/// @brief SENSOR��������ַ
	BYTE SlaveID;          ///<SENSOR��������ַ
	/// @brief SENSOR��I2Cģʽ
	BYTE mode;						 ///<SENSOR��I2Cģʽ
	/// @brief SENSOR��־�Ĵ���1.
	USHORT FlagReg;				 ///<SENSOR��־�Ĵ���1.
	/// @brief SENSOR��־�Ĵ���1��ֵ
	USHORT FlagData;			 ///<SENSOR��־�Ĵ���1��ֵ
	/// @brief SENSOR��־�Ĵ���1������ֵ
	USHORT FlagMask;			 ///<SENSOR��־�Ĵ���1������ֵ
	/// @brief SENSOR��־�Ĵ���2.
	USHORT FlagReg1;			 ///<SENSOR��־�Ĵ���2.
	/// @brief SENSOR��־�Ĵ���2��ֵ
	USHORT FlagData1;			 ///<SENSOR��־�Ĵ���2��ֵ
	/// @brief SENSOR��־�Ĵ���2������ֵ
	USHORT FlagMask1;			 ///<SENSOR��־�Ĵ���2������ֵ
	/// @brief SENSOR������
	char name[64];				///<SENSOR������

	/// @brief ��ʼ��SENSOR���ݱ�
	USHORT* ParaList;			///<��ʼ��SENSOR���ݱ�
	/// @brief ��ʼ��SENSOR���ݱ��С����λ�ֽ�
	USHORT  ParaListSize; ///<��ʼ��SENSOR���ݱ��С����λ�ֽ�

	/// @brief SENSOR����Sleepģʽ�Ĳ�����
	USHORT* SleepParaList;	///<SENSOR����Sleepģʽ�Ĳ�����
	/// @brief SENSOR����Sleepģʽ�Ĳ������С����λ�ֽ�
	USHORT  SleepParaListSize;///<SENSOR����Sleepģʽ�Ĳ������С����λ�ֽ�

	/// @brief SENSOR������ݸ�ʽ��YUV//0:YCbYCr;	//1:YCrYCb;	//2:CbYCrY;	//3:CrYCbY.
	BYTE outformat;         ///<SENSOR������ݸ�ʽ��YUV//0:YCbYCr;	//1:YCrYCb;	//2:CbYCrY;	//3:CrYCbY.
	/// @brief SENSOR������ʱ��MCLK��0:12M; 1:24M; 2:48M.
	int mclk;               ///<SENSOR������ʱ��MCLK��0:12M; 1:24M; 2:48M.
	/// @brief SENSOR��AVDD��ѹֵ
	BYTE avdd;              ///<SENSOR��AVDD��ѹֵ
	/// @brief SENSOR��DOVDD��ѹֵ
	BYTE dovdd;             ///<SENSOR��DOVDD��ѹֵ
	/// @brief SENSOR��DVDD��ѹֵ		
	BYTE dvdd;							///<SENSOR��DVDD��ѹֵ

	BYTE afvcc;			//add by cts for o-film			
	
	/// @brief SENSOR�����ݽӿ�����
	BYTE port; 							///<SENSOR�����ݽӿ�����
	USHORT Ext0;
	USHORT Ext1;
	USHORT Ext2; 

	/// @brief AF��ʼ��������
	USHORT* AF_InitParaList;        ///<AF��ʼ��������
	/// @brief AF��ʼ���������С����λ�ֽ�
	USHORT  AF_InitParaListSize;		///<AF��ʼ���������С����λ�ֽ�

	/// @brief AF_AUTO������
	USHORT* AF_AutoParaList;				///<AF_AUTO������
	/// @brief AF_AUTO�������С����λ�ֽ�
	USHORT  AF_AutoParaListSize;		///<AF_AUTO�������С����λ�ֽ�

	/// @brief AF_FAR������
	USHORT* AF_FarParaList;					///<AF_FAR������
	/// @brief AF_FAR�������С����λ�ֽ�
	USHORT  AF_FarParaListSize;			///<AF_FAR�������С����λ�ֽ�

	/// @brief AF_NEAR������
	USHORT* AF_NearParaList;				///<AF_NEAR������
	/// @brief AF_NEAR�������С����λ�ֽ�
	USHORT  AF_NearParaListSize;		///<AF_NEAR�������С����λ�ֽ�

	/// @brief �ع������
	USHORT* Exposure_ParaList;      ///<�ع������
	/// @brief �ع�������С����λ�ֽ�
	USHORT  Exposure_ParaListSize;	///<�ع�������С����λ�ֽ�

	/// @brief ���������
	USHORT* Gain_ParaList;          ///<���������
	/// @brief ����������С����λ�ֽ�
	USHORT	Gain_ParaListSize;			///<����������С����λ�ֽ� 

	//cts parameters
	DWORD OPERATION_SEQUENCE;
	BYTE SEQUENCE_DELAY;
	float SENSOR_IO_LEVEL;
	BYTE I2C_SPEED;
	float SENSOR_FRAME_RATE;
	BYTE USB3_BANDWIDTH;
	
	BYTE DATA_PATH;
	BYTE SENSOR_OUTPUT_FORMAT;
	BYTE MIPI_LANE_NUMBER;
	BYTE MIPI_T_hs_zero_MASK;

	_SensorTab()
	{
		width=0;
		height=0;
		type=0;
		pin=0;
		SlaveID=0;
		mode=0;
		FlagReg=0;
		FlagData=0;
		FlagMask=0;
		FlagReg1=0;
		FlagData1=0;
		FlagMask1=0;
		memset(name,0,sizeof(name));

		ParaList=NULL;
		ParaListSize=0;
		SleepParaList=NULL;
		SleepParaListSize=0;

		outformat=0;
		mclk=0;               //0:12M; 1:24M; 2:48M.
		avdd=0;               // 
		dovdd=0;              //
		dvdd=0;

		afvcc = 0;

		port=0; 	
		Ext0=0;
		Ext1=0;
		Ext2=0; 

		AF_InitParaList=NULL;        //AF_InitParaList
		AF_InitParaListSize=0;

		AF_AutoParaList=NULL;
		AF_AutoParaListSize=0;

		AF_FarParaList=NULL;
		AF_FarParaListSize=0;

		AF_NearParaList=NULL;
		AF_NearParaListSize=0;

		Exposure_ParaList=NULL;      //�ع�
		Exposure_ParaListSize=0;

		Gain_ParaList=NULL;          //����
		Gain_ParaListSize=0;

		//cts parameters
		OPERATION_SEQUENCE = 0;
		SEQUENCE_DELAY = 0;
		SENSOR_IO_LEVEL = 0;
		I2C_SPEED = 0;
		SENSOR_FRAME_RATE = 0;
		USB3_BANDWIDTH = 0;
		
		DATA_PATH = 0;
		SENSOR_OUTPUT_FORMAT = 0;
		MIPI_LANE_NUMBER = 0;
		MIPI_T_hs_zero_MASK = 0;
	}
}SensorTab, *pSensorTab;


typedef struct _CTSSensorTab
{
	float DVDD; // pwr0, (LDO, 0.7~3.5V, 0.025V/step, 200mA driving ability)
	float AVDD; // pwr1, (Low noise LDO, 1.8~3.3V, 0.1V/step, 200mA driving ability)
	float DOVDD; // pwr2, (Low noise LDO, 1.25 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.5 2.7 2.8 3.0 3.1 3.2 3.3V, 200mA driving ability)
	float AFVDD; // pwr3, (DC-DC power, 0.7~3.5V, 0.025V/step, 500mA driving ability)
	float PWR4; // pwr4, (DC-DC power, 0.7~2.275V, 0.025V/step, 500mA driving ability)
	float PWR5; // pwr5, (PMOS power switch, 0:0V, 5:5V, 900mA driving ability)

	BYTE SENSOR_POWERDOWN_ACTIVE; // 0:low active, 1:high active
	BYTE SENSOR_RESET_ACTIVE; // 0:low active, 1:high active
	DWORD OPERATION_SEQUENCE; //The Number sequence stands the sequence of the upper operation prefix, and the "F" stands for any disabled power 
	BYTE SEQUENCE_DELAY; //(0~255mS), delay time between HCLK enable,OPERATION_SEQUENCE and sensor I2C initiate
	float SENSOR_IO_LEVEL; //(1.25~3.3), Volts
	BYTE I2C_SPEED; //0:100Kbps, 1:400Kbps(default), 2:1Mbps, 3:200Kbps, 4:3.4Mbps
	BYTE SENSOR_I2C_ID; //sensor I2C slave address
	BYTE SENSOR_REG_ADDR_WIDTH; //(byte), sensor register address width
	BYTE SENSOR_REG_DATA_WIDTH; //(byte), sensor register value width
	float SENSOR_FRAME_RATE; //(FPS), it better be integer, use for usb bandwidth
	float SENSOR_EXTCLK; //(MHz), 1.5MHz~80MHz, 0.25MHz/step, 0.001MHz accuracy
	WORD SENSOR_OUTPUT_WIDTH; //total horizontal effective pixels size    
	WORD SENSOR_OUTPUT_HEIGHT; //total vertical effective pixels size
	
	BYTE USB3_BANDWIDTH; //0:auto select(default), 1:64MB/S, 2:128MB/S, 3:152MB/S, 4:256MB/S, 5:296MB/S,6:320MB/S, 7:384MB/S, N>7:N*8MB/S
	
	BYTE DATA_PATH; // path 1~8, with path1~4 for mipi, and path 5~8 for dvp
	BYTE SENSOR_OUTPUT_FORMAT; //0x00->Sensor/CTS 8/8bit,0x01->Sensor/CTS 10/8bit,0x02->Sensor/CTS 12/8bit,0x0a->Sensor/CTS 10/10bit,0x0C->Sensor/CTS 12/12bit,0x0E->Sensor/CTS 14/14bit
	BYTE MIPI_LANE_NUMBER; //1:1Lane, 2:2Lanes, 4:4Lanes
	BYTE MIPI_T_hs_zero_MASK; //(0x00~0x0f, default:0x05),MIPI data lane high speed T_clk_zero mask

	// for path 1 or 5
	WORD DSP_SAMPLE_START_PIX_X; //(0~4095), start pixels abandoned,to make sure the first 2 valid pixels into the DSP must be G R
	WORD DSP_SAMPLE_START_PIX_Y; //(0~4095), start lines abandoned,to make sure the first 2 valid pixels into the DSP must be G R
	WORD DSP_OUTPUT_WIDTH; //horizontal output pixels
	WORD DSP_OUTPUT_HEIGHT; //vertical output lines

	BYTE BAYER_PIX_ORDER; // 0:RG_GB, 1:GR_BG, 2:GB_RG, 3:BG_GR
	
	BYTE YUV_PIX_ORDER;  //0:UYVY, 1:VYUY, 2:YVYU, 3:YUYV(default) , only for yuv path
	BYTE SENSOR_PCLK_SAMPLE_EDGE; //0:failing edge sample, 1:rising edge sample, only for path5~8

	BYTE SENSOR_HSYNC_POLARITY; //0:low active, 1:high active , only for path5~8
	BYTE SENSOR_VSYNC_POLARITY; //0:low active, 1:high active , only for path5~8

	BYTE MIPI_AUTO_SKEW_CALIBRATION; //0:manual(default), 1:auto calibration (not accurate sometimes)
	BYTE MIPI_CLK_DATA0_SKEW_CALIBRATION; //(0x00~0xFF), [7:4]:CLK0 delay,[3:0]:DATA0 delay, each step will delay 270mil, default:0x00
	BYTE MIPI_CLK_DATA1_SKEW_CALIBRATION; //(0x00~0xFF), [7:4]:CLK0 delay,[3:0]:DATA1 delay, each step will delay 270mil, default:0x00
	BYTE MIPI_CLK_DATA2_SKEW_CALIBRATION; //(0x00~0xFF), [7:4]:CLK0 delay,[3:0]:DATA2 delay, each step will delay 270mil, default:0x00
	BYTE MIPI_CLK_DATA3_SKEW_CALIBRATION; //(0x00~0xFF), [7:4]:CLK0 delay,[3:0]:DATA3 delay, each step will delay 270mil, default:0x00

	//int RegSettingsLen;
	vector<tagREG> vecRegSettings;


	_CTSSensorTab()
	{
		DVDD = 0;
		AVDD = 0;
		DOVDD = 0;
		AFVDD = 0;
		PWR4 = 0;
		PWR5 = 0;

		SENSOR_POWERDOWN_ACTIVE = 0;
		SENSOR_RESET_ACTIVE = 0;
		OPERATION_SEQUENCE = 0;
		SEQUENCE_DELAY = 0;
		SENSOR_IO_LEVEL = 0;
		I2C_SPEED = 0;
		SENSOR_I2C_ID = 0;
		SENSOR_REG_ADDR_WIDTH = 0;
		SENSOR_REG_DATA_WIDTH = 0;
		SENSOR_FRAME_RATE = 0;
		SENSOR_EXTCLK = 0;
		SENSOR_OUTPUT_WIDTH = 0;
		SENSOR_OUTPUT_HEIGHT = 0;
		
		USB3_BANDWIDTH = 0;
		
		DATA_PATH = 0;
		SENSOR_OUTPUT_FORMAT = 0;
		MIPI_LANE_NUMBER = 0;
		MIPI_T_hs_zero_MASK = 0;

		DSP_SAMPLE_START_PIX_X = 0;
		DSP_SAMPLE_START_PIX_Y = 0;
		DSP_OUTPUT_WIDTH = 0;
		DSP_OUTPUT_HEIGHT = 0;
		
		YUV_PIX_ORDER = 0;
		SENSOR_PCLK_SAMPLE_EDGE = 0;

		SENSOR_HSYNC_POLARITY = 0;
		SENSOR_VSYNC_POLARITY = 0;

		MIPI_AUTO_SKEW_CALIBRATION = 0;
		MIPI_CLK_DATA0_SKEW_CALIBRATION = 0;
		MIPI_CLK_DATA1_SKEW_CALIBRATION = 0;
		MIPI_CLK_DATA2_SKEW_CALIBRATION = 0;
		MIPI_CLK_DATA3_SKEW_CALIBRATION = 0;

	}
	
}CTSSensorTab;



class cdtparse
{
public:
	cdtparse();
	~cdtparse();

	void NewMemory();
	void DeleteMemory();

	BOOL bLoadLibIniFile(CString sfilename);
	BOOL bGetI2CDataFromLibFile(CString filename, pSensorTab pSensor);

	BOOL ParseDTINItoCTSStruct(CString sfilename);

	SensorTab CurrentSensor;

	char*      m_pSensorName;
	USHORT*    m_pSensorPara;
	USHORT*    m_pSensorSleepPara;

	//20130823 added..
	USHORT*		m_pAF_InitParaList;
	USHORT*		m_pAF_AutoParaList;
	USHORT*		m_pAF_FarParaList;
	USHORT*		m_pAF_NearParaList;
	USHORT*		m_pExposure_ParaList;
	USHORT*		m_pGain_ParaList;

	CTSSensorTab CTSSensor;
	
};

#endif // 
