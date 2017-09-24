//
// Created by xushen on 9/18/17.
//

#include "FeatureExtractor.h"
using namespace std;
using namespace cv;

FeatureExtractor::FeatureExtractor(const string &descriptor) : desc(descriptor) {
    setFeature(descriptor);
}

void FeatureExtractor::extract(const Mat & m, vector<KeyPoint> & p, Mat & descs) {
    detector->detectAndCompute(m, cv::Mat(), p, descs);
}

FeatureExtractor::FeatureExtractor() {
}

void FeatureExtractor::setFeature(const string &descriptor) {
    desc = descriptor;
    if (descriptor=="orb")        detector=cv::ORB::create();
    else if (descriptor=="brisk") detector=cv::BRISK::create();
    else if (descriptor=="akaze") detector=cv::AKAZE::create();
    else if(descriptor=="surf" )  detector=cv::xfeatures2d::SURF::create(500, 4, 2, true);
    else if(descriptor=="sift" )  detector=cv::xfeatures2d::SIFT::create(1000);
    else throw std::runtime_error("Invalid descriptor");
    assert(!descriptor.empty());
}


