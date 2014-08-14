#include "evaluate.h"
#include "exception.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

// 2014-08-12 Simon Hofmann <mail@simon-hofmann.org>

using namespace std;
using namespace cv;

Evaluator::Evaluator(const char *cascade, const char *posFile)
{
    int pos = this->parsePositives(posFile);
    
    if(pos)
    {
        cout << "Successfully loaded " << pos << " positive samples." << endl;
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
                    }
                    else
                    {
                        throw parseException("Error parsing file information!");
                    }
                }
                catch (parseException &pe)
                {
                    cerr << pe.what() << endl;
                    this->freeMem();
                    break;
                }
                ++count;
            }
            catch (bad_alloc &ba)
            {
                cerr << "Exception: " << ba.what() << endl;
                this->freeMem();
                break;
            }
        }
    }
    else
    {
        cerr << "Error opening file!" << endl;
    }

    inFile.close();

    return count;
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
