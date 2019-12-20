import PyCad

sphere=PyCad.gen.sphere(r=2).translate(1, 0, 2)
cylinder=PyCad.gen.cylinder(d=3, h=5)

fuse=PyCad.op.fuse(sphere, cylinder)
fuse.translate(7, 2, 0)
fuse.setColor(PyCad.Color(1, 0, 0, 1))

cut=PyCad.op.cut(cylinder, sphere)
cut.translate(-5, -2, 0)
cut.setColor(PyCad.Color(0, 0, 1, 1))

common=PyCad.op.common(cylinder, sphere)
common.translate(2, -8, 0)
common.setColor(PyCad.Color(0, 1, 0, 1))

result=[sphere, cylinder, fuse, cut, common]

## SNIP

doc="""
# Boolean operations with solids

(csg.jpg)

[CODE]
"""
