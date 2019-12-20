#!/usr/bin/env python3
from PyQt5 import QtGui, QtCore, Qt, QtWidgets, QtOpenGL
import sys, os, math, traceback, io
import importlib, importlib.util

sys.path.insert(0, "/home/nu/Data/Development/CAD/PyCad")
import PyCad

def loadFile(path):
	spec=importlib.util.spec_from_file_location(f"sample_{path}", path)
	module=importlib.util.module_from_spec(spec)
	spec.loader.exec_module(module)
	return module

specificExample=sys.argv[1] if len(sys.argv)>=2 else None


# this method is called from the Qt app
def makeScene(view):
	
	# set initial view for camera
	view.setTarget(0, 0, 0)
	view.setCamera(math.radians(-90), math.radians(30), 20)
	
	# if a specific sample has been requested, show it - otherwise, 
	# export all samples found
	if(specificExample):
		try:
			m=loadFile(specificExample)
			result=m.result
			view.clearObjects()
			for o in result:
				showElement(o, view)
		except:
			traceback.print_exc()
		
	else:
		doctext=io.StringIO()
		doctext.write("# Examples\n")
		
		samples=[i for i in os.listdir("src") if i.endswith(".py")]
		for s in sorted(samples):
			m=loadFile(os.path.join("src", s))
			result=m.result
			
			# clear view, show items
			view.clearObjects()
			for o in result:
				showElement(o, view)
			
			# save image to file
			imgFile=os.path.join("out", s.split(".")[0]+".jpg")
			view.update()
			view.saveImage(imgFile)
			
			# NOTE: OCC seems to have a bug, always saves image as PBM. (D'Oh)
			# Run imageMagick to make a jpg of it. 
			os.system(f"convert {imgFile} -quality 95 {imgFile}")
			
			# extract code from example
			src=[i.replace("\n", "").replace("\r", "") for i in open(os.path.join("src", s), "r")]
			if("## SNIP" in src):
				src=src[:src.index("## SNIP")]
			else:
				src=None
			
			if(src is not None):
				code=["```python"]
				code+=src
				code+=["```"]
			else:
				code=[]
				
			# get doc text, write it to output file
			doc=m.doc
			doc=doc.replace("[CODE]", "\n".join(code))
			doctext.write(doc+"\n\n")
			
		with open(os.path.join("out", "samples.md"), "w") as f:
			f.write(doctext.getvalue())
			
		app.exit()

#
# make  viewer
#

def showElement(obj_, view):
	if(isinstance(obj_, PyCad.brep.Object)):
		vobj=PyCad.viewer.ViewObject(obj_)
		vobj.setColor(obj_.color())
		view.addObject(vobj)
		
	elif(isinstance(obj_, PyCad.GeometryBase)):
		vobj=PyCad.viewer.ViewObject(obj_)
		vobj.setColor(PyCad.Color(1, 1, 1, 1))
		view.addObject(vobj)
	elif(isinstance(obj_, PyCad.Vec3)):
		vobj=PyCad.viewer.ViewObject(PyCad.gen.point_3d(obj_));
		vobj.setColor(PyCad.Color(1, 0, 0, 1))
		view.addObject(vobj)
	elif(isinstance(obj_, PyCad.ObjectGroup)):
		for i in obj_.objects():
			showElement(i, view)
	elif(isinstance(obj_, list)):
		for i in obj_:
			showElement(i, view)
	else:
		raise NotImplementedError(repr(obj_))


class MainWindow(QtWidgets.QMainWindow):
	def __init__(self):
		super().__init__()
		
		self._view=PyCad.viewer.QtViewer()
		self._view.init()
		
		l=QtWidgets.QGridLayout()
		l.addWidget(self._view, 0, 0)
		
		# pack layout
		w=QtWidgets.QWidget()
		w.setLayout(l)
		self.setCentralWidget(w)
		
		self.resize(1024, 768)
		
		# cb
		self._timer=QtCore.QTimer()
		self._timer.timeout.connect(self._slotTimeout)
		self._timer.start(1000)
        
	def _slotTimeout(self):
		makeScene(self._view)
		self._timer.stop()

	def keyPressEvent(self, event):
		if(event.key()==QtCore.Qt.Key_R):
			makeScene(self._view)
	
app=QtWidgets.QApplication([])
wnd=MainWindow()
wnd.show()
app.exec_()

