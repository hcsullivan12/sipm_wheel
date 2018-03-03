#!bin/bash

EVENT_DIS_CONFIG="$( cd $( dirname $BASH_SOURCE[0]) && pwd)"
EVENT_DIS_BASEDIR="$( dirname $EVENT_DIS_CONFIG)"

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



# Add bin to PATH
if [[ ! ":$PATH:" == *":$EVENT_DIS_BASEDIR/bin:"* ]]; then
  export PATH=$EVENT_DIS_BASEDIR/bin:$PATH
fi

if [[ ! ":$PYTHONPATH:" == *":$EVENT_DIS_BASEDIR/bin:"* ]]; then
  export PYTHONPATH=$EVENT_DIS_BASEDIR/bin:$PYTHONPATH
fi
# Set permissions
chmod +x $EVENT_DIS_BASEDIR/bin/sipm_wheel_evd.py

# Add python to PATH
if [[ ! ":$PATH:" == *":$EVENT_DIS_BASEDIR/python:"* ]]; then
  export PATH=$EVENT_DIS_BASEDIR/python:$PATH
fi

if [[ ! ":$PYTHONPATH:" == *":$EVENT_DIS_BASEDIR/python:"* ]]; then
  export PYTHONPATH=$EVENT_DIS_BASEDIR/python:$PYTHONPATH
fi
