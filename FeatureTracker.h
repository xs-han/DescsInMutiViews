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
    FeatureTracker(string vName);
    void track();
    void showTrackedDescs();
};


#endif //DESCSINMUTIVIEWS_FEATURESTRACKER_H
