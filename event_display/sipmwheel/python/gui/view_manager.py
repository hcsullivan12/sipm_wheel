from viewport import viewport
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import pyqtgraph as pg
import numpy

class view_manager(QtCore.QObject):
	def __init__(self):
		super(view_manager, self).__init__()
		self._layout = QtGui.QVBoxLayout()

		self._detectorView = viewport()
		self._layout.addWidget(self._detectorView)

		self._plotView = self.getPlot()
		self._layout.addWidget(self._plotView)

	def getLayout(self):
		return self._layout

	def getPlot(self):
		plot_widget = pg.GraphicsLayoutWidget()
		
		#plot_widget.resize(800,350)
		plt1 = plot_widget.addPlot(title="SiPM Signals")
		plt1.setLabel("bottom", "SiPM")
		plt2 = plot_widget.addPlot(title="SiPM Signals")
		plt2.setLabel("bottom", "SiPM")

		## make interesting distribution of values
		vals = numpy.hstack([numpy.random.normal(size=500), numpy.random.normal(size=260, loc=4)])

		## compute standard histogram
		y,x = numpy.histogram(vals, bins=numpy.linspace(0, 8, 40))

		## Using stepMode=True causes the plot to draw two lines for each sample.
		## notice that len(x) == len(y)+1
		plt1.plot(x, y, stepMode=True, fillLevel=0, brush=(0,0,255,150))
		plt2.plot(x, y, stepMode=True, fillLevel=0, brush=(0,0,255,150))

		plot_widget.show()
		return plot_widget
