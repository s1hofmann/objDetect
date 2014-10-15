#include "evaluate.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <ctime>

// 2014-08-12 Simon Hofmann <mail@simon-hofmann.org>

using std::cout;
using std::cerr;
using std::endl;
using std::fixed;
using std::string;
using std::out_of_range;

using namespace cv;

Evaluator::Evaluator(string cascade, string positive, bool verbose, bool show, double scale, double neighbours, Size_<double> min, Size_<double> max, bool percent) : posFile(positive), cascadeFile(cascade), verbose_(verbose), show_(show), scale_(scale), neighbours_(neighbours), min_(min), max_(max), percent_(percent)
{
    this->parser = new fileParser(this->posFile.c_str());

    int count = this->parser->parsePositives();

    if(this->verbose_)
    {
        cout << "Cascade file: " << this->cascadeFile << endl;
        cout << "Positive sample file: " << this->posFile << endl;
        cout << "--" << endl;
        cout << "Parsed " << count << " samples." << endl;
    }
}

Evaluator::~Evaluator()
{
  // cleanup
  delete this->parser;
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
        if(this->verbose_)
        {
            cout << "Cascade loaded!" << endl;
        }
    }

    time_t start_time, end_time;

    if(!(this->show_))
    {
        time(&start_time);
    }

    // Loop over all positive samples
    for(int i = 0; i < this->parser->positives.size(); ++i)
    {
        Mat inputImg = imread(this->parser->positives[i]->filename.c_str(), IMREAD_GRAYSCALE);
        Size_<double> min, max;

        // Initial amount of missed object == total amount of available objects
        this->parser->positives[i]->no_misses = this->parser->positives[i]->hits.size();
        
        if(inputImg.empty())
        {
            cerr << "Unable to load sample!" << endl;
            return -1;
        }
        
        if(this->verbose_)
        {
            cout << "Processing file: " << this->parser->positives[i]->filename.c_str() << endl;
        }

        if(this->percent_)
        {
            min = Size_<double>(inputImg.size().width * this->min_.width, inputImg.size().height * this->min_.height);
            max = Size_<double>(inputImg.size().width * this->max_.width, inputImg.size().height * this->max_.height);
        }
        else
        {
            min = this->min_;
            max = this->max_;
        }

        // CASCADE_DO_CANNY_PRUNING: Function uses Canny edge detector to reject some image regions that contain too few or too much edges and thus can not contain the searched object
        
        try
        {
            classify.detectMultiScale(inputImg, this->detects, this->scale_, this->neighbours_, CASCADE_SCALE_IMAGE|CASCADE_DO_CANNY_PRUNING, min, max);
        }
        catch (out_of_range)
        {
            cerr << "ERROR! Something ran out of bounds while searching for objects." << endl;
        }

        // Loop over every detected object in an image
        for(int j = 0; j < this->detects.size(); ++j)
        {
            // Loop over every defined object in a positive sample
            for(int k = 0; k < this->parser->positives[i]->hits.size(); ++k)
            {
                // Check local neighbourhood of defined positive for hits
                if(abs(this->parser->positives[i]->hits[k]->x - this->detects[j].x) < this->parser->positives[i]->hits[k]->width/2 and abs(this->parser->positives[i]->hits[k]->y - this->detects[j].y) < this->parser->positives[i]->hits[k]->width/2)
                {
                    if(this->verbose_)
                    {
                        cout << "Possible match" << endl;
                        cout << "dx: " << abs(this->parser->positives[i]->hits[k]->x - this->detects[j].x) << " dy: " << abs(this->parser->positives[i]->hits[k]->y - this->detects[j].y) << endl;
                    }

                    if(this->checkOverlap(*this->parser->positives[i]->hits[k], this->detects[j]) and !(this->parser->positives[i]->detected[k]))
                    {
                        // Set detected flag so the sample won't get detected twice
                        this->parser->positives[i]->detected[k] = true;
                        
                        this->parser->positives[i]->no_hits++;
                        this->parser->positives[i]->no_misses--;
                        if(this->parser->positives[i]->no_misses < 0)
                        {
                            this->parser->positives[i]->no_misses = 0;
                        }

                        if(this->verbose_)
                        {
                            cout << "True hit." << endl;
                        }
                        
                        if(this->show_)
                        {
                            // Black bounding box for detected objects
                            rectangle(inputImg, Point(this->detects[j].x, this->detects[j].y), Point(this->detects[j].x+this->detects[j].width, this->detects[j].y+this->detects[j].height), Scalar(0,0,0), 5);
                            // White bounding box for defined objects
                            rectangle(inputImg, Point(this->parser->positives[i]->hits[k]->x, this->parser->positives[i]->hits[k]->y), Point(this->parser->positives[i]->hits[k]->x+this->parser->positives[i]->hits[k]->width, this->parser->positives[i]->hits[k]->y+this->parser->positives[i]->hits[k]->height), Scalar(255,255,255), 5);
                        }
                    }
                    else if(this->checkOverlap(*this->parser->positives[i]->hits[k], this->detects[j]) and (this->parser->positives[i]->detected[k]))
                    {
                        if(this->verbose_)
                        {
                            cout << "Multiple detect, skipping." << endl;
                        }
                    }
                    else
                    {
                        if(this->verbose_)
                        {
                            cout << "False positive." << endl;
                        }
                        this->parser->positives[i]->no_false_positives++;
                    }
                }
            }
        }

        // Display image showing defined and detected objects
        if(this->show_)
        {
            resize(inputImg, inputImg, Size(1280,800), 1, 1);
            namedWindow("Output", WINDOW_AUTOSIZE);
            imshow("Output", inputImg);

            waitKey(0);

            inputImg.release();
        }
        else
        {
            inputImg.release();
        }
    }

    if(!(this->show_))
    {
        time(&end_time);
        this->time_ = difftime(end_time, start_time);
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
    // cout formatting
    fixed;
    cout.precision(4);

    long total_hits = 0;
    long total_misses = 0;
    long total_false_positives = 0;

    long total_positives = 0;

    cout << "Evaluation done!" << endl;
    cout << endl;
    cout << "Hits:\t| Misses:\t| False positives:\t| File:" << endl;
    cout << "-------------------------------------------------------" << endl;
    for(int i = 0; i < this->parser->positives.size(); ++i)
    {
        total_hits += this->parser->positives[i]->no_hits;
        total_misses += this->parser->positives[i]->no_misses;
        total_false_positives += this->parser->positives[i]->no_false_positives;

        total_positives += this->parser->positives[i]->hits.size();

        cout << this->parser->positives[i]->no_hits << "\t| " << this->parser->positives[i]->no_misses << "\t\t| " << this->parser->positives[i]->no_false_positives << "\t\t\t| " << this->parser->positives[i]->filename << endl;
    }
    cout << "-------------------------------------------------------" << endl;
    cout << endl << endl;
    cout << "Evaluated " << this->parser->positives.size() << " samples." << endl;
    if(!(this->show_))
    {
        cout << "Evaluation took " << ((int)this->time_/3600) << " hours, " << ((int)this->time_%3600)/60 << " minutes and " << ((int)this->time_%3600)%60 << " seconds." << endl;
    }
    cout << "-------------------------------------------------------" << endl;
    cout << "Total hits:\t| Total misses:\t| Total false positives:" << endl;
    cout << total_hits << "\t\t| " << total_misses << "\t\t| " << total_false_positives << endl;
    cout << "-------------------------------------------------------" << endl;
    if(total_positives)
    {
        cout << "Total hit ratio: " << ((double)total_hits/(double)total_positives)*100 << "%" << endl;
        cout << "-------------------------------------------------------" << endl;
    }
}
