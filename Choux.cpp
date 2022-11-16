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
    cv::Mat frameHsv;
    int fourcc = capture.get(cv::CAP_PROP_FOURCC);
   
    int framerate = capture.get(cv::CAP_PROP_FPS);
    int frame_width = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    cv::VideoWriter output("choux_filtre.avi", cv::VideoWriter::fourcc('M','J','P','G'), framerate, cv::Size(frame_width, frame_height));
    //output.fourcc(cc1, cc2, cc3, cc4);
    cv::namedWindow("output", 1);
    cv::namedWindow("filtre", 2);
    
    cv::setMouseCallback("output", onMouse, &structdata);
    while(capture.isOpened())
    {

        cv::Mat framef;
        capture >> frame;
        cv::cvtColor(frame, frameHsv, cv::COLOR_BGR2HSV);
        img = frame;
        cv::inRange(frameHsv, cv::Scalar(35, 16, 100), cv::Scalar(72, 160, 255), frameHsv);
        cv::copyTo(frame, framef, frameHsv);
        

        if (frame.empty())
            break;

        output<<framef;

        cv::imshow("output", frame);
        cv::imshow("filtre", framef);

        // Event key principale
        int key = eventKey();
        if (key == 0)
            break;
    }

    output.release();

}

int main(int argc, char* argv[]) {
    
    string filename = getFilename(argc, argv);

    cv::VideoCapture capture = loadFrames(filename);
    cv::VideoWriter output;

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