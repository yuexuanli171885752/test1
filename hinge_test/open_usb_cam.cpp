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

img=cvQueryFrame(cap);//捕获相机的视频帧，并进行解码操作

if(!img)

break;

if(img->origin==IPL_ORIGIN_TL)//如果图片原点在左上角，将其沿X轴翻转，使左下角

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