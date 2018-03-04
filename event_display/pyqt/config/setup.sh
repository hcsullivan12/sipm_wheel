#!bin/bash

EVENT_DIS_CONFIGDIR="$( cd $( dirname $BASH_SOURCE[0]) && pwd)"
EVENT_DIS_BASEDIR="$( dirname $EVENT_DIS_CONFIGDIR)"

#echo $EVENT_DIS_CONFIG
#echo $EVENT_DIS_BASEDIR

# Print out which root
#echo
#echo "Using root $(which root)"

# Print out which python
#echo "Using python $(which python)"

# Get default PATH and PYTHONPATH
PYTHONPATH_backup=$PYTHONPATH
PATH_backup=$PATH

# Check for PyQt4
if ! $(python -c "import PyQt4" &> /dev/null); then
	echo "PyQt4 failed to configure."
   	PYTHONPATH=$PYTHONPATH_backup
  	PATH=$PATH_backup
  	return
fi

# Check for ROOT 
if [ `command -v rootcling` ]; then
    	export ROOT=1
else
    if [[ -z `command -v rootcint` ]]; then
        echo
        echo Looks like you do not have ROOT installed.
        echo You cannot use SiPM Wheel EVD w/o ROOT!
        echo Aborting.
        echo
        return;
    fi
fi
if [[ -z $ROOTSYS ]]; then
	echo 
	echo Could not find \$ROOTSYS. Make sure PyROOT is 
	echo configured. You need to export \$PYTHONPATH to
	echo include the dir where ROOT.py exists.
	echo
else
	if [[ ! ":$PYTHONPATH:" == *":$ROOTSYS/lib:"* ]]; then 
		export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH;
	fi
fi

# Set sub directories
export EVENT_DIS_LIBDIR=$EVENT_DIS_BASEDIR/lib
export EVENT_DIS_COREDIR=$EVENT_DIS_BASEDIR/core

# Add bin to PATH
if [[ ! ":$PATH:" == *":$EVENT_DIS_BASEDIR/bin:"* ]]; then
	export PATH=$EVENT_DIS_BASEDIR/bin:$PATH
fi

if [[ ! ":$PYTHONPATH:" == *":$EVENT_DIS_BASEDIR/bin:"* ]]; then
  	export PYTHONPATH=$EVENT_DIS_BASEDIR/bin:$PYTHONPATH
fi
# Set permissions
chmod +x $EVENT_DIS_BASEDIR/bin/sipm_wheel_evd.py

# Add lib to library
if [[ ! ":$LD_LIBRARY_PATH:" == *":$EVENT_DIS_LIBDIR:"* ]]; then
	export LD_LIBRARY_PATH=$EVENT_DIS_LIBDIR:$LD_LIBRARY_PATH
fi

# Add python to PATH
if [[ ! ":$PATH:" == *":$EVENT_DIS_BASEDIR/python:"* ]]; then
  	export PATH=$EVENT_DIS_BASEDIR/python:$PATH
fi

if [[ ! ":$PYTHONPATH:" == *":$EVENT_DIS_BASEDIR/python:"* ]]; then
  	export PYTHONPATH=$EVENT_DIS_BASEDIR/python:$PYTHONPATH
fi
