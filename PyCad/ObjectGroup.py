

class ObjectGroup:
	def __init__(self, *args, color=None):
		self._objects=list(args)
		
		if(color is not None):
			for i in self._objects:
				i.setColor(color)
		
		
	def addObject(self, object_):
		self._objects.append(object_)
	def objects(self):
		return list(self._objects)
	
	def copy(self):
		return ObjectGroup(*[i.copy() for i in self._objects])
	def translate(self, x, y, z):
		for i in self._objects:
			i.translate(x, y, z)
		return self
	def rotate(self, x, y, z):
		for i in self._objects:
			i.rotate(x, y, z)
		return self
