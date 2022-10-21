#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/core/utility.hpp>
#include <string>

using namespace std;

typedef struct {
    int i;
    float j;
} st_data;
st_data structdata;

cv::Mat img;

void onMouse(int event, int x,int y, int flags, void* params);

string getFilename(int nbArg, char* arguments[]) {
    const string keys =
        "{help h usage ? |      | print this message   }"
        "{@image1        |choux.mp4| image1 for compare   }"
        "{@image2        |<none>| image2 for compare   }"
        "{@repeat        |1     | number               }"
        "{path           |.     | path to file         }"
        "{fps            | -1.0 | fps for output video }"
        "{N count        |100   | count of objects     }"
        "{ts timestamp   |      | use time stamp       }"
        ;

    cv::CommandLineParser parser(nbArg, arguments, keys);
    parser.about("Application name v1.0.0");
    string file = parser.get<std::string>(0);
    return file;
}

cv::VideoCapture loadFrames(string filename) {
    cv::VideoCapture capture(filename);
   
    cv::Mat frameHsv;

    if( !capture.isOpened() )
        throw "Error when reading " + filename;
    return capture;
}

int eventKey() {
    char key = cv::waitKey(20); // waits to display frame
    int result = 1;

    switch (key) {
        case 'p':
            cv::waitKey(-1);// Pause a l'appui sur le P
            break;
        case 'q':
            result = 0; // Return 0 pour quitter
            break;
        default:
            break;   
    }
    return result;
}

void frameLoop(cv::VideoCapture capture) {
    cv::Mat frame;
    cv::namedWindow("output", 1);
    
    cv::setMouseCallback("output", onMouse, &structdata);
    while(capture.isOpened())
    {
        capture >> frame;
        //cv::cvtColor(frameHsv, frame, cv::COLOR_BGR2HSV);
        img = frame;
        //cv::inRange(frameHsv, cv::Scalar(24, 125, 168), cv::Scalar(30,255,255), frameHsv);
        
        if (frame.empty())
            break;

        cv::imshow("output", frame);
        
        // Event key principale
        int key = eventKey();
        if (key == 0)
            break;
    }
}

int main(int argc, char* argv[]) {
    
    string filename = getFilename(argc, argv);

    cv::VideoCapture capture = loadFrames(filename);

    // Loop principale
    frameLoop(capture);
    

    cv::waitKey(0);
}

void onMouse(int event,int x,int y,int flags,void* params){
  st_data *data = (st_data *)params;
  cv::Mat color = img(cv::Range(y,y+1),cv::Range(x,x+1));
  cv::Mat hsv;
  cv::cvtColor(color, hsv, cv::COLOR_BGR2HSV);
  
  if (event == cv::EVENT_LBUTTONDOWN) {
    std::cout << "x : " << x << " y : " << y << std::endl << " color : " << color << "hsv : " << hsv << std::endl;
    //  std::cout <<(int) hsv.at<cv::Vec3b>(0, 0).val[0] << std::endl;
    // 
  }
}