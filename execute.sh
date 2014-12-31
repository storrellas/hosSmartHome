#!/bin/bash

# Get path of program
#####################
FULL_PATH=$(readlink -f $0)
RELATIVE_PATH=`dirname $FULL_PATH`

echo Executing in $RELATIVE_PATH


# Execute command
#################

sudo $RELATIVE_PATH/hosSmartHome -qws &> output.txt
echo "++ Contents of 'output.txt' ++"
cat output.txt

