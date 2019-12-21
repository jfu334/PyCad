import PyCad

solid1=PyCad.gen.cube(6, 6, 6, center=True)
solid1=PyCad.op.fuse(solid1, PyCad.gen.cylinder(d=3, h=4).translate(0, 0, 3))
solid1.translate(-5, 0, 0)

solid2=solid1.copy().translate(10, 0, 0)

# Selecting the edges can be a pain in the ass at the moment. 
# Will be improved when I know how. 

def edgesNear(solid, pnt, distance):
	return [i for i in solid.edges() if PyCad.calc.distance(i, pnt)<distance]

pnt=PyCad.Vec3(4, 0, 2)
edges=edgesNear(solid2, pnt, 2)

pnt=PyCad.Vec3(5, 0, -3)
edges+=edgesNear(solid2, pnt, 5)

# use this to see what edges have been selected, maybe remove solid2 to 
# only see the edges. 
# result=[solid1, solid2, pnt]+edges

solid2=PyCad.op.fillet(solid2, edges, 1)
solid2.setColor(PyCad.colors.green)
result=[solid1, solid2]


## SNIP

doc="""
## Fillets

![Picture](40_fillets.jpg)

Fillets allow to smooth edges. For this feature the internal structure of an object has to be explored in order to select the edges to smooth.  

[CODE]
"""
