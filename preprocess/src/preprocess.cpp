#include "preprocess.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

// 2014-09-04 Simon Hofmann <mail@simon-hofmann.org>

using namespace std;
using namespace cv;

Preprocessor::Preprocessor(const char *positives, const char *background)
{
    this->parser = new fileParser(positives);
    this->parser->parsePositives();
}

int Preprocessor::process()
{
    int count = 0;

    for(auto entry : this->parser->positives)
    {
        Mat bigImage = imread(entry->filename);
        for(auto img : entry->hits)
        {
            for(int i = 0; i < 4; ++i)
            {
                Mat smallImg = Mat(bigImage, Rect(img->x, img->y, img->width, img->height));
                Mat rotation = getRotationMatrix2D(Point2f(smallImg.cols/2, smallImg.rows/2), i*90 , 1);
                Mat tmp = Mat::zeros(smallImg.rows, smallImg.cols, CV_64F);
                Mat output;
                warpAffine(smallImg, tmp, rotation, tmp.size());

                if(smallImg.cols > smallImg.rows)
                {
                    output = Mat(tmp, Rect((tmp.cols/2)-(tmp.rows/2), 0, tmp.rows, tmp.rows));
                }
                else if(smallImg.cols < smallImg.rows)
                {
                    output = Mat(tmp, Rect(0, (tmp.rows/2)-(tmp.cols/2), tmp.cols, tmp.cols));
                }
                else
                {
                    output = tmp;
                }

                namedWindow("output");
                imshow("output", output);
                waitKey(0);
                count++;
            }
        }
    }

    return count;
}

//TODO: Create an image twice the size of the input image which is used as background for further processing.
Mat Preprocessor::tile(Mat tile, Size output_size)
{
}

Preprocessor::~Preprocessor()
{
  // cleanup
  delete this->parser;
}
