import PyCad

outline=PyCad.gen.line_3d([
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(1, 0, 0), 
	PyCad.Vec3(1, 1, 0), 
	PyCad.Vec3(0, 1, 0), 
	PyCad.Vec3(0, 0, 0)
])
outline1=outline.copy().rotate(0, 45, 0)

path1=PyCad.gen.bezier_3d([
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(3, 0, 3), 
	PyCad.Vec3(0, 0, 6),
])

solid1a=PyCad.op.pipe_solid(outline1, path1)
solid1a.translate(3, 0, 0)
solid1a.setColor(PyCad.colors.green)

solid1b=PyCad.op.pipe_shell(outline1, path1)
solid1b.translate(6, 0, 0)
solid1b.setColor(PyCad.colors.green)

outline2=outline.copy().translate(-3, 0, 0)
path2=PyCad.gen.line_3d([
	PyCad.Vec3(-2.5, 0.5, 0), 
	PyCad.Vec3(-2.5, 0.5, 6)])

orientation2=PyCad.gen.helix(d=3, revolutions=2, ascend=6/2).translate(-2.5, 0.5, 0)

solid2=PyCad.op.pipe_solid(outline2, path2, orientation2)
solid2.translate(-5, 0, 0)
solid2.setColor(PyCad.colors.yellow)


outline3=PyCad.gen.line_3d([
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(0.3, 0, 0), 
	PyCad.Vec3(0.3, 0, 0.2), 
	PyCad.Vec3(0, 0, 0.2), 
	PyCad.Vec3(0, 0, 0)
	]).translate(1, -7, 0)

path3=PyCad.gen.helix(d=2, revolutions=5, ascend=0.6).translate(0, -7, 0)
orientation3=PyCad.gen.helix(d=3, revolutions=5, ascend=0.6).translate(0, -7, 0)

solid3=PyCad.op.pipe_solid(outline3, path3, orientation3)
solid3.translate(-5, 0, 0)
solid3.setColor(PyCad.colors.red)

result=[outline1, path1, solid1a, solid1b, 
	outline2, path2, orientation2, solid2, 
	outline3, path3, orientation3, solid3]


## SNIP

doc="""
## Pipes

![Picture](50_pipes.jpg)

Piping shapes can be seen as a generalized variant of extrusion. It allows to 
extrude a shape along a predefined path with predefined orientation. 

[CODE]
"""
