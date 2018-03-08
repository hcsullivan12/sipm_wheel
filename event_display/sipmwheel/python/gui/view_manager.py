from viewport import viewport
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import pyqtgraph as pg
import numpy
import math

class view_manager(QtCore.QObject):
	def __init__(self):
		super(view_manager, self).__init__()
		self._layout = QtGui.QVBoxLayout()

		self._detectorView = viewport()
		self._layout.addWidget(self._detectorView)

		self._plotView = None
		self.getPlot()
		#self._layout.addWidget(self._plotView)

		self._gl_hits = None

	def getLayout(self):
		return self._layout

	def getPlot(self):
		if self._plotView is not None:
			self._layout.removeWidget(self._plotView)
			self._plotView = None
		self._plotView = pg.GraphicsLayoutWidget()
		
		# Set titles
		plot = self._plotView.addPlot(title="SiPM Signals")
		plot.setLabel("bottom", "SiPM")
		plot.setLabel("left", "p.e.")

		# Create a random distribution
		vals = []
		for count in range(1,100):
			val = numpy.random.randint(1, 10)
			vals.append(val - 1)	

		# Fill
		y, x = numpy.histogram(vals, bins=8)

		# Set colors
		pg.setConfigOption('background', 'w')

		hist_pen = pg.mkPen(color=[0,0,100,255], width=4)
		hist_brush = pg.mkBrush(color=[0,0,255,255])

		point_pen = pg.mkPen(color=[0,0,0,255], width=2) 
		point_brush = pg.mkBrush(color=[0,0,0,255])

		hist = pg.PlotCurveItem(x, y, stepMode=True, pen=hist_pen, fillLevel=0, brush=hist_brush)
		plot.addItem(hist)
		
		# Add points 
		#getPoints()
		xtemp = []
		ytemp = []
		for i in range(0,8):
			xtemp.append(i + 0.5)	
		for i in range(0,8):
			error = numpy.random.randint(1,5)
			ytemp.append(y[i] + error)

		x_points = numpy.array(xtemp)
                y_points = numpy.array(ytemp)

		points = pg.ScatterPlotItem()
                points.setData(x_points, y_points, symbol='o', pen=None)
                points.setSize(20)
                points.setPen(point_pen)
                points.setBrush(point_brush)

                plot.addItem(points)

		# Add error bars
		top_error_temp = []
		bottom_error_temp = []
		for i in range(0,8):
			top_error_temp.append(2)
			bottom_error_temp.append(2)
		
		error_bars = pg.ErrorBarItem(beam=0.25, pen=point_pen)

                top_error = numpy.array(top_error_temp)
                bottom_error = numpy.array(bottom_error_temp)
		
		error_bars.setData(x=x_points,y=y_points,top=top_error,bottom=bottom_error)
                plot.addItem(error_bars)
		
		self._layout.addWidget(self._plotView)
		self._plotView.show()

	def updateVoxel(self):
		self.redraw()

	def updatePlot(self):
		self.getPlot()		

	def getDetectorView(self):
		return self._detectorView

	def replot(self):
		return

	def redraw(self):
	
                if self._gl_hits is not None:
			self.getDetectorView().removeItem(self._gl_hits)
			self._gl_hits = None
			
                pt_list = []
                nPoints = numpy.random.randint(1, 20)
                for i in range(1, nPoints):
                        x = numpy.random.uniform(-1,1)
                        y = numpy.random.uniform(-1,1)
			if math.sqrt(x*x + y*y) <= 1:
                        	pt_list.append( [x,y,0] )
		

                pts = numpy.array(pt_list)
		if len(pt_list) > 0:
			hits = gl.GLScatterPlotItem(pos=pts,color=(0,0,1,1), size=0.05, pxMode=False)
			self._gl_hits = hits
			self.getDetectorView().addItem(self._gl_hits)
			self.getDetectorView()._background_items.append(self._gl_hits)

