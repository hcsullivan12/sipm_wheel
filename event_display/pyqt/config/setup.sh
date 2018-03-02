#!bin/bash

EVENT_DIS_CONFIG="$( cd $( dirname $BASH_SOURCE[0]) && pwd)"
EVENT_DIS_BASEDIR="$( dirname $EVENT_DIS_CONFIG)"

#echo $EVENT_DIS_CONFIG
#echo $EVENT_DIS_BASEDIR

# Print out which root
echo "Using root $(which root)"

# Print out which python
echo "Using python $(which python)"

# Get default PATH and PYTHONPATH
PYTHONPATH_backup=$PYTHONPATH
PATH_backup=$PATH

# Add bin to PATH
export PATH=$EVENT_DIS_BASEDIR/bin:$PATH
export PYTHONPATH=$EVENT_DIS_BASEDIR/bin:$PYTHONPATH

# Add python to PATH
export PATH=$EVENT_DIS_BASEDIR/python:$PATH
export PYTHONPATH=$EVENT_DIS_BASEDIR/python:$PYTHONPATH
