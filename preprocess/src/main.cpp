#include "preprocess.h"

#include <exception>
#include <iostream>
#include <string>

using namespace std;

void showHelp();

int main(int argc, char **argv)
{
    string output;
    string positive;
    string background;

    int rmin = 0;
    int rmax = 360;
    int amount = 3;
    
    bool show = false;

    if(argc < 3)
    {
        showHelp();
        return -1;
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            if (!(strcmp(argv[i],"-output")) and (i+1 < argc))
            {
                output = argv[i+1];
            }
            else if (!(strcmp(argv[i],"-positives")) and (i+1 < argc))
            {
                positive = argv[i+1];
            }
            else if (!(strcmp(argv[i],"-background")) and (i+1 < argc))
            {
                background = argv[i+1];
            } 
            else if (!(strcmp(argv[i],"-s")))
            {
                show = true;
            }
            else if(!(strcmp(argv[i],"-rmin")) and (i+1 < argc))
            {
                rmin = atoi(argv[i+1]);
            }
            else if(!(strcmp(argv[i],"-rmax")) and (i+1 < argc))
            {
                rmax = atoi(argv[i+1]);
            }
            else if(!(strcmp(argv[i],"-no")))
            {
                amount = atoi(argv[i+1]);
            }
            else if(!(strcmp(argv[i],"-h")))
            {
                showHelp();
                return -1;
            }
        }
    
        if(!(output.empty() or positive.empty() or background.empty()))
        {
            if(amount > 0)
            {
                Preprocessor test(output, positive, background, amount, rmin, rmax, show);
                if(test.process())
                {
                    return 0;
                }
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
    cout << "-output <output directory>" << endl;
    cout << "-positives <positive samples file>" << endl;
    cout << "-background <background image file>" << endl;
    cout << "[-rmin <minimal rotation value>: Minimal rotation in degrees.]" << endl;
    cout << "[-rmax <maximum rotation value>: Maximum rotation in degrees.]" << endl;
    cout << "[-no <value>: Number of transformed images for each bounding box.]" << endl;
    cout << "[-s: Show mode, displays transformed images.]" << endl;
    cout << "-h: Help, displays the text you are reading right now." << endl;
}
