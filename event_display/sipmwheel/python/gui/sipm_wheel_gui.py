#!/usr/bin/env python

import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import * 
from ROOT import TCanvas

class sipm_wheel_gui():

	def __init__(self):
		print ""		

	def getLeftLayout(self):
		leftLayout   = QVBoxLayout()

		# Create widgets for left
                b_next     = QPushButton("Next")
                b_previous = QPushButton("Previous")
                b_quit     = QPushButton("Quit")

                leftLayout.addWidget(b_next)
                leftLayout.addWidget(b_previous)
                leftLayout.addStretch()
                leftLayout.addWidget(b_quit)

		return leftLayout

	def getMiddleLayout(self):
		middleLayout   = QVBoxLayout()
		
		# Create widgets for middle
                b_space = QPushButton("space")
		middleLayout.addWidget(b_space)
		
		return middleLayout

	def getRightLayout(self):
		rightLayout   = QVBoxLayout()
	
		# Create widgets for right
                title = QLabel("SiPM Wheel\nEvent Display")
                title.setAlignment(Qt.AlignLeft)
                rightLayout.addWidget(title)

                f_entry  = QFormLayout()
                rInc     = QLabel("Radius Increment: ")
                thetaInc = QLabel("Theta Increment: ")
                nInc     = QLabel("N Increment: ")
                
                f_entry.addRow(rInc, QLineEdit())
                f_entry.addRow(thetaInc, QLineEdit())
                f_entry.addRow(nInc, QLineEdit())               

                rightLayout.addLayout(f_entry)

                b_setparam = QPushButton("Set Parameters")
                b_start    = QPushButton("Start")

                rightLayout.addWidget(b_setparam)
                rightLayout.addWidget(b_start)

		return rightLayout

	def create(self):
		app    = QApplication(sys.argv)
		window = QWidget()
		
		# Create a horizontal frame to place vertical frames
                hbox = QHBoxLayout()

		self.leftLayout = self.getLeftLayout()
		self.middleLayout = self.getMiddleLayout()
		self.rightLayout = self.getRightLayout()

		hbox.addLayout(self.leftLayout)
		hbox.setAlignment(self.leftLayout, Qt.AlignLeft)
		hbox.addLayout(self.middleLayout)
		hbox.addLayout(self.rightLayout)
		hbox.setAlignment(self.rightLayout, Qt.AlignRight)

		# Add to window
		window.setLayout(hbox)

		window.setWindowTitle("Event Display")
		window.setGeometry(10,10,1500,1000)
	        window.show()
        	sys.exit(app.exec_())		
