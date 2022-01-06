#include "watershed.hpp"

Vec3b colorTab[] =
    {

        Vec3b(0, 0, 255),
        Vec3b(0, 255, 0),
        Vec3b(255, 0, 0),

        Vec3b(255, 0, 255),
        Vec3b(0, 255, 255),
        Vec3b(255, 255, 0),

        Vec3b(255, 100, 100),
        Vec3b(100, 0, 255),
        Vec3b(100, 255, 0),
        Vec3b(255, 0, 100),

        Vec3b(100, 255, 100),
        Vec3b(100, 100, 255),

        Vec3b(255, 255, 100),
        Vec3b(255, 100, 255),
        Vec3b(100, 255, 255),

        Vec3b(0, 255, 100),
        Vec3b(255, 100, 0),
        Vec3b(0, 100, 255),

};

Mat Watershed::runWatershed(Mat *img0,
                            Mat *maskMarkers,
                            Mat *markers,
                            Mat *wshed,
                            Mat imgGray)
{
    //cout << "Comecando wshed" << endl;

    // FILTERING
    medianBlur(*img0, *img0, 7);

    int i, j, compCount = 0; //NUMEROS DE REGIOES DIFERENTES MARCADAS (compCount)!
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(*maskMarkers, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        return Mat();

    markers = new Mat(maskMarkers->size(), CV_32S, Scalar(0));

    int idx = 0;
    for (; idx >= 0; idx = hierarchy[idx][0], compCount++)
    {
        //preenche dentro do contorno. essa etapa foi feita para detectar a qtd de contornos
        drawContours(*markers, contours, idx, Scalar::all(compCount + 1), -1, 8, hierarchy, INT_MAX);
    }
    if (compCount == 0)
        return Mat();

    //    if (compCount > 18) {
    //
    //        vector<Vec3b> colorTab;
    //
    //        for (i = 0; i < compCount; i++) {
    //            int b = theRNG().uniform(0, 255);
    //            int g = theRNG().uniform(0, 255);
    //            int r = theRNG().uniform(0, 255);
    //
    //            colorTab.push_back(Vec3b((uchar) b, (uchar) g, (uchar) r));
    //        }
    //    }

    watershed(*img0, *markers);

    wshed = new Mat(markers->size(), CV_8UC3);

    // paint the watershed image
    for (i = 0; i < markers->rows; i++)
        for (j = 0; j < markers->cols; j++)
        {
            int index = markers->at<int>(i, j);
            if (index == -1)
            { //representa as bordas. é necessario remover o valor de -1 e atribuir o valor da vizinhança
                int offset = 3;
                Rect roi = Rect(max(0, j - offset), max(0, i - offset), min(markers->cols - 1, j + offset) - max(0, j - offset), min(markers->rows - 1, i + offset) - max(0, i - offset));
                Mat neighbors = (*markers)(roi);
                for (int ii = 0; ii < neighbors.rows; ii++){
                    for (int jj = 0; jj < neighbors.cols; jj++)
                    {
                        int closeIndex = neighbors.at<int>(ii, jj);
                        if (closeIndex != -1)
                        {
                            markers->at<int>(i, j) = closeIndex;
                            wshed->at<Vec3b>(i, j) = colorTab[closeIndex - 1];
                            ii = 10000; jj = 10000; //break loop
                        }
                    }
                }
                // wshed->at<Vec3b>(i, j) = Vec3b(255, 255, 255);
            }
            else if (index <= 0 || index > compCount)
            {
                wshed->at<Vec3b>(i, j) = Vec3b(0, 0, 0); //erro
            }
            else
            { //regiao segmentada
                wshed->at<Vec3b>(i, j) = colorTab[index - 1];
            }
        }

    *wshed = (*wshed) * 0.5 + imgGray * 0.5;
    imshow("wshed", *wshed);
    mMarkers = markers->clone();
    getAllFoodRegionsArea(*markers, compCount);
    return mMarkers;
}

void Watershed::getAllFoodRegionsArea(Mat markers, int numberOfRegions)
{

    foodAreas.reserve(numberOfRegions);
    for (int i = 0; i < numberOfRegions; i++)
    {
        foodAreas[i] = 0;
    }

    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i, j);
            if (index > 0)
            {
                foodAreas[index - 1]++;
            }
        }
    }
}

int Watershed::getFoodRegionArea(Point point)
{

    int index = getFoodRegionIndex(point);
    return foodAreas[index];
}

int Watershed::getFoodRegionIndex(Point point)
{
    int pos = mMarkers.at<int>(point.y, point.x);
    return pos - 1;
}