#include "preprocess.h"

#include <exception>
#include <iostream>
#include <string>

using namespace std;

void showHelp();

int main(int argc, char **argv)
{
    bool verbose = false;
    bool show = false;

    if(argc < 2)
    {
        showHelp();
    }
    else
    {
        Preprocessor test(argv[1], argv[2]);
        cv::Size tmp = cv::Size(1000, 500);
        cv::Mat test_mat = test.tile(test.bckg, tmp);

        cv::namedWindow("output");
        cv::imshow("output", test_mat);
        cv::waitKey(0);
        //cout << test.process() << endl;

        //for (int i = 1; i < argc; i++)
        //{
            //if (!(strcmp(argv[i],"-cascade")) and (i+1 < argc))
            //{
                //cascade = argv[i+1];
            //}
            //else if (!(strcmp(argv[i],"-positives")) and (i+1 < argc))
            //{
                //positive = argv[i+1];
            //}
            //else if (!(strcmp(argv[i],"-v")))
            //{
                //verbose = true;
            //} 
            //else if (!(strcmp(argv[i],"-s")))
            //{
                //show = true;
            //}
            //else if (!(strcmp(argv[i],"-scale")) and (i+1 < argc))
            //{
                //scale = atof(argv[i+1]);
            //}
            //else if (!(strcmp(argv[i],"-neighbours")) and (i+1 < argc))
            //{
                //neighbours = atof(argv[i+1]);
            //}
            //else if (!(strcmp(argv[i],"-percent")))
            //{
                //percent = true;
            //}
            //else if (!(strcmp(argv[i],"-minw")) and (i+1 < argc))
            //{
                //min_width = atof(argv[i+1]);
            //}
            //else if (!(strcmp(argv[i],"-minh")) and (i+1 < argc))
            //{
                //min_height = atof(argv[i+1]);
            //}
            //else if (!(strcmp(argv[i],"-maxw")) and (i+1 < argc))
            //{
                //max_width= atof(argv[i+1]);
            //}
            //else if (!(strcmp(argv[i],"-maxh")) and (i+1 < argc))
            //{
                //max_height = atof(argv[i+1]);
            //}
            //else if (!(strcmp(argv[i],"-h")))
            //{
                //showHelp();
                //return 1;
            //}
    }

    return 0;
}

void showHelp()
{
    cout << "Usage: opencv_evaluate" << endl;
    cout << "-cascade <cascade classifier *.xml file>" << endl;
    cout << "-positives <positive samples file>" << endl;
    cout << "[-scale <scaling factor>: Parameter specifying how much the image size is reduced at each image scale.]" << endl;
    cout << "[-neighbours <count>: Parameter specifying how many neighbors each candidate rectangle should have to retain it.]" << endl; 
    cout << "[-percent: If this flag is activated minimum and maximum object size are given as a percentual value between 0 and 1.]" << endl; 
    cout << "[-minw <minimal width>: Minimum object width, smaller objects will be discarded.]" << endl;
    cout << "[-minh <minimal height>: Minimum object height, smaller objects will be discarded.]" << endl;
    cout << "[-maxw <minimal width>: Maximum object width, larger objects will be discarded.]" << endl;
    cout << "[-maxh <minimal width>: Maximum object height, larger objects will be discarded.]" << endl;
    cout << "[-v: Verbose mode, detailed output.]" << endl;
    cout << "[-s: Show mode, displays sample images showing bounding boxes of detected and defined objects.]" << endl;
    cout << "-h: Help, displays the text you are reading right now." << endl;
}
