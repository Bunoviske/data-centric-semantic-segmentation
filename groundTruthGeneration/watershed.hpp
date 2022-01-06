#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/flann/flann.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <fstream>
#include <vector>
#include <filesystem>
#include <map>
#include <algorithm>

using namespace cv;
using namespace std;

class Watershed
{

public:
    Watershed(){};
    Watershed(Mat markers, int numberOfRegions)
    {
        mMarkers = markers;
        getAllFoodRegionsArea(markers, numberOfRegions);
    };

    Mat runWatershed(Mat *img0, Mat *maskMarkers, Mat *markers, Mat *wshed,Mat imgGray);
    void getAllFoodRegionsArea(Mat markers, int numberOfRegions);
    int getFoodRegionArea(Point point);
    int getFoodRegionIndex(Point point);

private:
    Mat mMarkers;
    int numComp;
    vector<int> foodAreas;
};
