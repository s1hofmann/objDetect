#!/bin/bash

#Create random samples used as negative images for opencv_traincascade
# Args:
# $1: Image directory
# $2: Output directory
# $3: No. of samples to create per image

function create_negatives () {
    for entry in $1/*
    do
        #Check if entry is a file
        if [ -f $entry ]
        then
            #Check if file is an image
            if [[ `file -ib $entry | grep 'image'` ]]
            then
                count=0
                while [ $count -lt $3 ]
                do
                    x=$((RANDOM%1000+1000))
                    y=$((RANDOM%1000+1000))

                    off_x=$((RANDOM%500))
                    off_y=$((RANDOM%500))

                    echo "Processing $(basename $entry) ..."

                    output="neg_${count}_$(basename $entry)"
                    #Cut out a random part of the image using the coordinates from above
                    convert $entry -crop $x\x$y+$off_x+$off_y $2/$output
                    let count=count+1
                done
            fi
        fi
        if [[ -d $entry && $entry != $2 ]]
        then
            create_negatives $entry $2
        fi
    done
}

mkdir -p $2

create_negatives $1 $2 $3

#Create file list for use with opencv_traincascade
find $2 -type f -not -iname "*.txt" >> $2/neg.txt
