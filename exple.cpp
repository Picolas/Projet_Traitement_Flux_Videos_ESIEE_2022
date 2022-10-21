//! @file   exple.cpp
//! @author P. FOUBERT
//! @brief  OpenCV project to show how to load and display an image

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/core/utility.hpp>

#include <string>

#ifndef DATA_FOLDER
#define DATA_FOLDER 
#endif
void onMouse(int event,int x,int y,int flags,void* params);

typedef struct {
int i;
float j;
} st_data;
cv::Mat img;
cv::Mat img2;
// ===========================================================================
//! @brief  Main
//! @return <0 if error, 0 if success, >0 if warning
// ===========================================================================
int main(int argc,char * argv[]) {
  int res(0);
  char c;
  int nb =0 ;
  int angle=0;
  const std::string keys =
        "{help h usage ? |      | print this message   }"
        "{@image1        |smarties.jpg| image1 for compare   }"
        "{@image2        |<none>| image2 for compare   }"
        "{@repeat        |1     | number               }"
        "{path           |.     | path to file         }"
        "{fps            | -1.0 | fps for output video }"
        "{N count        |100   | count of objects     }"
        "{ts timestamp   |      | use time stamp       }"
        ;
st_data data;
data.i = 12;
    cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
  cv::setMouseCallback("image",onMouse,&data);
cv::CommandLineParser parser(argc, argv, keys);
parser.about("Application name v1.0.0");
  std::string img1 = parser.get<std::string>(0);
  const std::string filename(img1);

  img = cv::imread(filename, cv::IMREAD_COLOR);
  img2 = cv::imread(filename, cv::IMREAD_COLOR);
  cv::Mat imgbw;
  cv::Point2f pc(img.cols/2.f,img.rows/2.f);
  cv::cvtColor(img, imgbw, cv::COLOR_BGR2GRAY);
  cv::inRange(imgbw, 230, 255, imgbw);
  cv::Mat a = cv::Mat::ones(imgbw.size(), imgbw.type());
  a *= 255;
  a -= imgbw;
  //cv::cvtColor(img, img2,cv::COLOR_RGB2GRAY);
  if(img.empty()) {
    std::cout << "Cannot load image \"" << filename << "\"!" << std::endl;
    res = -1;
  } else {
    //cv::namedWindow("imagegris",cv::WINDOW_AUTOSIZE);
    //cv::imshow("imagegris",img2);
    //cv::imwrite("smartiesgris.png",img2);
      std::vector<std::vector<cv::Point>> contours;
      //cv::blur(a, a, cv::Size(5, 5));
      cv::GaussianBlur(a, a, cv::Size(5, 5), 0);
      cv::dilate(a, a, 0);
      cv::erode(a, a, 0);
      cv::findContours(a, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
     
      std::cout << contours.size() << std::endl;
      cv::imshow("test", img);
    cv::imshow("image", img2);
    while(1){ 
      c= (char) cv::waitKey(0);
      if('l'==c) {
        angle+=10;
        cv::Mat r = cv::getRotationMatrix2D(pc, angle, 1.0);
         //cv::Rect2f bbox=cv::RotatedRect(cv::Point2f(),img.size(),angle).boundingRect2f();
          cv::warpAffine(img, img2, r, img.size());
          cv::imshow("image", img2);
      }
      if('r'==c){
        angle-=10;
          cv::Mat r = cv::getRotationMatrix2D(pc, angle, 1.0);
          //cv::Rect2f bbox=cv::RotatedRect(cv::Point2f(),img.size(),-10).boundingRect2f();
          cv::warpAffine(img, img2, r, img.size());
          cv::imshow("image", img2);
      }
      if ('a' == c && nb < contours.size()) {
        
          cv::drawContours(img, contours, nb, cv::Scalar(255, 0, 0), 6);
          cv::imshow("test", img);
          nb++;
      }
      if('s'==c) break;
    }
  }
  return res;
}

void onMouse(int event,int x,int y,int flags,void* params){
  st_data *data = (st_data *)params;
  cv::Mat color = img2(cv::Range(y,y+1),cv::Range(x,x+1));
  cv::Mat hsv;
  cv::Mat imgf;
  cv::Mat dst;
  cvtColor(color, hsv, cv::COLOR_BGR2HSV);
  if (event == cv::EVENT_LBUTTONDOWN) {
      std::cout << "x : " << x << " y : " << y << std::endl << " color : " << color << " HSV : " << hsv << std::endl;
    //  std::cout <<(int) hsv.at<cv::Vec3b>(0, 0).val[0] << std::endl;
      cvtColor(img2, imgf, cv::COLOR_BGR2HSV);
      cv::inRange(imgf, cv::Scalar(24, 125, 168), cv::Scalar(30,255,255), imgf);
      cv::copyTo(img2, dst, imgf);
      cv::imshow("filtre", dst);
  }
}
