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

struct hit
{
    int x;
    int y;
    int width;
    int height;
};

struct positive
{
    std::string filename;
    int count;
    std::vector <hit*> hits;
};

class Evaluator
{
  public:

    /**
     * Default constructor
     */
    Evaluator(const char *cascade, const char *posFile);

    /**
     * Destructor
     */
    virtual ~Evaluator();

    /**
     * Copy constructor
     * @param other Reference on object to copy.
     */
    Evaluator(const Evaluator& other);

    /** 
     * Assignment operator
     * @param other Reference on object to copy.
     * @return Reference on initialisated object.
     */
    Evaluator& operator=(const Evaluator& other);

  private:
    std::string inFile;
    std::vector <positive*> positives;
};

#endif /* #ifndef EVALUATOR_H */
