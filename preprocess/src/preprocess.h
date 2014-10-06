#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "../../parser/src/fileParser.h"

using namespace std;
using namespace cv;

/**
 * @brief Preprocessor ... short description ...
 * @author Simon Hofmann <mail@simon-hofmann.org>
 * @date 2014-09-04
 * ... description ...
 */

class Preprocessor
{
  public:

    /**
     * Default constructor
     */
    //TODO: Preprocessor need marked positives, a background image of arbitrary size and some parameters.
    Preprocessor(const char *positives, const char *background);

    /**
     * Create some rotated and distorted images for each positive sample.
     * Returns total number of created images.
     */
    int process();
    
    /**
     * Creates a tiles background image for further processing
     */
    Mat tile(Mat &tile, Size output_size);
    Mat bckg;

    /**
     * Destructor
     */
    virtual ~Preprocessor();

  private:
    /**
     * Parser object
     */
    fileParser *parser;

    /**
     * Background Mat
     */

};

#endif /* #ifndef PREPROCESS_H */
