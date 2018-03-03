#!/usr/bin/env python

import sys
import argparse

from gui import sipm_wheel_gui as GUI

def main():

	# Read in data files with argparse
	
	# Instance of GUI object
	thisgui = GUI.sipm_wheel_gui()
	thisgui.create()




if __name__ == '__main__':
	main()
