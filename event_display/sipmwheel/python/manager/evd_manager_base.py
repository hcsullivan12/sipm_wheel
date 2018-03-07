from pyqtgraph.Qt import QtCore
from io_manager import io_manager

class evd_manager_base(QtCore.QObject):
	
	# Signals for changing event
	eventChanged = QtCore.pyqtSignal()
	
	def __init__(self):
		super(evd_manager_base, self).__init__()
		
		# Initialize io manager
		self._io_manager = io_manager()
		self._io_manager.eventChanged.connect(self.eventChangedEcho)

	def eventChangedEcho(self):
		self.eventChanged.emit()

	def io(self):
		return self._io_manager

	def event(self):
		return self._io_manager.event()

	def run(self):
		return 1

	def subrun(self):
		return 1
		
