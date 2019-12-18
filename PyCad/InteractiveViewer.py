from PyQt5 import QtGui, QtCore, Qt, QtWidgets, QtOpenGL
from . import PyCadCpp

class MainWindow(QtWidgets.QMainWindow):
	def __init__(self, interactiveViewer):
		super().__init__()
		self._interactiveViewer=interactiveViewer
		
		self.resize(800, 600)
		self._views=[]
		self._viewLayout=QtWidgets.QGridLayout()
		
		# pack layout
		w=QtWidgets.QWidget()
		w.setLayout(self._viewLayout)
		self.setCentralWidget(w)
		
		# rest
		self.show()
		self.update()
	
	def addView(self):
		view=PyCadCpp.viewer.QtViewer()
		self._views.append(view)
		self._viewLayout.addWidget(view, 0, len(self._views)-1)
		view.init()
		view.update()
		
		return view
	
	def update(self):
		for i in self._views:
			i.update()
	
	def keyPressEvent(self, event):
		if(event.key()==QtCore.Qt.Key_R):
			self._interactiveViewer.slotReloadRequested()
	
	def resizeEvent(self, args):
		for i in self._views:
			i.update()


class InteractiveViewer:
	
	def __init__(self):
		self._app=QtWidgets.QApplication([])
		self._wnd=MainWindow(self)
		self._reloadRequestHandler=None
		
	def setReloadRequestHandler(self, handler):
		self._reloadRequestHandler=handler
	
	def slotReloadRequested(self):
		""" Is called when UI requested a reload """
		if(self._reloadRequestHandler is not None):
			self._reloadRequestHandler()
	
	def run(self):
		self._app.exec_()
	
	def addView(self):
		return self._wnd.addView()
	
