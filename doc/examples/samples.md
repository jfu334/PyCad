# Examples

## Simple 3d objects

![Picture](10_solids.jpg)

```python
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

```



## Boolean operations with solids

![Picture](11_csg.jpg)

```python
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

```



## Lines 

![Picture](20_wires.jpg)

```python
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

```



## Advanced Lines 

PyCad offers an experimental curve design tool that allows to shape a curve with
control points and various other stuff. 

![Picture](21_wires_adv.jpg)

```python
import PyCad


# returns a dict with keys 'result' and 'debug'. The latter can be used to 
# visualize the defining aspects of the curve. 
curve=PyCad.modelling.curve_experimental_v1([
	{"name": "p1", "p": PyCad.Vec3(0, -4, 3), 
		"smooth": True, "weight": 1, 
		"dir": PyCad.Vec3(6, -4, 0)},
	
	{"name": "p2", "p": PyCad.Vec3(4, -4, 3), 
		"smooth": True, "weight": 2},
	
	{"name": "p3", "p": PyCad.Vec3(4, 0, 3), 
		"smooth": True, "weight": 3, 
		"dir": PyCad.Vec3(-1, 0, 0), 
		"support": [PyCad.Vec3(-4, 10, 3)]},
	
	{"name": "p4", "p": PyCad.Vec3(-4, 4, 3), 
		"smooth": True, "weight_in": 2, "weight_out": 1},
	
	{"name": "p5", "p": PyCad.Vec3(-6, 0, 3)},
], close=True)

shell=PyCad.op.extrude_linear(curve["result"], -0.5).translate(0, 0, -6)
shell.setColor(PyCad.colors.green)

result=[shell, curve["result"]]+ curve["debug"]

```



## Shells 

Planar wires can be closed to a shell with PyCad.op.wireToShell. 

PyCad.op.filling can be used for non-planar wires. 
It also allows to specify support wires that define the curvature. 

![Picture](25_shells.jpg)

```python
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

```



## Extrusion

![Picture](30_extrusion.jpg)

Extrusion of a wire leads to a shell. Extrusion of a shell leads to a solid. 

```python
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

```



## Fillets

![Picture](40_fillets.jpg)

Fillets allow to smooth edges. For this feature the internal structure of an object has to be explored in order to select the edges to smooth.  

```python
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


```



## Pipes

![Picture](50_pipes.jpg)

Piping shapes can be seen as a generalized variant of extrusion. It allows to 
extrude a shape along a predefined path with predefined orientation. 

```python
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


```


