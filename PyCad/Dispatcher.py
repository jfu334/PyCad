import sys, os
from typing import *
import argparse

from . import PyCadCpp
from . import Project
from .InteractiveViewer import InteractiveViewer


"""
Implements a simple command line argument dispatcher. 
Can be used to provide various functionality through command line. 
"""

class SimpleDispatcher:
	
	def __init__(self, project):
		self._project=project
		
	def run(self):
		parser=argparse.ArgumentParser(description='CadSuite2 SimpleDispatcher')
		
		parser.add_argument('-e', '--export', type=str, action="append",
			help="Manually specify objects to create and export.")
		parser.add_argument('--export-all', action='store_true',
			help="Create and export all objects.")
		
		parser.add_argument('-d', '--display', type=str, nargs="+", action="append", 
			help="Show specified object(s) in viewer")
		
		args=parser.parse_args()
		
		
		exportObjects=[]
		if(args.export is not None):
			for o in args.export:
				obj=self._findObject(o)
				exportObjects.append(obj)
		elif(args.export_all):
			exportObjects=self._data
		
		## handle export
		#for obj in exportObjects:
			#print(f"Creating {obj['name']}...")
			#object_=obj["func"]()
			
			#if(object_["type"]!="object"):
				#raise Exception("Only object types can be exported!")
			
			#for exp in obj["export"]:
				#print(f"Exporting {obj['name']} to {exp['filename']}...")
				#self._handleExport(object_, exp)
		
		# handle display
		def showElement(obj_, view):
			if(isinstance(obj_, PyCadCpp.Object)):
				view.addObject(obj_)
			elif(isinstance(obj_, Project.ObjectGroup)):
				for i in obj_.objects():
					showElement(i, view)
			elif(isinstance(obj_, list)):
				for i in obj_:
					showElement(i, view)
			else:
				raise NotImplementedError(repr(obj_))
		
		
		if(args.display is not None):
			viewer=InteractiveViewer()
			
			for v in args.display:
				view=viewer.addView()
					
				for i in v:
					element=self._project.findElement(i)
					
					if(element["type"]=="object"):
						data=element["constructor"]()
						showElement(data, view)
					elif(element["type"]=="scene"):
						data=element["constructor"]()
						showElement(data, view)
				
			viewer.run()
		
	def _handleExport(self, object_, export):
		if(export["format"]=="stl"):
			PyCadCpp.export_.export_stl(export["filename"], object_, export["tolerance"])
		else:
			raise NotImplementedError()
	

		
