#include "evaluate.h"
#include "exception.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

// 2014-08-12 Simon Hofmann <mail@simon-hofmann.org>

using namespace std;
using namespace cv;

Evaluator::Evaluator(const char *cascade, const char *posFile)
{
    string input;
    ifstream inFile(posFile);

    int count = 1;

    if(inFile.is_open())
    {
        while(getline(inFile, input))
        {
            try
            {
                istringstream iss(input);
                positive *pos = new positive;

                //TODO: Throw exception here
                if(iss >> pos->filename >> pos->count)
                {
                    for(int i = 0; i < pos->count; ++i)
                    {
                        hit *coord = new hit;
                        iss >> coord->x >> coord->y >> coord->width >> coord->height;
                        pos->hits.push_back(coord);
                    }

                    this->positives.push_back(pos);
                }
                else
                {
                    cerr << "Error parsing data ..." << endl;
                }
                ++count;
            }
            catch (exception &e)
            {
                cerr << "Exception: " << e.what() << endl;
            }
        }
    }
    else
    {
        cerr << "Error opening file!" << endl;
    }

    inFile.close();
}

Evaluator::~Evaluator()
{
  // cleanup
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
