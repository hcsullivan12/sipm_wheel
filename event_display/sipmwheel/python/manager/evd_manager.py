from pyqtgraph.Qt import QtCore, QtGui
from evd_manager_base import evd_manager_base

try:
	import pyqtgraph.opengl as gl
except:
	print "Need pyqt opengl. Exiting..."
	exit()

class evd_manager(evd_manager_base):
	
	def __init__(self):
		super(evd_manager_base, self).__init__()

