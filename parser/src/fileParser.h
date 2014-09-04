#ifndef FILEPARSER_H 
#define FILEPARSER_H 

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <vector>
#include <string>

/**
 * @brief fileParser, a class to parse a file of positive samples for an OpenCV cascade classifier
 * @author Simon Hofmann <mail@simon-hofmann.org>
 * @date 2014-08-12
 */

struct positive
{
    std::string filename;
    int count = 0;
    std::vector <cv::Rect*> hits;
    std::vector <bool> detected;
    long no_hits = 0;
    long no_misses = 0;
    long no_false_positives = 0;
};

class fileParser
{
  public:

    /**
     * Default constructor
     */
    fileParser(const char *posFile);

    /**
     * Destructor
     */
    virtual ~fileParser();
    
    /**
     * Stores positive image data
     */
    std::vector <positive*> positives;
    
    /**
     * Parse files containing positive image samples as described in the CascadeClassifier documentation
     * Returns number of successfully parsed samples
     */
    int parsePositives();

  private:
    /*
     * Text file containing positive sample data
     */
    std::string posFile;

    /**
     * Free dynamically allocated memory
     */
    void freeMem();
    
};

#endif /* #ifndef FILEPARSER_H*/
