from typing import *

class Project:
	
	def __init__(self):
		self._elements=[]
	
	def addObject(self, name:str, constructor:Callable):
		self._elements.append({"type": "object", "name": name, "constructor": constructor})
	
	def addScene(self, name:str, constructor:Callable):
		self._elements.append({"type": "scene", "name": name, "constructor": constructor})
	
	def findElement(self, name:str):
		try:
			return next(i for i in self._elements if i["name"]==name)
		except StopIteration:
			raise Exception(f"Element '{name}' not found!")
	
	
