#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../../parser/src/fileParser.h"

#include <vector>
#include <string>
#include <ctime>

/**
 * @brief Evaluator, a class to evaluate the precision of an OpenCV cascade classifier
 * @author Simon Hofmann <mail@simon-hofmann.org>
 * @date 2014-08-12
 * This class provides methods to evaluate cascade classifiers.
 * It runs a trained classifier and creates an overview on hits, misses and false positives.
 */

class Evaluator
{
  public:

    /**
     * Default constructor
     */
    Evaluator(std::string cascade, std::string positive, bool verbose=false, bool show = false, double scale = 1.1, double neighbours = 3, cv::Size_<double> min = cv::Size_<double>(0,0), cv::Size_<double> max = cv::Size_<double>(0,0), bool percent = false);

    /**
     * Destructor
     */
    virtual ~Evaluator();
    
    /**
     * Run a given classifier on a set of positive sample images.
     * Generates an overview on hits, misses and false positives.
     */
    int evaluate();

    /**
     * Shows results of evaluation.
     */
    void showResults();

  private:
    /**
     *Verbose mode - more detailed output
     */
    bool _verbose;

    /**
     *Show image with bounding boxes of defined and detected elements
     */
    bool _show;

    /**
     * Scale factor for cascade classifier
     */
    double _scale;
    
    /**
     * Min. no. of neighbours for positive hit
     */
    double _neighbours;

    /**
     * Min. size of detected objects
     */
    cv::Size_<double> _min;

    /**
     * Max size of detected objects
     */
    cv::Size_<double> _max;

    /**
     * Min and max size are entered in percent
     */
    bool _percent;

    /**
     * Used to measure calculation time
     */
    double _time = 0;

    /**
     * Stores bounding boxes of detected objects
     */
    std::vector <cv::Rect> detects;

    /*
     * Path to cascade classifiers *.xml file
     */
    std::string cascadeFile;

    /*
     * Text file containing positive sample data
     */
    std::string posFile;

    /**
     * Checks the percentage of overlapping of two rectangles
     */
    double checkOverlap(cv::Rect positive, cv::Rect detect);

    /**
     * Parser for positive samples
     */
    fileParser *parser;
};
