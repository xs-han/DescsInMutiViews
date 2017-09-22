//
// Created by xushen on 9/11/17.
//

#ifndef DESCSINMUTIVIEWS_IMPORTVIDEO_H
#define DESCSINMUTIVIEWS_IMPORTVIDEO_H

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#define Debug
using namespace std;
using namespace cv;

class VideoStream {
protected:
    string videoName;
    int videoWidth;
    int videoHeight;
    double frameRate;
    Mat frame;
    VideoCapture cap;
    bool finish;

public:
    explicit VideoStream(const string &vName, int vWidth = -1, int vHeight = -1);

    VideoStream();

    VideoStream(const VideoStream &);

    void setVideo(const string &videoName);

    bool read(Mat & m);

    virtual VideoStream& operator >> (Mat & m);

    virtual explicit operator bool() ;

    void showVideo(bool enableKeyStop);

    int getVideoWidth() const;

    void setVideoWidth(int videoWidth);

    int getVideoHeight() const;

    void setVideoHeight(int videoHeight);

    double getFrameRate() const;

};


#endif //DESCSINMUTIVIEWS_IMPORTVIDEO_H
