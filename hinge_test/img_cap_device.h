#ifndef _IMG_CAP_DEVICE_H_
#define _IMG_CAP_DEVICE_H_

class ImgCapDev
{
public:
	virtual int init(int id){return TRUE;}
	virtual int run(void){return TRUE;}
	virtual int cap(BYTE *buf, long buf_size){return TRUE;}
	virtual int stop(void){return TRUE;}
	virtual int destroy(void){return TRUE;}
};

class ImgFile:public ImgCapDev
{
	virtual int init(int id);
	virtual int run(void);
	virtual int cap(BYTE *buf, long buf_size);
	virtual int stop(void);
	virtual int destroy(void);

private:
	int dev_id;
};

class CVBS_TSW_306E:public ImgCapDev
{
public:
	virtual int init(int id);
	virtual int run(void);
	virtual int cap(BYTE *buf, long buf_size);
	virtual int stop(void);
	virtual int destroy(void);

private:
	int dev_id;
	int is_run;
};

class CTS:public ImgCapDev
{
public:
	
	virtual int init(int id);
	virtual int run(void);
	virtual int cap(BYTE *buf, long buf_size);

	virtual int stop(void);
	virtual int destroy(void);
//	CTS();
//	HWND handle;

private:
	int dev_id;
	char sensor_path[MAX_PATH];
};

class Hinge:public ImgCapDev
{
public:
	virtual int init(int id);
	virtual int run(void);
	virtual int cap(BYTE *buf, int buf_size);

	virtual int stop(void);
	virtual int destroy(void);

private:
	int dev_id;

};


#endif

