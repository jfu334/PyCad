from . import PyCadCpp
from . import op, gen, calc
from . import modelling

# import global symbols from PyCadCpp
for i in dir(PyCadCpp):
	if(not i.startswith("_") and i not in ("op", "gen")):
		globals()[i]=getattr(PyCadCpp, i)

from .ObjectGroup import ObjectGroup
from .Project import Project

# import py stuff
from .Dispatcher import SimpleDispatcher
