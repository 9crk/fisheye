#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#define PI 3.141592654


void onMouse(int event, int x, int y, int flags, void* param)  
{  
    switch (event)  
    {  
        case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度  
            printf("x=%d y=%d\n",x,y);
            break;  
        case CV_EVENT_RBUTTONDOWN:    //鼠标右键按下响应：输入坐标并返回该坐标的灰度  
            break;            
    }  
}  


IplImage* zFish(IplImage* fishImage,CvPoint center,CvPoint scenter,int R, int d,int width,int height)
{
	int x,y,h,w;
	double thetaX,thetaY,dX,dY;
	IplImage* fishImg = cvCreateImage(cvSize(width,height),8,3);

	for(h=0;h<height;h++){
		for(w=0;w<width;w++){
			x = scenter.x - width/2 + w;
			y = scenter.y - height/2 + h;
			thetaX = atan(x*1.0/(R+d));
			dX = sin(thetaX)*R;
			thetaY = atan(y*1.0/(R+d));
			dY = cos(thetaY)*R;
			//printf("%d-%d = %f-%f\n",x,y,thetaX*180/PI,thetaY*180/PI);
			unsigned char rgb[3];
			int rx,ry;
			rx = center.x + (int)(dX+0.5);
			ry = center.y + (int)(dY+0.5); 
			//printf("%d-%d %d %d\n",x,y,rx,ry);
			
			rgb[0] = fishImage->imageData[ry*fishImage->width*3 + rx*3];
			rgb[1] = fishImage->imageData[ry*fishImage->width*3 + rx*3+1];
			rgb[2] = fishImage->imageData[ry*fishImage->width*3 + rx*3+2];
			fishImg->imageData[h*width*3+w*3] = rgb[0];
			fishImg->imageData[h*width*3+w*3+1] = rgb[1];
			fishImg->imageData[h*width*3+w*3+2] =rgb[2];
		}
	}
	return fishImg;
}
int main()
{	
	int i,j,ret;
	IplImage* src;
	CvCapture* capture = cvCreateCameraCapture(0);
	IplImage* img = NULL;
	CvSize board_size = cvSize(9,6);
	CvPoint2D32f *corners = (CvPoint2D32f*)malloc(sizeof(CvPoint2D32f)*100);
	int corners_cnt;

	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH, 1280);
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT, 720);  
    char c;
    cvNamedWindow("ooo");
    //cvSetMouseCallback( "ooo", onMouse,NULL);
    IplImage* s = cvCreateImage(cvSize(1000,500),8,3);
	while(1){
		src = cvQueryFrame(capture);
		cvShowImage("sss",src);
    	IplImage* sss = zFish(src,cvPoint(700,360),cvPoint(0,0),310,50,2000,1000);
    	cvResize(sss,s);

    	cvShowImage("ooo",s);
    	c = cvWaitKey(200);

    	cvReleaseImage(&sss);
	}
	//findChessboardCorners();
	cvShowImage("sss",src);
	cvWaitKey(0);
}
