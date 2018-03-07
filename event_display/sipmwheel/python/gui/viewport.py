try:
	import pyqtgraph.opengl as gl
except:
	print "Error, must have pyqtgraph to use this viewer."
	exit(1)

from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph as pg
import numpy
import math

from ROOT import wheelevd

class viewport(gl.GLViewWidget):	

	def __init__(self):
		super(viewport, self).__init__()
		
		# Add a view box
		# Add an image item
		self.setBackgroundColor((0,0,0,255))
		self._background_items =  []
		self.drawDetector()
		self.drawAxes()
		self.setSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
		#self.show()

	def drawDetector(self):
		#self.setCenter((0,0,0))

		#pts = numpy.array([[0,0,0], [1,0,0],
                                 #[1,1,0], [0,1,0],
                                 #[0,0,0], [0,0,1],
                                 #[1,0,1], [1,1,1],
                                 #[0,1,1], [0,0,1],
                                 #[1,0,1], [1,0,0],
                                 #[1,1,0], [1,1,1],
                                 #[0,1,1], [0,1,0]],
                                 #dtype=float)

		for _item in self._background_items:
			self.removeItem(_item)
			self._background_items = []

		nPoints = 20
		radius = 1
		height = 0.1
		pt_list = []
		# Bottom Face
		for i in range(0, nPoints + 1):
			x = radius*math.cos( i*(2*math.pi/nPoints) ) 
			y = radius*math.sin( i*(2*math.pi/nPoints) )
			z = 0
			pt_list.append( [x,y,z] )
		# Top Face
		for i in range(0, nPoints + 1):
			x = radius*math.cos( i*(2*math.pi/nPoints) )
			y = radius*math.sin( i*(2*math.pi/nPoints) )
			z = height
			pt_list.append( [x,y,z] )
		pts = numpy.array(pt_list)

	
		self._det_outline = gl.GLLinePlotItem(pos=pts,color=(1,1,1,1), width=3)
		self.addItem(self._det_outline)
		self._background_items.append(self._det_outline)

	def drawAxes(self):
		self._axes = gl.GLAxisItem()
		self._axes.setSize(1,1,1)
		self.addItem(self._axes)
		self._background_items.append(self._axes)
