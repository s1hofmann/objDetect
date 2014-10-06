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
    this->bckg = imread(background);
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

Mat Preprocessor::tile(Mat &tile, Size output_size)
{
    Mat output = Mat::zeros(output_size, tile.type());

    int width = output_size.width / tile.cols;
    int height = output_size.height / tile.rows;

    int remaining_width = output_size.width % tile.cols;
    int remaining_height = output_size.height % tile.rows;

    cout << width << " " << height << endl;
    cout << remaining_width << " " << remaining_height << endl;
    
    //Amount of tiles which fit the image
    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            tile.copyTo(output(Rect(i*tile.cols, j*tile.rows, tile.cols, tile.rows)));
            
            if(remaining_height and remaining_width)
            {
                //Fills right space
                tile(Rect(0, 0, remaining_width, tile.rows)).copyTo(output(Rect(width*tile.cols, j*tile.rows, remaining_width, tile.rows)));
                //Fills bottom space
                tile(Rect(0, 0, tile.cols, remaining_height)).copyTo(output(Rect(i*tile.cols, height*tile.rows, tile.cols, remaining_height)));
                //Fills the remaining corner part
                tile(Rect(0, 0, remaining_width, remaining_height)).copyTo(output(Rect(width*tile.cols, height*tile.rows, remaining_width, remaining_height)));
            }
            else if(remaining_height)
            {
                tile(Rect(0, 0, tile.cols, remaining_height)).copyTo(output(Rect(i*tile.cols, height*tile.rows, tile.cols, remaining_height)));
            }
            else if(remaining_width)
            {
                tile(Rect(0, 0, remaining_width, tile.rows)).copyTo(output(Rect(width*tile.cols, j*tile.rows, remaining_width, tile.rows)));
            }
        }
    }

    return output;
}

Preprocessor::~Preprocessor()
{
  // cleanup
  delete this->parser;
}
