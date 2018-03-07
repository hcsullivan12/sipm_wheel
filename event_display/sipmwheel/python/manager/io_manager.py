from pyqtgraph.Qt import QtCore
import threading

class io_manager(QtCore.QObject):
	
	eventChanged = QtCore.pyqtSignal()

	def __init__(self):
		super(io_manager, self).__init__()
		self._isCycling = False

		self._event = 0

	def startCycle(self, delay):
		print "Starting Cycle!"
		self._isCycling = True
		#self._stopCycleFlag = threading.Event()
		#self._cycleWatcher = delayTimer(self._stopCycleFlag, delay)
		#self._cycleWatcher.delayExpired.connect(self.next)
		#self._cycleWatcher.start()
		timer = threading.Timer(2, self.next)
		timer.start()

	def stopCycle(self):
		print "Stopping cycle!"
		self._isCycling = False

	def next(self):
		if self._isCycling:		
			timer = threading.Timer(2, self.next)
			timer.start()
			self.goToEvent(self._event + 1)

	def goToEvent(self, event):
		self._event = event
		self.eventChanged.emit()

	def isCycling(self):
		return self._isCycling

	def event(self):
		return self._event
