## This class is a standard event display object ##

from PyQt4 import QtCore, QtGui

class gui(QtGui.QWidget):

	def __init__(self):
		super(gui, self).__init__()

	def getEventControlButtons(self):
		self._goToLabel = QtGui.QLabel("Go to: ")
		self._nextButton = QtGui.QPushButton("Next")
		self._previousButton = QtGui.QPushButton("Previous")
		self._entryBox = QtGui.QLineEdit()
		self._entryBox.setToolTip("Enter an event to skip to")
		self._runLabel = QtGui.QLabel("Run: 0")
		self._eventLabel = QtGui.QLabel("Event: 0")
		self._subrunLabel = QtGui.QLabel("Subrun: 0")

		# Put into layout
		self._eventControlBox = QtGui.QVBoxLayout()
		
		self._grid = QtGui.QHBoxLayout()
		self._grid.addWidget(self._goToLabel)
		self._grid.addWidget(self._entryBox)
		
		self._eventControlBox.addLayout(self._grid)
		self._eventControlBox.addWidget(self._nextButton)
		self._eventControlBox.addWidget(self._previousButton)
		self._eventControlBox.addWidget(self._eventLabel)
		self._eventControlBox.addWidget(self._runLabel)
		self._eventControlBox.addWidget(self._subrunLabel)

		return self._eventControlBox

	def getQuitButton(self):
		self._quitButton = QtGui.QPushButton("Quit")
		self._quitButton.setToolTip("Close Event Display")
		self._quitButton.clicked.connect(self.quit)
		return self._quitButton

	def quit(self):
		QtCore.QCoreApplication.instance().quit()

	def getLeftLayout(self):
		event_control = self.getEventControlButtons()
		quit_control = self.getQuitButton()

		# New layout
		self._leftLayout = QtGui.QVBoxLayout()
		self._leftLayout.addLayout(event_control)
		self._leftLayout.addStretch(1)
		self._leftLayout.addWidget(quit_control)
		self._leftLayout.setAlignment(QtCore.Qt.AlignLeft)

		self._leftWidget = QtGui.QWidget()
		self._leftWidget.setLayout(self._leftLayout)
		self._leftWidget.setMaximumWidth(200)
		self._leftWidget.setMinimumWidth(100)

		return self._leftWidget

	def getRightLayout(self):
		# Create a standard layout
		label = QtGui.label("Standard")
		self._rightWidget = QtGui.QWidget()
		self._rightLayout = QVBoxLayout()
		self._rightLayout.addWidget(label)
		self._rightLayout.addStretch(1)
		self._rightWidget.setLayout(self._rightLayout)
		self._rightWidget.setMaximumWidth(200)
		self._rightWidget.setMinimumWidth(100)
		return self._rightWidget

	def getCenterLayout(self):
		button = QtGui.QPushButton("Dummy")
		wid = QtGui.QWidget()
		lay = QtGui.QVBoxLayout()
		lay.addWidget(button)
		wid.setLayout(lay)
		wid.setMaximumWidth(1500)
		wid.setMinimumWidth(200)
		return wid

	def initUI(self):
		# Get all of the widgets
		self.rightWidget = self.getRightLayout()
		self.leftWidget = self.getLeftLayout()
		
		# Get the center view
		self.centerWidget = self.getCenterLayout()
		# Put together 
		self.master = QtGui.QVBoxLayout()
		self.slave = QtGui.QHBoxLayout()
		
		self.slave.addWidget(self.leftWidget)
		self.slave.addWidget(self.centerWidget)
		self.slave.addWidget(self.rightWidget)

		self.master.addLayout(self.slave)
		

		self.setLayout(self.master)
		self.setGeometry(10, 10, 1500, 1000)
		self.setWindowTitle('Event Display')
		self.setFocus()
		self.show()


