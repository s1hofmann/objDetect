#include "preprocess.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <ctime>
#include <cmath>

// 2014-09-04 Simon Hofmann <mail@simon-hofmann.org>

using namespace std;
using namespace cv;

Preprocessor::Preprocessor(string output, string positives, string background, int num, int min_rotation, int max_rotation, bool show) : out_dir(output), amount(num), min_rot(min_rotation), max_rot(max_rotation), show(show)
{
    srand(time(NULL));
    this->parser = new fileParser(positives);
    this->parser->parsePositives();
    this->bckg = imread(background.c_str());
}

int Preprocessor::process()
{
    int count = 0;
    try
    {
        string list;
        list = this->out_dir + "/positive.dat";

        ofstream filelist;
        filelist.exceptions(ofstream::failbit | ofstream::badbit);

        //Empty a possibliy existing filelist
        filelist.open(list.c_str(), ofstream::out | ofstream::trunc);

        for(auto entry : this->parser->positives)
        {
            Mat bigImage = imread(entry->filename);
            for(auto img : entry->hits)
            {
                Mat bounding_box= Mat(bigImage, Rect(img->x, img->y, img->width, img->height));
                
                for(int i = 0; i < this->amount; ++i)
                {
                    this->output = transform(bounding_box);

                    stringstream filename;
                    
                    filename << this->out_dir << "/pos_" << count << ".png";
                    imwrite(filename.str(), output);
                    filelist << filename.str() << " " << 1 << " " << 0 << " " << 0 << " " << this->output.cols << " " << this->output.rows << endl;

                    count++;

                    filename.clear();

                    if(this->show)
                    {
                        namedWindow("output");
                        imshow("output", output);
                        waitKey(0);
                    }
                }
            }
        }

        filelist.close();

    }
    catch (ofstream::failure e)
    {
        cerr << "File I/O error!" << endl;
        return -1;
    }

    return count;
}

Mat Preprocessor::tile(const Mat &tile, Size output_size)
{
    Mat output = Mat::zeros(output_size, tile.type());

    int width = output_size.width / tile.cols;
    int height = output_size.height / tile.rows;

    int remaining_width = output_size.width % tile.cols;
    int remaining_height = output_size.height % tile.rows;

    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            //Amount of tiles which fit the image
            tile.copyTo(output(Rect(i*tile.cols, j*tile.rows, tile.cols, tile.rows)));
            
            if(remaining_height and remaining_width)
            {
                //Fills right border
                tile(Rect(0, 0, remaining_width, tile.rows)).copyTo(output(Rect(width*tile.cols, j*tile.rows, remaining_width, tile.rows)));
                //Fills bottom border
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

Mat Preprocessor::transform(const Mat &input)
{
    int max_len = sqrt(input.cols*input.cols+input.rows*input.rows);
    
    Mat out = tile(this->bckg, Size(max_len*2, max_len*2));

    int angle = 0;

    if(this->max_rot != 0 and this->min_rot != 0 and this->max_rot != this->min_rot)
    {
        angle = this->min_rot + rand()%(this->max_rot-this->min_rot);
    }

    input.copyTo(out(Rect(out.cols/2-(input.cols/2), out.rows/2-(input.rows/2), input.cols, input.rows)));
    
    Mat rotation = getRotationMatrix2D(Point2f(out.cols/2, out.rows/2), angle , 1);
    warpAffine(out, out, rotation, out.size());

    out = Mat(out, Rect((out.cols/2)-max_len/2, (out.rows/2)-max_len/2, max_len, max_len));

    return out;
}

Preprocessor::~Preprocessor()
{
  // cleanup
  delete this->parser;
  this->output.release();
}
