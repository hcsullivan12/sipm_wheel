#!/usr/bin/env python

from ROOT import TCanvas, TF1, wheelevd
import sys
import argparse
import signal

from PyQt4 import QtGui, QtCore
from gui import sipm_wheel_gui as GUI
from manager import evd_manager as evd_manager

def main():

	# Read in data files with argparse
	
	# Create Qt application 
	app = QtGui.QApplication(sys.argv)

	# Create a manager
	manager = evd_manager.evd_manager()

	# Instance of GUI object
	thisgui = GUI.sipm_wheel_gui(manager)
	thisgui.initUI()
	
	# Connect eventChanged to updating GUI
	manager.eventChanged.connect(thisgui.update)
	thisgui.update()

	obj = wheelevd.Test()
	obj.Print()

	sys.exit(app.exec_())


if __name__ == '__main__':
	main()
