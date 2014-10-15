#include "evaluate.h"

#include <exception>
#include <iostream>
#include <string>

using std::cout; 
using std::cerr; 
using std::endl;
using std::string;

void showHelp();

int main(int argc, char **argv)
{
    string cascade;
    string positive;

    bool verbose = false;
    bool show = false;
    bool percent = false;

    double neighbours = 3;
    double scale = 1.1;

    double min_height = 0;
    double max_height = 0;
    double min_width = 0;
    double max_width = 0;

    if(argc < 2)
    {
        showHelp();
        return -1;
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            if (!(strcmp(argv[i],"-cascade")) and (i+1 < argc))
            {
                cascade = argv[i+1];
            }
            else if (!(strcmp(argv[i],"-positives")) and (i+1 < argc))
            {
                positive = argv[i+1];
            }
            else if (!(strcmp(argv[i],"-v")))
            {
                verbose = true;
            } 
            else if (!(strcmp(argv[i],"-s")))
            {
                show = true;
            }
            else if (!(strcmp(argv[i],"-scale")) and (i+1 < argc))
            {
                scale = atof(argv[i+1]);
            }
            else if (!(strcmp(argv[i],"-neighbours")) and (i+1 < argc))
            {
                neighbours = atof(argv[i+1]);
            }
            else if (!(strcmp(argv[i],"-percent")))
            {
                percent = true;
            }
            else if (!(strcmp(argv[i],"-minw")) and (i+1 < argc))
            {
                min_width = atof(argv[i+1]);
            }
            else if (!(strcmp(argv[i],"-minh")) and (i+1 < argc))
            {
                min_height = atof(argv[i+1]);
            }
            else if (!(strcmp(argv[i],"-maxw")) and (i+1 < argc))
            {
                max_width= atof(argv[i+1]);
            }
            else if (!(strcmp(argv[i],"-maxh")) and (i+1 < argc))
            {
                max_height = atof(argv[i+1]);
            }
            else if (!(strcmp(argv[i],"-h")))
            {
                showHelp();
                return -1;
            }
        }

        if(!(cascade.empty() or positive.empty()))
        {
            if(max_width >= 0 and max_height >= 0 and min_width >= 0 and min_height >= 0)
            {
                if((max_width >= min_width and max_height >= min_height) or (max_width == 0 and max_height == 0))
                {
                    if(!percent or (percent and min_width <= 1 and min_height <= 1 and max_width <= 1 and max_height <= 1))
                    {
                        cv::Size_<double> min_size(min_width, min_height);
                        cv::Size_<double> max_size(max_width, max_height);

                        Evaluator eval(cascade, positive, verbose, show, scale, neighbours, min_size, max_size, percent);
                        if(!(eval.evaluate()))
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        cerr << "Percentual sizes have to be [0 <= $value <= 1]." << endl;
                        return -1;
                    }
                }
                else
                {
                    cerr << "Maximum size values must be greater than minimum size values." << endl;
                    return -1;
                }
            }
            else
            {
                cerr << "Bad size argument!" << endl;
                return -1;
            }
        }
        else
        {
            cerr << "Arguments missing!" << endl;
            showHelp();
            return -1;
        }
    }
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
