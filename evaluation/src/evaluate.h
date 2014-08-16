#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <vector>
#include <string>

/**
 * @brief Evaluator, a class to evaluate the precision of an OpenCV cascade classifier
 * @author Simon Hofmann <mail@simon-hofmann.org>
 * @date 2014-08-12
 * This class provides methods to evaluate cascade classifiers.
 * It runs a trained classifier and creates an overview on hits, misses and false positives.
 */

struct positive
{
    std::string filename;
    int count;
    std::vector <cv::Rect*> hits;
    int no_hits;
    int no_misses;
    int no_false_positives;
};

class Evaluator
{
  public:

    /**
     * Default constructor
     */
    Evaluator(const char *cascade, const char *posFile, bool verbose=false, bool show = false);

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
     *Stores bounding boxes of detected objects
     */
    std::vector <cv::Rect> detects;

    /*
     *Path to cascade classifiers *.xml file
     */
    std::string cascadeFile;

    /*
     *Text file containing positive sample data
     */
    std::string posFile;

    /**
     *Stores positive image data
     */
    std::vector <positive*> positives;

    /**
     *Free dynamically allocated memory
     */
    void freeMem();
    
    /**
     *Parse files containing positive image samples as described in the CascadeClassifier documentation
     */
    int parsePositives(const char *posFile);
    
    /**
     *Checks the percentage of overlapping of two rectangles
     */
    double checkOverlap(cv::Rect positive, cv::Rect detect);
};

#endif /* #ifndef EVALUATOR_H */
