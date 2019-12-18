from . import PyCadCpp

for i in dir(PyCadCpp.op):
	if(not i.startswith("_")):
		globals()[i]=getattr(PyCadCpp.op, i)
