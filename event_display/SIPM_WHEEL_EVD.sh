#!/usr/bin/env python

EVENT_DISPLAY_DIR=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
WHEEL_DIR=$( dirname $EVENT_DISPLAY_DIR )

source $WHEEL_DIR/setup.sh
source $EVENT_DISPLAY_DIR/pyqt/config/setup.sh

echo
echo "Running event display..."
echo 

sipm_wheel_evd.py
# root -l ./root/EventDisplay.C
