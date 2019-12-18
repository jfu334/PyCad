from . import PyCadCpp

for i in dir(PyCadCpp.calc):
	if(not i.startswith("_")):
		globals()[i]=getattr(PyCadCpp.calc, i)

