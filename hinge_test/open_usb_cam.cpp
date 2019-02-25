#include"cv.h"

#include"highgui.h"

#include"iostream"

using namespace std;

int main(int argc,char* argv[])

{

CvCapture* cap;

cap=cvCaptureFromCAM(1);

if(!cap)

{

cout<<"create camera capture error"<<endl;

system("pause");

exit(-1);

}

cvNamedWindow("img",1);

IplImage* img;

while(1)

{

img=cvQueryFrame(cap);//�����������Ƶ֡�������н������

if(!img)

break;

if(img->origin==IPL_ORIGIN_TL)//���ͼƬԭ�������Ͻǣ�������X�ᷭת��ʹ���½�

{

cvFlip(img,img);

}

cvShowImage("img",img);

cvWaitKey(3);

}

cvReleaseCapture(&cap);

cvDestroyAllWindows();

cvReleaseImage(&img);

return 0;

}