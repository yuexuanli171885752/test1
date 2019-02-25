#ifndef _SHM_H_
#define _SHM_H_

#define RAM_INT_NUM			500
#define RAM_FLOAT_NUM		300
#define RAM_DOUBLE_NUM		300

#define MAX_IMG_WIDTH		3000
#define MAX_IMG_HEIGHT		2000
#define MAX_IMG_SIZE		(MAX_IMG_WIDTH*MAX_IMG_HEIGHT*3)

typedef struct _tagSHM
{
	char	DEVICE_NAME[MAX_PATH];
	int		RAM_INT[RAM_INT_NUM];
	float	RAM_FLOAT[RAM_FLOAT_NUM];
	double  RAM_DOUBLE[RAM_DOUBLE_NUM];
	BYTE	img_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT*3];
	
}SHM;

extern SHM *shm;  //share memory pointer

#endif