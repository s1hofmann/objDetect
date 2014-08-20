#!/bin/bash

# Create random samples used as negative images for opencv_traincascade
# Args:

function create_negatives () {
    for entry in $1/*
    do
        #Check if entry is a file
        if [[ -f $entry ]]
        then
            #Check if file is an image
            if [[ `file -ib $entry | grep 'image'` ]]
            then
                count=0
                # Statically generate five samples.
                # TODO: Dynamic version is not working due to variable scope in loops. Fix this...
                while [ $count -lt 5 ]
                do
                    # CONFIGURE IMAGE SIZE HERE
                    x=$((RANDOM%1000+500))
                    y=$((RANDOM%1000+500))
                    
                    # CONFIGURE IMAGE OFFSET HERE
                    off_x=$((RANDOM%500))
                    off_y=$((RANDOM%500))

                    echo "Processing $(basename $entry) ..."

                    output="neg_${count}_$(basename $entry)"
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

# Parameter check, argument 1 and 2 should be directories.
if [[ -d $1 ]] && [[ -d $2 ]]
then
    create_negatives $1 $2
elif [[ $1 == "-list" ]] && [[ -d $2 ]]
then
    #Create file list for use with opencv_traincascade
    find $2 -type f -not -iname "*.txt" >> $2/neg.txt
else
    echo "Usage: create_negatives arg1 arg2"
    echo ""
    echo "arg1: Directory containing the input images or '-list'."
    echo "arg2: Directory to store output images."
fi
