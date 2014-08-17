# opencv_evaluate

opencv_evaluate is a tool to evaluate OpenCV's cascade classifier.

It runs a trained classifier against a set of positive marked images and displays an overview of positive hits, misses and false positives.

## Usage:

opencv_evaluate

-cascade <cascade classifier *.xml file>

-positives <positive samples file>

[-scale <scaling factor>: Parameter specifying how much the image size is reduced at each image scale.]

[-neighbours <count>: Parameter specifying how many neighbors each candidate rectangle should have to retain it.

[-minw <minimal width>: Minimum object width, smaller objects will be discarded.]

[-minh <minimal height>: Minimum object height, smaller objects will be discarded.]

[-maxw <minimal width>: Maximum object width, larger objects will be discarded.]

[-maxh <minimal width>: Maximum object height, larger objects will be discarded.]

[-v: Verbose mode, detailed output.]

[-s: Show mode, displays sample images showing bounding boxes of detected and defined objects.]

-h: Help, displays the text you are reading right now.
