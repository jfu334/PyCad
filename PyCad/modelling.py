from .PyCadCpp import *

def curve2d_experimental(pts):
	curve=None
	for i in range(len(pts)-1):
		
		p1=pts[i]
		p2=pts[i+1]
		
		vp1=(p2[0]-p1[0]).norm()
		if(i>0):
			vp1=(vp1+(p1[0]-pts[i-1][0]).norm()).norm()
			
		vp2=(p2[0]-p1[0]).norm()
		if(i+2<len(pts)):
			vp2=(vp2+(pts[i+2][0]-p2[0]).norm()).norm()
		
		support=[]
		
		if(len(p1)>1 and "w_out" in p1[1]):
			support.append(p1[0]+vp1*p1[1]["w_out"])
		if(len(p2)>1 and "w_in" in p2[1]):
			support.append(p2[0]-vp2*p2[1]["w_in"])
		
		c=gen.bezier_2d([p1[0]]+support+[p2[0]])
		if(curve is None):
			curve=c
		else:
			curve=op.fuse(curve, c)
	return curve
