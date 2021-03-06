#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "../../parser/src/fileParser.h"

#include <string>

using namespace std;
using namespace cv;

/**
 * @brief PreProcessor
 * @author Simon Hofmann <mail@simon-hofmann.org>
 * @date 2014-09-04
 * The PreProcessor class provides methods to generate multiple transformed images from one defined bounding box in order to increase the total amount of positive samples
 */

class PreProcessor
{
  public:

    /**
     * Default constructor
     *
     * Parameters:
     *
     * output: output directory
     * positives: path to a file which contains marked positives
     * background: an arbitrary background image which is used for further processing
     * num: Number of tranformed images to generate for each defined bounding box
     * min_rotation: Minimal rotation value
     * max_rotation: Maximal rotation value
     * show: Shows transformed images
     */
    PreProcessor(string output_dir, string positives_file, string background_img, int num_of_samples = 1, int min_rotation = 1, int max_rotation = 360, bool show = false);

    /**
     * Create some rotated and distorted images for each positive sample.
     * Returns total number of created images.
     */
    int process();
    
    /**
     * Destructor
     */
    virtual ~PreProcessor();

  private:
    /**
     * Creates a tiles background image for further processing
     */
    Mat tile(const Mat &tile, Size output_size);

    /**
     * Performs image transformation
     */
    Mat transform(const Mat &input);

    /**
     * Parser object
     */
    fileParser *parser_;

    /**
     * Output directory
     */
    string out_dir_;

    /**
     * Background Mat
     */
    Mat bckg_;

    /**
     * Output Mat
     */
    Mat output_;

    /**
     * Number of transformed images to generate
     */
    int amount_;

    /**
     * Minimum amount of rotation
     */
    int min_rot_;

    /**
     * Maximum amount of rotation
     */
    int max_rot_;
    
    /**
     * Verbose mode, show generated images
     */
    bool show_;

};
