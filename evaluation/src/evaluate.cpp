#include "evaluate.h"
#include "exception.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

// 2014-08-12 Simon Hofmann <mail@simon-hofmann.org>

using namespace std;
using namespace cv;

Evaluator::Evaluator(const char *cascade, const char *posFile, bool verbose) : _verbose(verbose)
{
    this->posFile = string(posFile);
    this->cascadeFile = string(cascade);

    if(this->_verbose)
    {
        cout << "Cascade file: " << this->cascadeFile << endl;
        cout << "Positive sample file: " << this->posFile << endl;
    }

    if(!(this->parsePositives(posFile)))
    {
        if(this->_verbose)
        {
            cout << "Successfully loaded " << this->positives.size() << " positive samples." << endl;
        }
    }
    else
    {
        if(this->_verbose)
        {
            cerr << "Unable to load positive samples." << endl;
        }
    }
}

Evaluator::~Evaluator()
{
  // cleanup
  this->freeMem();
}

void Evaluator::freeMem()
{
    for(int i = 0; i < this->positives.size(); ++i)
    {
        for(int j = 0; j < this->positives[i]->hits.size(); ++j)
        {
            delete this->positives[i]->hits[j];
            this->positives[i]->hits.clear();
        }
        delete this->positives[i];
        this->positives.clear();
    }
}

int Evaluator::parsePositives(const char *posFile)
{
    string input;
    ifstream inFile(posFile);

    int count = 0;

    if(inFile.is_open())
    {
        while(getline(inFile, input))
        {
            try
            {
                istringstream iss(input);
                positive *pos = new positive;
                
                try
                {
                    if(iss >> pos->filename >> pos->count)
                    {
                        for(int i = 0; i < pos->count; ++i)
                        {
                            Rect *coord = new Rect;
                            if(!(iss >> coord->x >> coord->y >> coord->width >> coord->height))
                            {
                                throw parseException("Error parsing coordinates!");
                            }
                            pos->hits.push_back(coord);
                        }

                        this->positives.push_back(pos);
                        ++count;
                    }
                    else
                    {
                        throw parseException("Error parsing file information!");
                    }
                }
                catch (parseException &pe)
                {
                    cerr << pe.what() << endl;
                    break;
                }
            }
            catch (bad_alloc &ba)
            {
                cerr << "Exception: " << ba.what() << endl;
                this->freeMem();
                return -1;
            }
        }
        inFile.close();
    }
    else
    {
        cerr << "Error opening file!" << endl;
    }

    return 0;
}

int Evaluator::evaluate()
{
    CascadeClassifier classify;
    if(!(classify.load(this->cascadeFile)))
    {
        cerr << "Unable to load cascade file!" << endl;
        return -1;
    }
    else
    {
        if(this->_verbose)
        {
            cout << "Cascade loaded!" << endl;
        }
    }

    // Loop over all positive samples
    for(int i = 0; i < this->positives.size(); ++i)
    {
        Mat inputImg = imread(this->positives[i]->filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        
        if(inputImg.empty())
        {
            cerr << "Unable to load sample!" << endl;
            return -1;
        }
        
        if(this->_verbose)
        {
            cout << "Processing file: " << this->positives[i]->filename.c_str() << endl;
        }
        
        // Stores bounding boxes of detected objects
        vector <Rect> detects;

        classify.detectMultiScale(inputImg, detects, 1.1, 2, 0|CASCADE_SCALE_IMAGE|CASCADE_DO_CANNY_PRUNING, Size(200, 200));

        // Loop over every detected object in an image
        for(int j = 0; j < detects.size(); ++j)
        {
            // Loop over every defined object in a positive sample
            for(int k = 0; k < this->positives[i]->hits.size(); ++k)
            {
                // Check local neighbourhood of defined positive for hits
                if(abs(this->positives[i]->hits[k]->x - detects[j].x) < this->positives[i]->hits[k]->width/2 and abs(this->positives[i]->hits[k]->y - detects[j].y) < this->positives[i]->hits[k]->width/2)
                {
                    if(this->_verbose)
                    {
                        cout << "Possible match" << endl;
                        cout << "dx: " << abs(this->positives[i]->hits[k]->x - detects[j].x) << " dy: " << abs(this->positives[i]->hits[k]->y - detects[j].y) << endl;
                    }
                    if(this->checkOverlap(*this->positives[i]->hits[k], detects[j]))
                    {
                        if(this->_verbose)
                        {
                            cout << "True hit." << endl;
                        }
                        this->positives[i]->no_hits++;
                    }
                    else
                    {
                        if(this->_verbose)
                        {
                            cout << "False positive." << endl;
                        }
                        this->positives[i]->no_false_positives++;
                    }
                }
            }

        }
        this->positives[i]->no_misses = this->positives[i]->count - this->positives[i]->no_hits;
    }
    this->showResults();
}

double Evaluator::checkOverlap(Rect positive, Rect detect)
{
    // Rect = Rect1 & Rect2: An easy way to calculate the intersection of two rectangles
    Rect intersection = positive & detect;

    // Check percentage of overlapping. >= 50% overlap -> true hit
    double overlap = (double)(intersection.width*intersection.height)/(positive.width * positive.height);
    
    if(overlap >= .5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Evaluator::showResults()
{
    cout << endl << "Evaluation done!" << endl;
    cout << endl;
    cout << "Hits:\t| Misses:\t| False positives:\t| File:" << endl;
    cout << "-------------------------------------------------------" << endl;
    for(int i = 0; i < this->positives.size(); ++i)
    {
        cout << this->positives[i]->no_hits << "\t| " << this->positives[i]->no_misses << "\t\t| " << this->positives[i]->no_false_positives << "\t\t\t| " << this->positives[i]->filename << endl;
    }
}
