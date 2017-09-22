//
// Created by xushen on 9/21/17.
//

#include "FeatureTracker.h"

FeatureTracker::FeatureTracker(string vName) {
    videoName = vName;
    vs.setVideo(vName);
    vs.setVideoWidth(640);
    vs.setVideoHeight(360);
    fe.setFeature("surf");
    fm.setDetecter(fe.detector);
}

void FeatureTracker::track() {
    if(initFrame.empty()){
        vs >> initFrame;
        fe.extract(initFrame, initKps, initDescs);
        lastFrame = initFrame.clone();
        lastKps = initKps;
        lastDescs = initDescs.clone();
        fm.match(initKps, initDescs, lastKps, lastDescs, matches4Init);
    }
    while(vs >> curtFrame){
        
    }
}
