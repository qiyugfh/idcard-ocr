#include <stdio.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>



using namespace std;
using namespace cv;


Mat getRplane(const Mat &in){
    vector<Mat> splitBGR(in.channels());
    cv::split(in, splitBGR);
 
    if(in.cols > 700 | in.rows > 600){
      Mat resizeR(450, 600, CV_8UC1);
      cv::resize(splitBGR[2], resizeR, resizeR.size());
      return resizeR;
    }else{
      return splitBGR[2];
    }
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
    char *src_img_path = "../testimage/2.jpg";
    char *dest_img_path = "result.jpg";
    Mat srcImage = cv::imread(src_img_path);
    Mat destImage = getRplane(srcImage);
    
    cv::imwrite(dest_img_path, destImage);

    printf("*******************source image ocr*******************\n");
    printImagText(src_img_path);
    printf("*******************rplane image ocr*******************\n");
    printImagText(dest_img_path);

	
    cv::namedWindow("source image", WINDOW_NORMAL);
    cv::imshow("source image", srcImage);
    cv::imshow("rpalne image", destImage);
    
    cv::waitKey(0);
    return 0;
}
