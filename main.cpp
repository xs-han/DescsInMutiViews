#include <iostream>
#include <unistd.h>
#include "VideoStream.h"
#include "BufferedVideoStream.h"
#include "FeatureTracker.h"

#define Debug
using namespace std;
using namespace cv;

int main() {
    std::cout << "Hello, World!" << std::endl;
    string videoName = "/home/xushen/Videos/GOPR0015.MP4";
    FeatureTracker vt(videoName);
    vt.track();
    return 0;
}