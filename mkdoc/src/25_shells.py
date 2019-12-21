import PyCad

#
# flat shell
#
line=PyCad.gen.line_3d([
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(-2, 0, 0), 
	PyCad.Vec3(-4, 3, 0), 
	PyCad.Vec3(-4, 6, 0), 
	PyCad.Vec3(3, 5, 0)])
bezier=PyCad.gen.bezier_3d([
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(3, 0, 0), 
	PyCad.Vec3(3, 5, 0)])

outline1=PyCad.op.fuse(line, bezier).translate(-5, 5, 0)
outline1.setColor(PyCad.colors.green)

shell1=PyCad.op.wireToShell(outline1).translate(0, -7, 0)
shell1.setColor(PyCad.colors.green)

#
# curved shell
#

bezier_points=[
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(0, 5, 0), 
	PyCad.Vec3(4, 4, 6), 
	PyCad.Vec3(8, 5, 0), 
	PyCad.Vec3(8, 0, 0)]

bezier=PyCad.gen.bezier_3d(bezier_points)

outline2=PyCad.op.fuse(bezier, bezier.copy().mirror(0, 1, 0).mirror(0, 0, 1))
outline2.setColor(PyCad.colors.red)

support2=PyCad.gen.bezier_3d([
	bezier_points[0], 
	(bezier_points[0]+bezier_points[-1])*0.5+PyCad.Vec3(0, -2, 5),
	bezier_points[-1]])

support2.setColor(PyCad.colors.yellow)

# "3" is the degree of the interpolated bspline surface
# multiple supports can be specified. The specified outlines must enclose the shell. 
shell2=PyCad.op.filling(2, [outline2], [support2])

# colors can be created this way, the last parameter is 
# optional and defines transparency. 
shell2.setColor(PyCad.Color(0, 1, 1, 0.25))

# move elements to improve visibility
for i in [outline2, support2, shell2]:
	i.rotate(0, 0, 90)
	i.translate(5, -3, 0)

outline2.translate(0, 0, -0.25)

result=[outline1, shell1, outline2, support2, shell2]

## SNIP

doc="""
## Shells 

Planar wires can be closed to a shell with PyCad.op.wireToShell. 

PyCad.op.filling can be used for non-planar wires. 
It also allows to specify support wires that define the curvature. 

![Picture](25_shells.jpg)

[CODE]
"""
