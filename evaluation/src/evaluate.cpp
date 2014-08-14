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

Evaluator::Evaluator(const char *cascade, const char *posFile)
{
    this->posFile = string(posFile);
    this->cascadeFile = string(cascade);

    cout << "Cascade file: " << this->cascadeFile << endl;
    cout << "Positive sample file: " << this->posFile << endl;

    if(!(this->parsePositives(posFile)))
    {
        cout << "Successfully loaded " << this->positives.size() << " positive samples." << endl;
    }
    else
    {
        cerr << "Unable to load positive samples." << endl;
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
                            hit *coord = new hit;
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
        cout << "Cascade loaded!" << endl;
    }

    for(int i = 0; i < this->positives.size(); ++i)
    {
        Mat inputImg = imread(this->positives[i]->filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        
        if(inputImg.empty())
        {
            cerr << "Unable to load sample!" << endl;
            return -1;
        }
        
        cout << "Processing file: " << this->positives[i]->filename.c_str() << endl;
        
        // Stores bounding boxes of detected objects
        vector <Rect> detects;

        classify.detectMultiScale(inputImg, detects, 1.1, 2, 0|CASCADE_SCALE_IMAGE|CASCADE_DO_CANNY_PRUNING, Size(200, 200));
        for(int j = 0; j < detects.size(); ++j)
        {
            for(int k = 0; k < this->positives[i]->hits.size(); ++k)
            {
                if(abs(this->positives[i]->hits[k]->x - detects[j].x) < 50 and abs(this->positives[i]->hits[k]->y - detects[j].y) < 50)
                {
                    //TODO: Check overlap of hits
                    cout << "Possible match" << endl;
                    cout << "dx: " << abs(this->positives[i]->hits[k]->x - detects[j].x) << " dy: " << abs(this->positives[i]->hits[k]->y - detects[j].y) << endl;
                }
            }
        }
    }
    
    cout << "Evaluation done." << endl;
}

Evaluator::Evaluator(const Evaluator& other)
{
  *this=other;
}

Evaluator& Evaluator::operator=(const Evaluator& other)
{
  if (this != &other){  // no self-assignmet
    // deep copy
  }
  return *this;
}
