#include "evaluate.h"

#include <exception>
#include <iostream>
#include <string>

using namespace std;

void showHelp();

int main(int argc, char **argv)
{
    string cascade;
    string positive;

    bool verbose = false;
    bool show = false;

    double neighbours = 3;
    double scale = 1.1;

    double min_height = 0;
    double max_height = 0;
    double min_width = 0;
    double max_width = 0;

    if(argc < 2)
    {
        showHelp();
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
                return 1;
            }
        }

        if(!(cascade.empty() or positive.empty()))
        {
            if(max_width >= 0 and max_height >= 0 and min_width >= 0 and min_height >= 0)
            {
                if((max_width >= min_width and max_height >= min_height) or (max_width == 0 and max_height == 0))
                {
                    cv::Size min_size(min_width, min_height);
                    cv::Size max_size(max_width, max_height);
                    Evaluator eval(cascade.c_str(), positive.c_str(), verbose, show, scale, neighbours, min_size, max_size);
                    if(!(eval.evaluate()))
                    {
                        return 0;
                    }
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
            return 1;
        }
    }
}

void showHelp()
{
    cout << "Usage: opencv_evaluate" << endl;
    cout << "-cascade <cascade classifier *.xml file>" << endl;
    cout << "-positives <positive samples file>" << endl;
    cout << "[-scale <scaling factor>: Parameter specifying how much the image size is reduced at each image scale.]" << endl;
    cout << "[-neighbours <count>: Parameter specifying how many neighbors each candidate rectangle should have to retain it." << endl; 
    cout << "[-minw <minimal width>: Minimum object width, smaller objects will be discarded.]" << endl;
    cout << "[-minh <minimal height>: Minimum object height, smaller objects will be discarded.]" << endl;
    cout << "[-maxw <minimal width>: Maximum object width, larger objects will be discarded.]" << endl;
    cout << "[-maxh <minimal width>: Maximum object height, larger objects will be discarded.]" << endl;
    cout << "[-v: Verbose mode, detailed output.]" << endl;
    cout << "[-s: Show mode, displays sample images showing bounding boxes of detected and defined objects.]" << endl;
    cout << "-h: Help, displays the text you are reading right now." << endl;
}
