#include <stdio.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>



using namespace std;
using namespace cv;


const char *src_img_path = "../testimage/2.jpg";
const char *gray_img_path = "result_gray.jpg";
const char *threshold_img_path = "result_threshold.jpg";
const char *inv_img_path = "result_inv.jpg";
const char *morphology_img_path = "result_morphology.jpg";
const char *extract_img_path = "result_extract.jpg";
const char *location_img_path = "result_location.jpg";


bool isEligible(const RotatedRect &candidate){
    float error = 0.2;
    const float aspect = 4.5 / 0.3;  // 长宽比
    int area_min = 10 * aspect * 10; // 最小区域
    int area_max = 50 * aspect * 50; // 最大区域
    float r_min = aspect - aspect * error; // 考虑误差后的最小长宽比
    float r_max = aspect + aspect * error; // 考虑误差后的最大长宽比
    int area = candidate.size.height * candidate.size.width;
    float r = (float)candidate.size.width / (float)candidate.size.height;
    if(r < 1){
        r = 1/r;
    }
    return (area_min <= area) && (area <= area_max) && (r_min <= r) && (r <= r_max);
}


void posDetect(const Mat &in){
    // 利用阈值二值化
    Mat img_threshold;
    cv::threshold(in, img_threshold, 0, 255, cv::THRESH_OTSU);
    cv::imwrite(threshold_img_path, img_threshold);      

    Mat img_write(in.size(), in.type(), cv::Scalar(255));
    // 黑白色反转，即背景为黑色
    img_threshold  = img_write - img_threshold;
    cv::imwrite(inv_img_path, img_threshold);

    // 获取闭形态学的结构元素
    Mat element = getStructuringElement(MORPH_RECT, Size(13, 14));
    Mat img_close;
    cv::morphologyEx(img_threshold, img_close, MORPH_CLOSE, element);
    cv::imwrite(morphology_img_path, img_close);  
    
    vector<vector<Point> > contours;
    // 只检测外轮廓
    cv::findContours(img_close, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    
    // 对候选的轮廓进行筛选
    Mat outRecs;
    in.copyTo(outRecs);
    RotatedRect rect;
    vector<vector<cv::Point> >::iterator itc = contours.begin();
    for(; itc != contours.end(); itc++){
	// 返回每个轮廓的最小有界矩形区域
        RotatedRect mr = cv::minAreaRect(Mat(*itc));
		if(mr.size.height < 1 || mr.size.width < 1){
			continue;
		}
		Point2f verticesNow[4];
		mr.points(verticesNow);
		for(int i=0; i<4; i++){
			// 画黑色线条
			line(outRecs, verticesNow[i], verticesNow[(i + 1) % 4], cv::Scalar(0));
		}
		// 判断矩形轮廓是否符合要求
		if(isEligible(mr)){
			rect = mr;
		}
    }
	cv::imwrite(extract_img_path, outRecs);
	
	// 区域定位
	Mat out;
	in.copyTo(out);
	Point2f vertices[4];
	rect.points(vertices);
	for(int i=0; i<4; i++){
		// 画黑色线条
		line(out, vertices[i], vertices[(i+1)%4], cv::Scalar(0));
	}
	cv::imwrite(location_img_path, out);
	
	
}

void printImagText(const char * image_path){
  char *outText = NULL;
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  if(api->Init(NULL, "eng+chi_sim")){
    fprintf(stderr, "could not initialize tesseract.\n");
    exit(1);
  }

  Pix *image = pixRead(image_path);
  api->SetImage(image);

  outText = api->GetUTF8Text();
  printf("\n%s\n", outText);

  api->End();
  delete[] outText;
  pixDestroy(&image);

}


int main() {
    printf("hello\n");
    
	// 加载原始图像
    Mat srcImage = cv::imread(src_img_path);
    printf("*******************source image ocr*******************\n");
    printImagText(src_img_path);

	// 灰度化
    Mat grayImage;
    cv::cvtColor(srcImage, grayImage, CV_RGB2GRAY); 
    cv::imwrite(gray_img_path, grayImage);
    printf("*******************gray image ocr*******************\n");
    printImagText(gray_img_path);

    // 获得身份证文字区域
    posDetect(grayImage);

#if 0	
    cv::namedWindow("source image", WINDOW_NORMAL);
    cv::imshow("source image", srcImage);
    cv::waitKey(0);
#endif   
	
    return 0;
}
