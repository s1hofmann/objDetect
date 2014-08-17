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

Evaluator::Evaluator(const char *cascade, const char *posFile, bool verbose, bool show, double scale, double neighbours, Size min, Size max) : _verbose(verbose), _show(show), _scale(scale), _neighbours(neighbours), _min(min), _max(max)
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

        this->positives[i]->no_misses = this->positives[i]->count;
        
        if(inputImg.empty())
        {
            cerr << "Unable to load sample!" << endl;
            return -1;
        }
        
        if(this->_verbose)
        {
            cout << "Processing file: " << this->positives[i]->filename.c_str() << endl;
        }
        
        // CASCADE_DO_CANNY_PRUNING: Function uses Canny edge detector to reject some image regions that contain too few or too much edges and thus can not contain the searched object
        classify.detectMultiScale(inputImg, this->detects, this->_scale, this->_neighbours, CASCADE_SCALE_IMAGE|CASCADE_DO_CANNY_PRUNING, this->_min, this->_max);

        // Loop over every detected object in an image
        for(int j = 0; j < this->detects.size(); ++j)
        {
            // Loop over every defined object in a positive sample
            for(int k = 0; k < this->positives[i]->hits.size(); ++k)
            {
                // Check local neighbourhood of defined positive for hits
                if(abs(this->positives[i]->hits[k]->x - this->detects[j].x) < this->positives[i]->hits[k]->width/2 and abs(this->positives[i]->hits[k]->y - this->detects[j].y) < this->positives[i]->hits[k]->width/2)
                {
                    if(this->_verbose)
                    {
                        cout << "Possible match" << endl;
                        cout << "dx: " << abs(this->positives[i]->hits[k]->x - this->detects[j].x) << " dy: " << abs(this->positives[i]->hits[k]->y - this->detects[j].y) << endl;
                    }
                    if(this->checkOverlap(*this->positives[i]->hits[k], this->detects[j]))
                    {
                        if(this->_verbose)
                        {
                            cout << "True hit." << endl;
                        }
                        
                        if(this->_show)
                        {
                            // Black bounding box for detected objects
                            rectangle(inputImg, Point(this->detects[j].x, this->detects[j].y), Point(this->detects[j].x+this->detects[j].width, this->detects[j].y+this->detects[j].height), Scalar(0,0,0), 5);
                            // White bounding box for defined objects
                            rectangle(inputImg, Point(this->positives[i]->hits[k]->x, this->positives[i]->hits[k]->y), Point(this->positives[i]->hits[k]->x+this->positives[i]->hits[k]->width, this->positives[i]->hits[k]->y+this->positives[i]->hits[k]->height), Scalar(255,255,255), 5);
                        }

                        this->positives[i]->no_hits++;
                        this->positives[i]->no_misses--;
                        if(this->positives[i]->no_misses < 0)
                        {
                            this->positives[i]->no_misses = 0;
                        }
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

        // Display image showing defined and detected objects
        if(this->_show)
        {
            resize(inputImg, inputImg, Size(1280,800), 1, 1);
            cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
            imshow("Output", inputImg);

            waitKey(0);

            inputImg.release();
        }
        else
        {
            inputImg.release();
        }
    }
    this->showResults();
    return 0;
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
    long total_hits = 0;
    long total_misses = 0;
    long total_false_positives = 0;

    cout << "Evaluation done!" << endl;
    cout << endl;
    cout << "Hits:\t| Misses:\t| False positives:\t| File:" << endl;
    cout << "-------------------------------------------------------" << endl;
    for(int i = 0; i < this->positives.size(); ++i)
    {
        total_hits += this->positives[i]->no_hits;
        total_misses += this->positives[i]->no_misses;
        total_false_positives += this->positives[i]->no_false_positives;

        cout << this->positives[i]->no_hits << "\t| " << this->positives[i]->no_misses << "\t\t| " << this->positives[i]->no_false_positives << "\t\t\t| " << this->positives[i]->filename << endl;
    }
    cout << "-------------------------------------------------------" << endl;
    cout << endl << endl;
    cout << "Evaluated " << this->positives.size() << " samples." << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "Total hits:\t| Total misses:\t| Total false positives:" << endl;
    cout << total_hits << "\t\t| " << total_misses << "\t\t| " << total_false_positives << endl;
    cout << "-------------------------------------------------------" << endl;
}
