#include "fileParser.h"
#include "exception.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

// 2014-08-12 Simon Hofmann <mail@simon-hofmann.org>

using namespace std;
using namespace cv;

fileParser::fileParser(string posFile)
{
    this->posFile = posFile;
}

fileParser::~fileParser()
{
  // cleanup
  this->freeMem();
}

void fileParser::freeMem()
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

int fileParser::parsePositives()
{
    string input;
    ifstream inFile(this->posFile.c_str());

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
                            // Vector containing samples
                            pos->hits.push_back(coord);
                            // Used to flag a sample as detected to avoid multiple detects.
                            pos->detected.push_back(false);
                        }

                        // Amount of successfully parsed samples
                        pos->count = pos->hits.size();

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
        return -1;
    }

    return count;
}
