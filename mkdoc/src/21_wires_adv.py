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

## SNIP

doc="""
## Advanced Lines 

PyCad offers an extermental curve design tool that allows to shape a curve with
control points and various other stuff. 

![Picture](21_wires_adv.jpg | width=300)

[CODE]
"""
