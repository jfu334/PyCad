#!/usr/bin/env python3
import sys, os, traceback
import importlib, importlib.util


sys.path.insert(0, "/home/nu/Data/Development/CAD/PyCad")
import PyCad

argProjectFile=sys.argv[1]
argViews=sys.argv[2:]

def projectToView(project, viewer):
	pass


specProject=importlib.util.spec_from_file_location("moduleProject", argProjectFile)
moduleProject=importlib.util.module_from_spec(specProject)
specProject.loader.exec_module(moduleProject)


import PyCad.Project
from PyCad.InteractiveViewer import InteractiveViewer

viewer=InteractiveViewer()

def showElement(obj_, view):
	if(isinstance(obj_, PyCad.ModellingObjectBase)):
		vobj=PyCad.viewer.ViewObject(obj_)
		vobj.setColor(obj_.color())
		view.addObject(vobj)
	elif(isinstance(obj_, PyCad.ObjectGroup)):
		for i in obj_.objects():
			showElement(i, view)
	elif(isinstance(obj_, list)):
		for i in obj_:
			showElement(i, view)
	else:
		raise NotImplementedError(repr(obj_))

views=[]

def reloadProject():
	try:
		specProject.loader.exec_module(moduleProject)
		
		for i in range(len(argViews)):
			views[i].clearObjects()
			
			element=moduleProject.project.findElement(argViews[i])
			if(element["type"]=="object"):
				data=element["constructor"]()
				showElement(data, views[i])
			elif(element["type"]=="scene"):
				data=element["constructor"]()
				showElement(data, views[i])
	except:
		traceback.print_exc()
		
# make views
for v in argViews:
	views.append(viewer.addView())
	
# reload project
reloadProject()

viewer.setReloadRequestHandler(reloadProject)

viewer.run()





