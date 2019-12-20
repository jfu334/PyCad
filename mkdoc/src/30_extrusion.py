import PyCad

bezier=PyCad.gen.bezier_3d([
	PyCad.Vec3(0, 0, 0), 
	PyCad.Vec3(4, 0, 0), 
	PyCad.Vec3(4, 2, 0)])

line=PyCad.gen.line_3d([
	PyCad.Vec3(4, 2, 0), 
	PyCad.Vec3(0, 2, 0), 
	PyCad.Vec3(0, 0, 0)])

outline=PyCad.op.fuse(bezier, line)

outline1=outline.copy().translate(-2, -7, 0)
face1=PyCad.op.wireToShell(outline1)

shell1=PyCad.op.extrude_linear(outline1, h=1).translate(-5, 0, 0)
shell1.setColor(PyCad.colors.green)

solid1=PyCad.op.extrude_linear(face1, h=1).translate(5, 0, 0)
solid1.setColor(PyCad.colors.red)

outline2=outline.copy().translate(0, -3, 0)
face2=PyCad.op.wireToShell(outline2)

shell2=PyCad.op.extrude_rotate(outline2, 180, PyCad.Vec3(1, 0, 0))
shell2.translate(-8, 3, 0)
shell2.setColor(PyCad.colors.yellow)

solid2=PyCad.op.extrude_rotate(face2, 270, PyCad.Vec3(1, 0, 0))
solid2.translate(4, 3, 0)
solid2.setColor(PyCad.colors.cyan)

result=[outline1, shell1, solid1, shell2, solid2]

## SNIP

doc="""
## Extrusion

![Picture](20_wires.jpg)

Extrusion of a wire leads to a shell. Extrusion of a shell leads to a solid. 

[CODE]
"""
