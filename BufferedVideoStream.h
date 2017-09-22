//
// Created by xushen on 9/13/17.
//

#ifndef DESCSINMUTIVIEWS_BUFFEREDVIDEOSTREAM_H
#define DESCSINMUTIVIEWS_BUFFEREDVIDEOSTREAM_H

#include "VideoStream.h"
#include "SynQueue.hpp"

#define Debug

class BufferedVideoStream: public VideoStream{

    int bufferSize;
    SynQueue<Mat> buff;
    bool finish;
    std::mutex mtx_finish;
    thread t_acquire;

public:
    bool isFinish();

    void setFinish(bool finish);

public:
    explicit BufferedVideoStream(const string & vName, int vWidth = -1, int vHeight = -1, int bSize = 20);

    explicit BufferedVideoStream(int bufferSize = 20);

    virtual ~BufferedVideoStream();

    BufferedVideoStream(const BufferedVideoStream &, int bSize = 20);

    void acquire(const string &vName);

    void setBufferedVideo(const string &vName);

    bool readFrame(Mat & m);

    virtual BufferedVideoStream& operator >> (Mat & m);

    virtual explicit operator bool();
};


#endif //DESCSINMUTIVIEWS_BUFFEREDVIDEOSTREAM_H
