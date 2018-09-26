#include <stdio.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int main() {
    printf("hello\n");
    Mat srcImage;
    srcImage = imread("/home/loocha/fanghua/Projects/idcard-ocr/testimage/test1.jpg");
    namedWindow("source image", WINDOW_NORMAL);
    imshow("source image", srcImage);
    waitKey(0);
    return 0;
}
