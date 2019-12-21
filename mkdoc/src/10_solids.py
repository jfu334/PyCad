import PyCad

sphere=PyCad.gen.sphere(r=3).translate(0, 0, 3)
cube=PyCad.gen.cube(3, 3, 3).translate(-6, -7, 0)
cylinder=PyCad.gen.cylinder(d=3, h=5).translate(6, 0, 0)
cone=PyCad.gen.cone(r1=2, r2=1, h=2).translate(2, -7, 0)

sphere.setColor(PyCad.colors.red)
cube.setColor(PyCad.colors.green)
cylinder.setColor(PyCad.colors.blue)
cone.setColor(PyCad.colors.yellow)

result=[sphere, cube, cylinder, cone]

## SNIP

doc="""
## Simple 3d objects

![Picture](10_solids.jpg | width=300)

[CODE]
"""
