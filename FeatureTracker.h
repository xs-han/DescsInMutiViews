//
// Created by xushen on 9/21/17.
//

#ifndef DESCSINMUTIVIEWS_FEATURESTRACKER_H
#define DESCSINMUTIVIEWS_FEATURESTRACKER_H


#include "VideoStream.h"
#include "FeatureMatcher.h"
#include "FeatureExtractor.h"

class FeatureTracker {
    int historySize = 20;

    string videoName;
    VideoStream vs;
    FeatureMatcher fm;
    FeatureExtractor fe;

    Mat curtFrame;
    vector<KeyPoint> curtKps;
    Mat curtDescs;

    deque<Mat> historicalFrame;
    deque<vector<KeyPoint> > historicalKps;
    deque<Mat> historicalDescs;

    Mat initFrame;
    vector<KeyPoint> initKps;
    Mat initDescs;

    deque<vector<DMatch> > matchesHistoryAndInit;
    vector<vector<DMatch> > matchesCurtAndHistory;
    vector<vector<DMatch> > matchesCurtAndInit;
    vector<DMatch> mergedMatchesCurtAndInit;

public:
    explicit FeatureTracker(string vName);
    void track();
    void forwardMatches(Mat &A, Mat &C, vector<DMatch> &matchAB, vector<DMatch> &matchBC, vector<DMatch> &matchAC);
    void mergeMatches(Mat &descA, Mat &descB, vector<vector<DMatch>> &multiMatchesAB, vector<DMatch> &matchAB);
    double calL2Distance(const Mat & a, const Mat & b);
    template<typename T>
    void pushQueue(deque<T> & queue, const T & anElement);

    double averageDistanse(const vector<DMatch> &match);
};


#endif //DESCSINMUTIVIEWS_FEATURESTRACKER_H
