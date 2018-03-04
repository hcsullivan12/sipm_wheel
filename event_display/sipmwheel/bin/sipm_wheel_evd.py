#!/usr/bin/env python

from ROOT import TCanvas, TF1, wheelevd
import sys
import argparse
import time

from gui import sipm_wheel_gui as GUI
#from manager import evd_manager as evd_manager

def main():

	# Read in data files with argparse
	
	# Instance of GUI object
	#thisgui = GUI.sipm_wheel_gui()
	#thisgui.create()
	c1 = TCanvas("c1", "c1", 800, 800)
	f = TF1("f", "sin(x)/x", 0, 10)
	f.Draw()
	#c1.Update()
	time.sleep(5)

	obj = wheelevd.Test()
	obj.Print()


if __name__ == '__main__':
	main()
