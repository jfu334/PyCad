import PyCad

sphere=PyCad.gen.sphere(r=3).translate(0, 0, 3)
cube=PyCad.gen.cube(3, 3, 3).translate(-6, -7, 0)
cylinder=PyCad.gen.cylinder(d=3, h=5).translate(6, 0, 0)
cone=PyCad.gen.cone(r1=2, r2=1, h=2).translate(2, -7, 0)

sphere.setColor(PyCad.Color(1, 0, 0, 1))
cube.setColor(PyCad.Color(0, 1, 0, 1))
cylinder.setColor(PyCad.Color(0, 0, 1, 1))
cone.setColor(PyCad.Color(1, 1, 0, 1))

result=[sphere, cube, cylinder, cone]

## SNIP

doc="""
# Simple 3d objects

(solids.jpg)

[CODE]
"""
