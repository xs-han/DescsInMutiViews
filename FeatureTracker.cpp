//
// Created by xushen on 9/21/17.
//

#include "FeatureTracker.h"
#include <ctime>

FeatureTracker::FeatureTracker(string vName):historySize(10) {
    videoName = vName;
    vs.setVideo(vName);
    vs.setVideoWidth(640);
    vs.setVideoHeight(360);
    fe.setFeature("surf");
    fm.setDetecter(fe.detector);
}

void FeatureTracker::track() {
    namedWindow(videoName, 0);
    namedWindow("Matches", 0);
    namedWindow("MatchWithInit", 0);

    Mat matchFrame;
    Mat matchInitFrame;
    while(vs >> curtFrame){
        if(initFrame.empty()){
            historicalFrame.clear();
            historicalKps.clear();
            historicalDescs.clear();
            matchesHistoryAndInit.clear();
            matchesCurtAndHistory.clear();
            matchesCurtAndInit.clear();
            curtKps.clear();
            initKps.clear();

            initFrame = curtFrame.clone();
            fe.extract(initFrame, initKps, initDescs);
            matchesCurtAndHistory.resize(historySize);
            matchesCurtAndInit.resize(historySize);
            for(int i = 0; i < historySize; i++){
                Mat tmpFrame;
                vector<KeyPoint> tmpKps;
                Mat tmpDescs;
                vector<DMatch> tmpMatch4Init;

                vs >> tmpFrame;
                fe.extract(tmpFrame, tmpKps, tmpDescs);
                historicalFrame.push_front(tmpFrame.clone());
                historicalKps.push_front(tmpKps);
                historicalDescs.push_front(tmpDescs.clone());

                fm.match(tmpKps, tmpDescs, initKps, initDescs, tmpMatch4Init);
                matchesHistoryAndInit.push_front(tmpMatch4Init);
            }
        }
        //clock_t begin = clock();
        int key = cvWaitKey((int)(vs.getFrameRate()+0.5) - 20);
        imshow(videoName, curtFrame);
        switch (key){
            case -1:
            {
                break;
            }
            case 's':
            {
                initFrame.resize(0);
                continue;
            }
            case 'p':
            {
                cvWaitKey(0);
                break;
            }
            case 'q':
            {
                return;
            }
            default:
            {
                cout << "s -> start, q -> quit, p -> pause." << endl;
            }
        }
        fe.extract(curtFrame, curtKps, curtDescs);
        //cout << "extract: " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
        for(int i = 0; i < historySize; i++){
            vector<DMatch> tmpMatch4Curt;
            fm.match( curtKps, curtDescs, historicalKps[i], historicalDescs[i],tmpMatch4Curt);
            matchesCurtAndHistory[i] = tmpMatch4Curt;
        }

        //cout << "match: " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
        cv::drawMatches(curtFrame, curtKps, historicalFrame[0], historicalKps[0], matchesCurtAndHistory[0], matchFrame);
        imshow("Matches", matchFrame);

        for(int i = 0; i < historySize; i++){
            forwardMatches(curtDescs, initDescs, matchesCurtAndHistory[i], matchesHistoryAndInit[i], matchesCurtAndInit[i]);
        }
        mergeMatches(curtDescs, initDescs, matchesCurtAndInit, mergedMatchesCurtAndInit);
        drawMatches(curtFrame, curtKps, initFrame, initKps, mergedMatchesCurtAndInit, matchInitFrame);
        imshow("MatchWithInit", matchInitFrame);

        cout << averageDistanse(mergedMatchesCurtAndInit) << endl;

        pushQueue(historicalFrame, curtFrame.clone());
        pushQueue(historicalKps, curtKps);
        pushQueue(historicalDescs, curtDescs.clone());
        pushQueue(matchesHistoryAndInit, mergedMatchesCurtAndInit);
        //cout << "show: " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
    }
}

void FeatureTracker::forwardMatches(Mat &descA, Mat &descC, vector<DMatch> &matchAB, vector<DMatch> &matchBC,
                                    vector<DMatch> &matchAC) {
    matchAC.clear();
    for(int i = 0; i < matchAB.size(); i++ ){
        for(int j = 0; j < matchBC.size(); j++){
            if(matchAB[i].trainIdx == matchBC[j].queryIdx){
                float dis = static_cast<float>(calL2Distance(descA.row(i), descC.row(j)));
                matchAC.push_back(DMatch(matchAB[i].queryIdx, matchBC[j].trainIdx, dis));
                continue;
            }
        }
    }
}

void FeatureTracker::mergeMatches(Mat &descA, Mat &descB, vector<vector<DMatch> > &multiMatchesAB,
                                  vector<DMatch> &matchAB) {
    matchAB.clear();
    const int numBDescs = descB.rows;
    int existMatchInB[numBDescs];
    for(int i = 0; i < numBDescs; i++){
        existMatchInB[i] = -1;
    }

    for(int i = 0; i < multiMatchesAB.size(); i++ ){
        const vector<DMatch> & anImageMatchPair = multiMatchesAB[i];
        for(int j = 0; j < anImageMatchPair.size(); j++){
            const DMatch & aMatch = anImageMatchPair[j];
            if(existMatchInB[aMatch.trainIdx] == -1){
                existMatchInB[aMatch.trainIdx] = aMatch.queryIdx;
            }
        }
    }

    for(int i = 0; i < numBDescs; i++){
        if(existMatchInB[i] != -1){
            float dis = static_cast<float>(calL2Distance(descA.row(existMatchInB[i]), descB.row(i)));
            matchAB.push_back(DMatch(existMatchInB[i], i, dis));
        }
    }
}

double FeatureTracker::calL2Distance(const Mat &a, const Mat &b) {
    assert(a.rows == 1 && b.rows == 1 && a.cols == b.cols);
    double dis = 0;
    for(int i = 0; i < a.rows; i++){
        dis += (a.at<float>(1,i) - b.at<float>(1,i)) * (a.at<float>(1,i) - b.at<float>(1,i));
    }
    return sqrt(dis);
}

double FeatureTracker::averageDistanse(const vector<DMatch> & match) {
    double dis = 0;
    for (int i = 0; i < match.size(); ++i) {
        dis += match[i].distance;
    }
    return dis / match.size();
}

template<typename T>
void FeatureTracker::pushQueue(deque<T> &queue, const T &anElement) {
    queue.push_front(anElement);
    queue.pop_back();
}

