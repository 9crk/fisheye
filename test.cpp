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
int is_adjusted = 0;
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
    
    CvMat* intrinsic_matrix= cvCreateMat(3,3, CV_64FC1); 
	CvMat* distortion_coeffs= cvCreateMat(1,4, CV_64FC1); 
			
	while(1){
		src = cvQueryFrame(capture);
		c = cvWaitKey(100);
		if(c == 'c' || c == 'C'){
			if(img == NULL)img = cvCreateImage(cvGetSize(src),8,1);
			cvCvtColor(src,img,CV_BGR2GRAY);
    		bool patternfound = cvFindChessboardCorners(img, board_size, corners,&corners_cnt,CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS);
    		if(!patternfound){
    			printf("fued\n");
    			continue;
    		}
			cvDrawChessboardCorners(src,board_size,corners,corners_cnt,patternfound);
			cvShowImage("src",src);
			cvWaitKey(0);
			/*cvFindCornerSubPix(img, corners, corners_cnt,cvSize(11, 11), cvSize(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            vector<Point2f> corners_zhou;
            for (j = 0; j < corners_cnt; j++)
            {
                cvCircle( src, cvPoint(corners[j].x,corners[j].y), 10, Scalar(0,0,255), 2, 8, 0);
                corners_zhou.push_back(Point2f(corners[j].x,corners[j].y));
            }
            */

            CvMat* object_points= cvCreateMat(1,board_size.width* board_size.height ,CV_64FC3);
			CvMat* image_points= cvCreateMat(1,board_size.width* board_size.height,CV_64FC2);
			CvMat* point_counts= cvCreateMat(1,1,CV_32SC1);
            CvMat* rotation_vectors= cvCreateMat(1,1, CV_64FC3);
			CvMat* translation_vectors= cvCreateMat(1,1, CV_64FC3);

			for (int i=0;i<board_size.height;i++) 
			{
				for (int j=0;j<board_size.width;j++) 
				{
					/* ¼ÙÉè¶¨±ê°å·ÅÔÚÊÀ½ç×ø±êÏµÖÐz=0µÄÆ½ÃæÉÏ */
					object_points->data.db[(i*board_size.width+j)*3] = i*20;
					object_points->data.db[(i*board_size.width+j)*3+1] =  j*20;
					object_points->data.db[(i*board_size.width+j)*3+2] = 0;
				}
			}
			for (unsigned int i=0;i<board_size.width*board_size.height;i++)
			{
				image_points->data.db[i*2]=corners[i].x;
				image_points->data.db[i*2+1]=corners[i].y;
			}
			point_counts->data.i[0] = board_size.width*board_size.height;
			
			cvCalibrateCamera2(object_points,image_points,point_counts,cvSize(src->width,src->height),intrinsic_matrix,distortion_coeffs,rotation_vectors,translation_vectors,0);
 			
 			is_adjusted = 1;
			//IplImage *t= cvCloneImage(src);
			//cvRemap(src, t, mapx, mapy);
			//cvShowImage("ttt",t);
	        //cvWaitKey(0);
		}
		if(is_adjusted){
			IplImage* mapx= cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
			IplImage* mapy= cvCreateImage(cvGetSize(src),IPL_DEPTH_32F,1);
			cvInitUndistortMap(intrinsic_matrix, distortion_coeffs, mapx, mapy);
			IplImage *t= cvCloneImage(src);
			cvRemap(src, t, mapx, mapy);
			cvShowImage("ttt",t);
	        cvReleaseImage(&t);	
		}
		cvShowImage("sss",src);
    	//IplImage* sss = zFish(src,cvPoint(700,360),cvPoint(300,0),310,50,2000,1000);
    	//cvReleaseImage(&sss);
	}
	//findChessboardCorners();
	cvShowImage("sss",src);
	cvWaitKey(0);
}
