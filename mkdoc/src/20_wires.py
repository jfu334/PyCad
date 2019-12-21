import PyCad

line=PyCad.gen.line_3d([
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(-3, 0, 0), 
	PyCad.Vec3(-6, 3, 0), 
	PyCad.Vec3(-6, 6, 0)])
line.setColor(PyCad.colors.red)

bezier=PyCad.gen.bezier_3d([
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(5, 0, 0), 
	PyCad.Vec3(5, 5, 0)])
bezier.setColor(PyCad.colors.green)

fuse=PyCad.op.fuse(line, bezier)
fuse.setColor(PyCad.colors.blue)
fuse.translate(0, 0, 3)

helix=PyCad.gen.helix(d=3, revolutions=5, ascend=0.4)
helix.translate(-5, -5, 0)

result=[line, bezier, fuse, helix]

## SNIP

doc="""
## Lines 

![Picture](20_wires.jpg | width=300)

[CODE]
"""
