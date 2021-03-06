#!/usr/bin/env python

import sys
from PyQt4 import QtCore, QtGui 
from gui import gui

class sipm_wheel_gui(gui):

	def __init__(self, manager):
		super(sipm_wheel_gui, self).__init__()
		self.connect_manager(manager)

	def initUI(self):
		super(sipm_wheel_gui, self).initUI()		

	def update(self):
		
		# Change the labels
		eventLabel = "Event: " + str(self._evd_manager.event()) 
		self._eventLabel.setText(eventLabel)
		runLabel = "Run: " + str(self._evd_manager.run())
		self._runLabel.setText(runLabel)
		subrunLabel = "Subrun: " + str(self._evd_manager.subrun())
		self._subrunLabel.setText(subrunLabel)

		# Update the detector and plot view
		self._view_manager.updateVoxel()
		self._view_manager.updatePlot()


	def getRightLayout(self):
	
		# Create main title
                text1 = QtGui.QLabel("SiPM Wheel\nEvent Display")
		text1.setAlignment(QtCore.Qt.AlignCenter)
		text2 = QtGui.QLabel("Reco Parameters")
                text2.setAlignment(QtCore.Qt.AlignCenter)
		text4 = QtGui.QLabel("Disk Parameters")
		text4.setAlignment(QtCore.Qt.AlignCenter)
		text3 = QtGui.QLabel("Run Reco")
		text3.setAlignment(QtCore.Qt.AlignCenter)
		font1 = text1.font()
		font2 = text2.font()
		font1.setBold(True)
		font2.setBold(True)
		font1.setPointSize(18)
		font2.setPointSize(15)
		text1.setFont(font1)
		text2.setFont(font2)
		text3.setFont(font2)
		text4.setFont(font2)
		
		# Parameter layout
		self._params = dict()
		self._params['diskR'] = 15.0
		self._params['attenL'] = 2.0
		self._params['nPos'] = 8
		self._params['rInc'] = 0.1
		self._params['thetaInc'] = 1 
		self._params['nInc'] = 1	               

                disk_params_controls = QtGui.QVBoxLayout()
		reco_params_controls = QtGui.QVBoxLayout()

		font3 = QtGui.QFont("Times", 12)

		# Disk Radius 
                diskR_layout = QtGui.QHBoxLayout()
		diskR_text = QtGui.QLabel("Disk Radius: ")
		diskR_text.setFont(font3)
                diskR_layout.addWidget(diskR_text)
                self._diskR_Box = QtGui.QLineEdit(str(self._params['diskR']))
		self._diskR_Box.setFont(font3)
                self._diskR_Box.setToolTip("Radius of disk")
                diskR_layout.addWidget(self._diskR_Box)
		diskRUnits_text = QtGui.QLabel(" cm")
		diskRUnits_text.setFont(font3)
                diskR_layout.addWidget(diskRUnits_text)
                disk_params_controls.addLayout(diskR_layout)

		# Attenuation Length 
                attenL_layout = QtGui.QHBoxLayout()
                attenL_text = QtGui.QLabel("Attenuation Length: ")
                attenL_text.setFont(font3)
                attenL_layout.addWidget(attenL_text)
                self._attenL_Box = QtGui.QLineEdit(str(self._params['attenL']))
                self._attenL_Box.setFont(font3)
                self._attenL_Box.setToolTip("Attenuation length of the disk")
                attenL_layout.addWidget(self._attenL_Box)
                attenLUnits_text = QtGui.QLabel(" cm")
                attenLUnits_text.setFont(font3)
                attenL_layout.addWidget(attenLUnits_text)
                disk_params_controls.addLayout(attenL_layout)

		# Number of positions 
                nPos_layout = QtGui.QHBoxLayout()
                nPos_text = QtGui.QLabel("Number of Positions: ")
                nPos_text.setFont(font3)
                nPos_layout.addWidget(nPos_text)
                self._nPos_Box = QtGui.QLineEdit(str(self._params['nPos']))
                self._nPos_Box.setFont(font3)
                self._nPos_Box.setToolTip("Number of positions around the disk")
                nPos_layout.addWidget(self._nPos_Box)
                nPosUnits_text = QtGui.QLabel(" pos")
                nPosUnits_text.setFont(font3)
                nPos_layout.addWidget(nPosUnits_text)
                disk_params_controls.addLayout(nPos_layout)

		# Radius increment 
                rInc_layout = QtGui.QHBoxLayout()
                rInc_text = QtGui.QLabel("Radius Increment: ")
                rInc_text.setFont(font3)
                rInc_layout.addWidget(rInc_text)
                self._rInc_Box = QtGui.QLineEdit(str(self._params['rInc']))
                self._rInc_Box.setFont(font3)
                self._rInc_Box.setToolTip("Amount to increment in the radius")
                rInc_layout.addWidget(self._rInc_Box)
                rIncUnits_text = QtGui.QLabel(" cm")
                rIncUnits_text.setFont(font3)
                rInc_layout.addWidget(rIncUnits_text)
                reco_params_controls.addLayout(rInc_layout)

		# Theta increment 
                thetaInc_layout = QtGui.QHBoxLayout()
                thetaInc_text = QtGui.QLabel("Theta Increment: ")
                thetaInc_text.setFont(font3)
                thetaInc_layout.addWidget(thetaInc_text)
                self._thetaInc_Box = QtGui.QLineEdit(str(self._params['thetaInc']))
                self._thetaInc_Box.setFont(font3)
                self._thetaInc_Box.setToolTip("Amount to increment in the angle")
                thetaInc_layout.addWidget(self._thetaInc_Box)
                thetaIncUnits_text = QtGui.QLabel(" deg")
                thetaIncUnits_text.setFont(font3)
                thetaInc_layout.addWidget(thetaIncUnits_text)
                reco_params_controls.addLayout(thetaInc_layout)
		
		# N increment 
                nInc_layout = QtGui.QHBoxLayout()
                nInc_text = QtGui.QLabel("N Increment: ")
                nInc_text.setFont(font3)
                nInc_layout.addWidget(nInc_text)
                self._nInc_Box = QtGui.QLineEdit(str(self._params['nInc']))
                self._nInc_Box.setFont(font3)
                self._nInc_Box.setToolTip("Amount to increment in the total light yield")
                nInc_layout.addWidget(self._nInc_Box)
                nIncUnits_text = QtGui.QLabel(" p.e.")
                nIncUnits_text.setFont(font3)
                nInc_layout.addWidget(nIncUnits_text)
                reco_params_controls.addLayout(nInc_layout)

		b_setparam = QtGui.QPushButton("Set Parameters")

		# Create start/stop buttons
		self._eventUpdateButton = QtGui.QPushButton("Start")
		self._eventUpdateButton.clicked.connect(self.eventUpdateButtonHandler)

		# Create the layout
		self._rightWidget = QtGui.QWidget()
		self._rightLayout = QtGui.QVBoxLayout()
		self._rightLayout.addWidget(text1)
		self._rightLayout.addStretch(1)
		self._rightLayout.addWidget(text4)
		self._rightLayout.addLayout(disk_params_controls)
		self._rightLayout.addWidget(text2)
		self._rightLayout.addLayout(reco_params_controls)
		self._rightLayout.addWidget(b_setparam)
		self._rightLayout.addStretch(1)
		self._rightLayout.addWidget(text3)
		self._rightLayout.addWidget(self._eventUpdateButton)
		self._rightLayout.setAlignment(QtCore.Qt.AlignRight)
		self._rightWidget.setLayout(self._rightLayout)
		self._rightWidget.setMaximumWidth(300)
		self._rightWidget.setMinimumWidth(100)
		
		return self._rightWidget

	def eventUpdateButtonHandler(self):
		
		if self._evd_manager.io().isCycling():
			self._evd_manager.io().stopCycle()
			self._eventUpdateButton.setText("Start")
			
		else:
			delay = 2
			self._eventUpdateButton.setText("Pause")
			self._evd_manager.io().startCycle(delay)
		
