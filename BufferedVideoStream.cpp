//
// Created by xushen on 9/13/17.
//

#include "BufferedVideoStream.h"

void bufferedAcquire(BufferedVideoStream * bvs, SynQueue<Mat> * buff) {
    Mat m;
    while((!bvs->isFinish()) && bvs->read(m)){
#ifdef Debug
        static int a = 0;
        cerr << a++ << endl;
#endif
        buff->put(m);
    }
    bvs->setFinish(true);
}

void notifyFinish(SynQueue<Mat> * buff, bool fin){
    buff->setFinish(fin);
}

BufferedVideoStream::BufferedVideoStream(const string &vName, int vWidth, int vHeight, int bSize)
        : VideoStream(vName, vWidth, vHeight), bufferSize(bSize), buff(bSize), finish(false){
    setBufferedVideo(vName);
}

BufferedVideoStream::BufferedVideoStream(int bSize) : bufferSize(bSize), buff(bSize), finish(false) {}

BufferedVideoStream::BufferedVideoStream(const BufferedVideoStream & bvstrm, int bSize)
        : VideoStream(bvstrm),bufferSize(bSize), buff(bSize), finish(false) {
}

void BufferedVideoStream::setBufferedVideo(const string &vName) {
    if (!buff.empty()) {
        buff.clear();
    }
    acquire(vName);
}

void BufferedVideoStream::acquire(const string &vName) {
    setVideo(vName);
    setFinish(false);
    thread t_acq(bufferedAcquire, this, &buff);
    t_acquire = std::move(t_acq);
    t_acquire.detach();
}

bool BufferedVideoStream::readFrame(Mat & m) {
    buff.take(m);
    if(isFinish() && buff.empty()){
        m = Mat::zeros(videoHeight,videoWidth, CV_8UC3);
        return false;
    }
    else{
        return true;
    }
}

BufferedVideoStream::~BufferedVideoStream() {
    setFinish(true);
}

bool BufferedVideoStream::isFinish() {
    std::lock_guard<std::mutex> locker(mtx_finish);
    return finish;
}

void BufferedVideoStream::setFinish(bool fin) {
    std::lock_guard<std::mutex> locker(mtx_finish);
    this->finish = fin;
    thread t_finish(notifyFinish,&buff,fin);
    t_finish.join();
}

BufferedVideoStream &BufferedVideoStream::operator>>(Mat &m) {
    readFrame(m);
    return *this;
}

BufferedVideoStream::operator bool(){
    std::lock_guard<std::mutex> locker(mtx_finish);
    return !finish;
}

