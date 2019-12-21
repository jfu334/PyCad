import PyCad

sphere=PyCad.gen.sphere(r=2).translate(1, 0, 2)
cylinder=PyCad.gen.cylinder(d=3, h=5)

fuse=PyCad.op.fuse(sphere, cylinder)
fuse.translate(7, 2, 0)
fuse.setColor(PyCad.colors.red)

cut=PyCad.op.cut(cylinder, sphere)
cut.translate(-5, -2, 0)
cut.setColor(PyCad.colors.blue)

common=PyCad.op.common(cylinder, sphere)
common.translate(2, -8, 0)
common.setColor(PyCad.colors.green)

result=[sphere, cylinder, fuse, cut, common]

## SNIP

doc="""
## Boolean operations with solids

![Picture](11_csg.jpg | width=300)

[CODE]
"""
