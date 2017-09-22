//
// Created by xushen on 9/18/17.
//

#ifndef DESCSINMUTIVIEWS_RANSAC_H
#define DESCSINMUTIVIEWS_RANSAC_H

#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

class FeatureMatcher {
private:
    // pointer to the feature point detector object
    // pointer to the feature descriptor extractor object
    cv::Ptr<cv::Feature2D> detector;

    // pointer to the matcher object
    cv::Ptr<cv::DescriptorMatcher > matcher;

    float ratio; // max ratio between 1st and 2nd NN
    bool refineF; // if true will refine the F matrix
    double distance; // min distance to epipolar
    double confidence; // confidence level (probability)

public:
    FeatureMatcher(): ratio(0.65f), refineF(true), confidence(0.99), distance(3.0) {
        // SURF is the default feature detector and extractor
        detector = cv::xfeatures2d::SURF::create();
        // BruteForce matcher is the default matcher
        matcher.reset(new cv::BFMatcher(cv::NORM_L2));
    }

    void setDetecter(cv::Ptr<cv::Feature2D> dt);

    // Set confidence level
    void setConfidenceLevel(double conf);

    //Set MinDistanceToEpipolar
    void setMinDistanceToEpipolar(double dist);

    //Set ratio
    void setRatio(float rat);

    // Clear matches for which NN ratio is > than threshold
    // return the number of removed points
    // (corresponding entries being cleared,
    // i.e. size will be 0)
    int ratioTest(std::vector<std::vector<cv::DMatch> >
                  &matches);

    // Insert symmetrical matches in symMatches vector
    void symmetryTest(const std::vector<std::vector<cv::DMatch> >& matches1,
                      const std::vector<std::vector<cv::DMatch> >& matches2,
                      std::vector<cv::DMatch>& symMatches);

    // Identify good matches using RANSAC
    // Return fundamental matrix
    cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
                       const std::vector<cv::KeyPoint>& keypoints1,
                       const std::vector<cv::KeyPoint>& keypoints2,
                       std::vector<cv::DMatch>& outMatches,
                       std::vector<cv::Point2f>& points1,
                       std::vector<cv::Point2f>& points2);

    // Match feature points using symmetry test and RANSAC
    // returns fundamental matrix
    cv::Mat match(cv::Mat& image2, // input scene image
                  std::vector<cv::KeyPoint>& keypoints1, // input computed object keypoints
                  cv::Mat& descriptors1, // input computed object descriptors
                  std::vector<cv::DMatch>& matches, // output matches
                  std::vector<cv::Point2f>& points1, // output object keypoints (Point2f)
                  std::vector<cv::Point2f>& points2); // output scene keypoints (Point2f)

    cv::Mat match(std::vector<cv::KeyPoint>& keypoints1, // input computed object keypoints
                  cv::Mat& descriptors1, // input computed object descriptors
                  std::vector<cv::KeyPoint>& keypoints2, // input computed object keypoints
                  cv::Mat& descriptors2, // input computed object descriptors
                  std::vector<cv::DMatch>& matches);

    // draw matched keypoints
    void drawMatches(cv::Mat& image, // output image
                     std::vector<cv::DMatch>& matches, // matches
                     std::vector<cv::Point2f>& scene); // scene keypoints (Point2f)

    // draw epipolar lines
    void drawEpipolarLines(cv::Mat& image_out, // output image
                           cv::Mat& image1, // image 1
                           cv::Mat& image2, // image 2
                           std::vector<cv::Point2f>& points1, // keypoints 1
                           std::vector<cv::Point2f>& points2, // keypoints 2)
                           int whichImage); // image to compute epipolar lines in

    void drawPerspective(cv::Mat& image, // output image
                         cv::Mat& image1, // input (object) image
                         std::vector<cv::Point2f>& points1, // keypoints 1
                         std::vector<cv::Point2f>& points2); // keypoints 2
};


#endif //DESCSINMUTIVIEWS_RANSAC_H
