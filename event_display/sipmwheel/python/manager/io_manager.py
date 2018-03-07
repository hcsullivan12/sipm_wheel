from pyqtgraph.Qt import QtCore
import threading

class io_manager(QtCore.QObject):
	
	eventChanged = QtCore.pyqtSignal()

	def __init__(self):
		super(io_manager, self).__init__()

	def startCycle(self, delay):
		self._cycling = True
		self._stopCycleFlag = threading.Event()
		self._cycleWatcher = delayTimer(self._stopCycleFlag, delay)
		self._cycleWatcher.delayExpired.connect(self.next)
		self._cycleWatcher.start()

	def stopCycle(self):
		self._cycling = False

	def next(self):
		self.goToEvent(self._event + 1)

	def goToEvent(self, event):
		self._event = event
		self.eventChanged.emit()
