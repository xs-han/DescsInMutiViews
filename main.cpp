#include <iostream>
#include <unistd.h>
#include "VideoStream.h"
#include "BufferedVideoStream.h"
#define Debug
using namespace std;
using namespace cv;

int main() {
    std::cout << "Hello, World!" << std::endl;
    string videoName = "/home/xushen/Videos/GOPR0015.MP4";
    VideoStream bvdsm("/home/xushen/Videos/GOPR0015.MP4", 640);
    Mat newFrame;
    namedWindow(videoName,CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    int k = 0;
    ifstream fin("main.cpp");
    while(bvdsm >> newFrame && k != 'q'){
        imshow(videoName, newFrame);
        k = cvWaitKey(1);
        if(k == 'd'){
            sleep(5);
        }
    }
    return 0;
}