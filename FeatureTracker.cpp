//
// Created by xushen on 9/21/17.
//

#include "FeatureTracker.h"
#include <ctime>

FeatureTracker::FeatureTracker(string vName) {
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
    if(initFrame.empty()){
        vs >> initFrame;
        fe.extract(initFrame, initKps, initDescs);
        lastFrame = initFrame.clone();
        lastKps = initKps;
        lastDescs = initDescs.clone();
        fm.match(initKps, initDescs, lastKps, lastDescs, matches4Init);
    }
    Mat matchFrame;
    Mat matchInitFrame;
    while(vs >> curtFrame){
        //clock_t begin = clock();
        imshow(videoName, curtFrame);
        int key = cvWaitKey((int)(vs.getFrameRate()+0.5) - 20);
        switch (key){
            case 's':
            {
                initFrame = curtFrame.clone();
                fe.extract(initFrame, initKps, initDescs);
                lastFrame = initFrame.clone();
                lastKps = initKps;
                lastDescs = initDescs.clone();
                fm.match(initKps, initDescs, lastKps, lastDescs, matches4Init);
                break;
            }
            case 'p':
            {
                cvWaitKey(0);
                break;
            }
            case 'e':
            {
                return;
            }
            default:
            {

            }
        }
        fe.extract(curtFrame, curtKps, curtDescs);
        //cout << "extract: " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
        fm.match(lastKps, lastDescs, curtKps, curtDescs, matches4CurtFrame);
        //cout << "match: " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
        cv::drawMatches(lastFrame, lastKps, curtFrame, curtKps, matches4CurtFrame, matchFrame);
        imshow("Matches", matchFrame);

        vector<DMatch> tmp;
        forwardMatches(initDescs, curtDescs, matches4Init, matches4CurtFrame, tmp);
        matches4Init = tmp;
        cout << tmp[0].distance << endl;
        drawMatches(initFrame, initKps, curtFrame, curtKps, matches4Init, matchInitFrame);
        imshow("MatchWithInit", matchInitFrame);

        lastFrame = curtFrame.clone();
        lastKps.clear();
        lastKps = curtKps;
        lastDescs = curtDescs.clone();
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

//void FeatureTracker::MergeMatches(vector<DMatch> &matchABOld, vector<DMatch> &matchABNew,
//                                    vector<DMatch> &matchAB) {
//    matchAB.clear();
//    for(int i = 0; i < matchABOld.size(); i++ ){
//        for(int j = 0; j < matchABNew.size(); j++){
//            if(matchABOld[i].trainIdx == matchABNew[j].queryIdx){
//                float dis = static_cast<float>(calL2Distance(descA.row(i), descC.row(j)));
//                matchAC.push_back(DMatch(matchAB[i].queryIdx, matchBC[j].trainIdx, dis));
//                continue;
//            }
//        }
//    }
//}

double FeatureTracker::calL2Distance(const Mat &a, const Mat &b) {
    assert(a.rows == 1 && b.rows == 1 && a.cols == b.cols);
    double dis = 0;
    for(int i = 0; i < a.rows; i++){
        dis += (a.at<float>(1,i) - b.at<float>(1,i)) * (a.at<float>(1,i) - b.at<float>(1,i));
    }
    return sqrt(dis);
}

