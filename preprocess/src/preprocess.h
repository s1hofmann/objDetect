#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "../../parser/src/fileParser.h"

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
    Preprocessor();

    /**
     * Destructor
     */
    virtual ~Preprocessor();

  private:
    /**
     * Parser object
     */
    fileParser *parser;

};

#endif /* #ifndef PREPROCESS_H */
