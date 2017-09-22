//
// Created by xushen on 9/18/17.
//

#ifndef DESCSINMUTIVIEWS_FEATUREEXTRATOR_H
#define DESCSINMUTIVIEWS_FEATUREEXTRATOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace std;
using namespace cv;


class FeatureExtractor{
public:
    string desc;
    cv::Ptr<cv::Feature2D> detector;
public:
    FeatureExtractor();
    explicit FeatureExtractor(const string &descriptor);
    void extract(const Mat & m, vector<KeyPoint> & p, Mat & descs);
    void setFeature(const string &descriptor);
};


#endif //DESCSINMUTIVIEWS_FEATUREEXTRATOR_H
