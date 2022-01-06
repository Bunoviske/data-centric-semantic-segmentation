#include "watershed.hpp"


// #define WIDTH 250
// #define HEIGHT 250
#define LINE_THICKNESS 2

void chooseFoods(int event, int x, int y, int flags, void *objeto);        
void chooseWshedMarkers(int event, int x, int y, int flags, void *objeto); 
void createGroundTruth(int index, int food);
string getFileNameWithoutExtension(string filepath);
string getFileNameWithExtension(string filepath);
int runManualSemanticSegmentation(Mat image, string filepath);
void getImagesAndRun(string folder);
void executeMkdir(string mkdir);
void executeMoveFile(string imageName, string destinationFolder);
void readLabelsTxtAndCreateMap(string filepath);

Watershed wshed;
//watershed opencv global variables
Mat markerMask, img, img0, groundTruth;
Mat imgGray;
Mat markers = Mat(img.size(), CV_32S);
Mat wshedMat = Mat(img.size(), CV_8UC3);
Point prevPt(-1, -1);

string IMAGES_FOLDER;
map<string, string> labels;

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        IMAGES_FOLDER = argv[1];
        readLabelsTxtAndCreateMap(argv[2]);
    }
    else
    {
        cout << "Insert your images folder path and the label ids file path" << endl;
        return -1;
    }

    executeMkdir("mkdir " + IMAGES_FOLDER + "/gt");
    executeMkdir("mkdir " + IMAGES_FOLDER + "/done");

    getImagesAndRun(IMAGES_FOLDER);

    return 0;
}
void readLabelsTxtAndCreateMap(string filepath)
{
    std::ifstream file(filepath);
    std::string str;
    while (std::getline(file, str))
    {
        int pos = str.find(": ");
        string id = str.substr(0, pos);
        string foodName = str.substr(pos + 2, str.length());
        //cout << id << " " << foodName << endl;
        labels.insert({id, foodName});
    }
}

void executeMkdir(string mkdir)
{
    try
    {
        system(mkdir.c_str());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void executeMoveFile(string imageName, string destinationFolder)
{
    //destination folder should be 'gt' or 'done'
    try
    {
        string mv = "mv " + IMAGES_FOLDER + '/' + imageName + ' ' + IMAGES_FOLDER + '/' + destinationFolder;
        system(mv.c_str());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void getImagesAndRun(string folder)
{
    vector<String> filepaths;

    glob(folder, filepaths);
    cout << "Total images: " << filepaths.size() << endl;

    for (size_t i = 0; i < filepaths.size(); ++i)
    {
        Mat src = imread(filepaths[i]);

        if (!src.data)
            cerr << "Problem loading image!!!" << endl;
        else
        {
            cout << filepaths[i] << endl;
            runManualSemanticSegmentation(src, filepaths[i]);
        }
    }
}

int runManualSemanticSegmentation(Mat image, string filepath)
{

    //resize(image, image, Size(WIDTH, HEIGHT));
    img = image.clone();

    cvtColor(img, markerMask, COLOR_BGR2GRAY);
    cvtColor(markerMask, imgGray, COLOR_GRAY2BGR);
    markerMask = Scalar::all(0);
    groundTruth = markerMask.clone();

    img0 = img.clone();
    namedWindow("plate", WINDOW_AUTOSIZE);
    resizeWindow("plate", img0.cols, img0.rows);
    namedWindow("wshed", WINDOW_AUTOSIZE);
    resizeWindow("wshed", img0.cols, img0.rows);
    imshow("plate", img0);
    setMouseCallback("plate", chooseWshedMarkers, NULL);

    //////////////////////// step one : watershed segmentation
    while (1)
    {
        char c = waitKey();
        //if you want to exit, press esc
        if ((char)c == 27) //esc
        {
            return -1;
        }
        if (c == 'c') // 'classification': classify segmented objects
        {
            img = image.clone();
            markers = wshed.runWatershed(&img, &markerMask, &markers, &wshedMat, imgGray);
            break;
        }
        if (c == 'w') // 'watershed' : run watershed algorithm
        {
            img = image.clone();
            markers = wshed.runWatershed(&img, &markerMask, &markers, &wshedMat, imgGray);
        }
        if (c == 'r') // 'reload': clear all segmentations
        {
            img0 = image.clone();
            img = image.clone();
            markerMask = Scalar::all(0);
            imshow("plate", img0);
        }
    }
    setMouseCallback("wshed", chooseFoods, NULL);
    cout << "********************** Classify each object (look your label ids file) **********************" << endl;
    //////////////////////// step two : classify the segmented objects
    while (1)
    {
        char c = waitKey();
        //if you want to exit, press esc
        if ((char)c == 27) //esc
        {
            return -1;
        }
        if (c == 'n') // 'next': finish the annotation from this image and go to the next one
        {
            break;
        }
    }

    //save GT image inside "gt" folder and move the original image to "done" folder
    string groundTruthFile = getFileNameWithoutExtension(filepath) + "_GT.png";
    imwrite(IMAGES_FOLDER + "/gt/" + groundTruthFile, groundTruth);
    executeMoveFile(getFileNameWithExtension(filepath), "done");

    groundTruth = Scalar::all(0);
    return 0;
}

void chooseWshedMarkers(int event, int x, int y, int flags, void *objeto)
{
    if (x < 0 || x >= img.cols || y < 0 || y >= img.rows)
        return;
    if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
        prevPt = Point(-1, -1);
    else if (event == EVENT_LBUTTONDOWN)
        prevPt = Point(x, y);
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
    {
        Point pt(x, y);
        if (prevPt.x < 0) //primeiro clique ou primeiro ponto!
            prevPt = pt;
        line(markerMask, prevPt, pt, Scalar::all(255), LINE_THICKNESS, 8, 0);
        line(img0, prevPt, pt, Scalar::all(255), LINE_THICKNESS, 8, 0);
        prevPt = pt;
        imshow("plate", img0);
    }
}

void chooseFoods(int event, int x, int y, int flags, void *objeto)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        int foodIndex = wshed.getFoodRegionIndex(Point(x, y)) + 1; //soma um pois essa funcao retorna o index - 1
        int foodId;
        cout << "Digite o id do alimento: " << flush;
        cin >> foodId;
        cout << foodId << endl;
        string foodName = labels.find(to_string(foodId))->second;  
        cout <<  foodName << endl;
        putText(img0,         //target image
                foodName, //text
                Point(x, y),      //top-left position
                cv::FONT_HERSHEY_DUPLEX,
                0.8,
                CV_RGB(0, 0, 255), //font color
                2.5);
        imshow("plate", img0);
        createGroundTruth(foodIndex, foodId);
    }
}

void createGroundTruth(int imageIndex, int food)
{
    //cout << imageIndex << endl;

    for (size_t i = 0; i < groundTruth.rows; i++)
    {
        for (size_t j = 0; j < groundTruth.cols; j++)
        {
            if (markers.at<int>(i, j) == imageIndex)
            {
                groundTruth.at<uchar>(i, j) = (uint8_t)food;
            }
        }
    }
}

string getFileNameWithoutExtension(string filepath)
{
    std::filesystem::path p(filepath);
    return p.stem();
}

string getFileNameWithExtension(string filepath)
{
    std::filesystem::path p(filepath);
    return p.filename();
}
