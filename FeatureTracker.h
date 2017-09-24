//
// Created by xushen on 9/21/17.
//

#ifndef DESCSINMUTIVIEWS_FEATURESTRACKER_H
#define DESCSINMUTIVIEWS_FEATURESTRACKER_H


#include "VideoStream.h"
#include "FeatureMatcher.h"
#include "FeatureExtractor.h"

class FeatureTracker {
    string videoName;
    VideoStream vs;
    FeatureMatcher fm;
    FeatureExtractor fe;

    Mat curtFrame;
    vector<KeyPoint> curtKps;
    Mat curtDescs;

    Mat lastFrame;
    vector<KeyPoint> lastKps;
    Mat lastDescs;

    Mat initFrame;
    vector<KeyPoint> initKps;
    Mat initDescs;

    vector<DMatch> matches4Init;
    vector<DMatch> matches4CurtFrame;

public:
    explicit FeatureTracker(string vName);
    void track();
    void forwardMatches(Mat &A, Mat &C, vector<DMatch> &matchAB, vector<DMatch> &matchBC, vector<DMatch> &matchAC);
    double calL2Distance(const Mat & a, const Mat & b);
};


#endif //DESCSINMUTIVIEWS_FEATURESTRACKER_H
