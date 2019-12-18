from . import PyCadCpp

for i in dir(PyCadCpp.gen):
	if(not i.startswith("_")):
		globals()[i]=getattr(PyCadCpp.gen, i)

# override some methods to provide more conveinience
def bezier_surface(vecs):
	
	# determine nr of rows and cols
	rows=len(vecs)
	cols=len(vecs[0])
	
	if(any(len(i)!=cols for i in vecs)):
		raise Exception("Varying number of column vectors!")
	
	data=[]
	for i in vecs:
		data+=i

	return PyCadCpp.gen.bezier_surface(rows, cols, data)
	
	
