
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <math.h>




BOOL CalcRGBBlockAvg8Bit(unsigned char *pRGBBuf, int blockW, int blockH, double &r_avg, double &g_avg, double &b_avg);
BOOL CalcRAWBlockAvg8Bit(unsigned char *pImgbuf, int w, int h, unsigned char nBayerOrder, double &r_avg, double &gr_avg, double &gb_avg, double &b_avg);
BOOL CalcGrayBlockAvg8Bit(unsigned char *pGrayBuf, int blockW, int blockH, double &y_avg);

BOOL CalcGray8ToGray10(unsigned short *pBuf10Bit, unsigned char *pBuf8Bit, int w, int h);
BOOL CalcGray10ToGray8(unsigned char *pBuf8Bit, unsigned short *pBuf10Bit, int w, int h);
BOOL CalcGray12ToGray8(unsigned char *pBuf8Bit, unsigned short *pBuf12Bit, int w, int h);
BOOL CalcGray12ToGray10(WORD *pBuf10Bit, WORD *pBuf12Bit, int w, int h);


BOOL BayerRaw8ToRGB24(BYTE *bgrbuf, BYTE *rawbuf, int width, int height, int BayerOrder, bool bflip);

BOOL Rgb24ToGray8(unsigned char *pgry, unsigned char *prgb, int w, int h, bool bflip);
BOOL Gray8ToRgb24(unsigned char *prgb, unsigned char *pgry, int w, int h);

BOOL CutRGB24ToGray8(unsigned char *pGrayBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0);
BOOL CutRGB24ToRGB24(unsigned char *pRGBBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, int bpp, bool bInvert = 0);

BOOL CutYUYVToGray8(unsigned char *pGrayBuf, unsigned char *pyuyvbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char yuvorder);
BOOL CutYUYVToRGB24(unsigned char *pRGBBuf, unsigned char *pyuyvbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char yuvorder);


BOOL CutBayerRaw8ToGray8(unsigned char *pGrayBuf, unsigned char *rawbuf, int width, int height, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder);
BOOL CutBayerRaw8ToRGB24(unsigned char *bgrbuf, unsigned char *rawbuf, int width, int height, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder);
BOOL CutBayerRaw10ToRGB24(unsigned char *bgrbuf, unsigned char *rawbuf, int width, int height, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder, int shift);

BOOL CutBayerRaw8ToBayerRaw8(unsigned char *pBayerBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder, BYTE &BayerOrderBlk);
BOOL CutBayerRaw8ToBayerRaw10(unsigned char *pBayerBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder, BYTE &BayerOrderBlk);

BOOL CutBayerRaw10ToBayerRaw8(unsigned char *pBayerBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder, BYTE &BayerOrderBlk);
BOOL CutBayerRaw10ToBayerRaw10(unsigned char *pBayerBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder, BYTE &BayerOrderBlk);

BOOL CutBayerRaw12ToBayerRaw8(unsigned char *pBayerBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder, BYTE &BayerOrderBlk);
BOOL CutBayerRaw12ToBayerRaw10(unsigned char *pBayerBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char BayerOrder, BYTE &BayerOrderBlk);
	

BOOL CutCalcBayerBlock8Bit(unsigned char *pBayerBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char nDataFormat, unsigned char BayerOrder, BYTE &BayerOrderBlk);
BOOL CutCalcBayerBlock10Bit(unsigned char *pBayerBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char nDataFormat, unsigned char BayerOrder, BYTE &BayerOrderBlk);


BOOL CutCalcGrayBlock8Bit(unsigned char *pGrayBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char nDataFormat, unsigned char BayerOrder);
BOOL CutCalcGrayBlock10Bit(unsigned short *pGrayBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char nDataFormat, unsigned char BayerOrder);
BOOL CutCalcRGB24Block(unsigned char *pRGBBuf, unsigned char *pImgbuf, int w, int h, int blockW, int blockH, int x0, int y0, unsigned char nDataFormat, unsigned char BayerOrder);

#endif
