#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#define N_BLOCK 7
int cnt = 0;
typedef struct Srect{
	int x;
	int y;
	int width;
	int height;
}Srect;
Srect rect[N_BLOCK];
void onMouse(int event, int x, int y, int flags, void* param)  
{  
    switch (event)  
    {  
        case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度  
            //printf("x=%d y=%d\n",x,y);
            rect[cnt].x = x;
            rect[cnt].y = y;
            break;  
        case CV_EVENT_LBUTTONUP:    //鼠标右键按下响应：输入坐标并返回该坐标的灰度
        	//printf("w=%d h=%d\n",x,y);  
            rect[cnt].width = x - rect[cnt].x;
            rect[cnt].height = y - rect[cnt].y;
            printf("%d: %d %d %d %d\n",cnt,rect[cnt].x,rect[cnt].y,rect[cnt].width,rect[cnt].height);
            cnt++;
            if(cnt == N_BLOCK){
            	FILE*fp = fopen("rect.matrix","wb");
            	fwrite(&rect,1,sizeof(Srect)*N_BLOCK,fp);
            	fclose(fp);
            }
            break;            
    }
}  

int main(int argc,char*argv[])
{	
	IplImage* img = cvLoadImage(argv[1]);
	cvShowImage("choose rect",img);
	cvSetMouseCallback( "choose rect", onMouse,NULL);
	cvWaitKey(0);
    for(int i=0;i<N_BLOCK;i++){
        cvRectangleR(img,*(CvRect*)&rect[i],CV_RGB(255,0,0));
    }
    cvSaveImage("img2.jpg",img);
}

