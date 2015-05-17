#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
//#include "cvcam.h"

//图像的像素直接提取
#define        _I(img,x,y) ((unsigned char*)((img)->imageData + (img)->widthStep*(y)))[(x)]
//亚像素级灰度值
#define        _IF(image,x,y)    ( ((int)(x+1)-(x))*((int)(y+1)-(y))*_I((image),(int)(x),(int)(y)) + ((int)(x+1)-(x))*((y)-(int)(y))*_I((image),(int)(x),(int)(y+1)) + ((x)-(int)(x))*((int)(y+1)-(y))*_I((image),(int)(x+1),(int)(y)) + ((x)-(int)(x))*((y)-(int)(y))*_I((image),(int)(x+1),(int)(y+1)) )//插值后的像素值(IN表示interpolation),x、y可以为小数


IplImage* callback(IplImage* image);

int main()
{

   
    IplImage* image=cvLoadImage("rect3.jpg",-1);
    IplImage* Show3 = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1);
    //cvcamGetProperty(0, CVCAM_VIDEOFORMAT,NULL);
    cvNamedWindow( "径向矫正1", 1 );//创建窗口
    cvNamedWindow( "径向矫正2", 1 );//创建窗口
    cvNamedWindow( "原图", 1 );//创建窗口
    cvShowImage("原图", image);
    
    Show3=callback(image);
    cvShowImage("径向矫正2", Show3);
    cvWaitKey(0);

    cvDestroyWindow( "径向矫正1" );//销毁窗口
    cvDestroyWindow( "径向矫正2" );//销毁窗口
    cvDestroyWindow( "原图" );//销毁窗口
    return 1;
}

IplImage* callback(IplImage* image)
{

    IplImage* Show1 = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1);
    IplImage* Show2 = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1);
    IplImage* ImageC1 = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1);

    //转换为灰度图
    cvCvtColor( image, ImageC1, CV_RGB2GRAY);
   // cvFlip( ImageC1, NULL, 0);
    
    double *mi;
    double *md;

    mi = new double[3*3];
    md = new double[4];

    CvMat intrinsic_matrix,distortion_coeffs;

    //摄像机内参数
    cvInitMatHeader(&intrinsic_matrix,3,3,CV_64FC1,mi);
    
    //镜头畸变参数
    cvInitMatHeader(&distortion_coeffs,1,4,CV_64FC1,md);

    ///////////////////////////////////////////////// 
    double fc1,fc2,cc1,cc2,kc1,kc2,kc3,kc4;
  /*fc1 = 636.796592;
    fc2 = 639.002846;
    cc1 = 320.575856;
    cc2 = 238.909624;
    kc1 = 0.008447;
    kc2 = -0.072905;
    kc3 = -0.001818;
    kc4 = -0.000562; */
    
    fc1 = 426.331624;
    fc2 = 426.556478;
    cc1 = 365.956413;
    cc2 = 202.451300;
    kc1 = -0.422263;
    kc2 = 0.182853;
    kc3 = -0.007801;
    kc4 = 0.002509;

    cvmSet(&intrinsic_matrix, 0, 0, fc1);
    cvmSet(&intrinsic_matrix, 0, 1, 0);
    cvmSet(&intrinsic_matrix, 0, 2, cc1);
    cvmSet(&intrinsic_matrix, 1, 0, 0);
    cvmSet(&intrinsic_matrix, 1, 1, fc2);
    cvmSet(&intrinsic_matrix, 1, 2, cc2);
    cvmSet(&intrinsic_matrix, 2, 0, 0);
    cvmSet(&intrinsic_matrix, 2, 1, 0);
    cvmSet(&intrinsic_matrix, 2, 2, 1);

    cvmSet(&distortion_coeffs, 0, 0, kc1);
    cvmSet(&distortion_coeffs, 0, 1, kc2);
    cvmSet(&distortion_coeffs, 0, 2, kc3);
    cvmSet(&distortion_coeffs, 0, 3, kc4);
    /////////////////////////////////////////////////
    //下面有两种矫正方法，第一种用opencv库，第二种自定义方法
    //矫正畸变(opencv)
    cvUndistort2( ImageC1, Show1, &intrinsic_matrix, &distortion_coeffs);

    //矫正畸变
    for (int nx=0; nx<640; nx++)
    {
        for (int ny=0; ny<480; ny++)
        {
          //  double x=nx-50;
          //  double y=ny-50;
            double x=nx;
            double y=ny;
            double xx=(x-cc1)/fc1;
            double yy=(y-cc2)/fc2;
            double r2=pow(xx,2)+pow(yy,2);
            double r4=pow(r2,2);
            double xxx=xx*(1+kc1*r2+kc2*r4)+2*kc3*xx*yy+kc4*(r2+2*xx*xx);
            double yyy=yy*(1+kc1*r2+kc2*r4)+2*kc4*xx*yy+kc3*(r2+2*yy*yy);
            double xxxx = xxx*fc1+cc1;
            double yyyy = yyy*fc2+cc2;
            if (xxxx>0 && xxxx<640 && yyyy>0 && yyyy<480)
            {
                _I(Show2,nx,ny) = (int)_IF(ImageC1,xxxx,yyyy);
            }
            else
            {
                _I(Show2,nx,ny) = 0;
            }

        }
    }


    //显示
   // cvShowImage("径向矫正1", Show1);
  //  cvShowImage("径向矫正2", Show2);
  //  cvWaitKey(1);
    cvReleaseImage( &Show1 );    
  //  cvReleaseImage( &Show2 );    
    cvReleaseImage( &ImageC1 );   
    return Show2; //选了第二种方法

}
