//
// Created by xushen on 9/11/17.
//

#include "VideoStream.h"
using namespace cv;

VideoStream::VideoStream(const string &vName, int vWidth, int vHeight) : videoName(
        vName), videoWidth(vWidth), videoHeight(vHeight), finish(false) {
    setVideo(vName);
    if(videoWidth < 0 && videoHeight < 0){
        videoHeight = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
        videoWidth = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
    }
    else if(vHeight < 0){
        videoHeight = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT) *
                (((double)vWidth) / cap.get(CV_CAP_PROP_FRAME_WIDTH)));
    }
}

VideoStream::VideoStream()  : videoName("unknown"), videoWidth(-1), videoHeight(-1), finish(false){}

VideoStream::VideoStream(const VideoStream & imVideo):videoName(imVideo.videoName), videoWidth(imVideo.videoWidth),
                                                      videoHeight(imVideo.videoHeight), frameRate(imVideo.frameRate),
                                                      frame(imVideo.frame.clone()), cap(imVideo.cap),
                                                      finish(imVideo.finish){}

void VideoStream::showVideo(bool enableKeyStop) {
    Mat newFrame;
    namedWindow(videoName,CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    while(read(newFrame)){
        imshow(videoName, newFrame);
        int k = 0;
        if (enableKeyStop && (k = waitKey(30)) >= 0) {
            if(k == 'q'){
                break;
            }
            waitKey(0);
        }
    }
}

void VideoStream::setVideo(const string &videoName) {
    VideoStream::videoName = videoName;
    cap.open(videoName);
    if(!cap.isOpened()){
        cerr << "Open input video error." << endl;
        exit(-1);
    }  // check if we succeeded
    frameRate = cap.get(CV_CAP_PROP_FPS);
}

VideoStream & VideoStream::operator>>(Mat &m) {
    read(m);
    return (*this);
}

bool VideoStream::read(Mat &m) {
    int loops = 0;
    while(!cap.read(frame)){
        loops += 1;
        if(loops == 5){
            break;
        }
    }
    if(loops == 5){
        finish = true;
        return false;
    }
    else {
        resize(frame, m, Size(videoWidth, videoHeight));
        if (loops > 0) {
            cerr << "warning: An empty frame is founded and discarded. " << endl;
            return true;
        }
        else {
            return true;
        }
    }
}

int VideoStream::getVideoWidth() const {
    return videoWidth;
}

void VideoStream::setVideoWidth(int videoWidth) {
    VideoStream::videoWidth = videoWidth;
}

int VideoStream::getVideoHeight() const {
    return videoHeight;
}

void VideoStream::setVideoHeight(int videoHeight) {
    VideoStream::videoHeight = videoHeight;
}

double VideoStream::getFrameRate() const {
    return frameRate;
}

VideoStream::operator bool() {
    return !finish;
}

