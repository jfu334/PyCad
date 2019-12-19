from . import PyCadCpp

def curve_experimental(definition, close=False):
	
	def getPropertyOfPoint(i, properties, default=None):
		for p in properties:
			if(p in definition[i][1]):
				return definition[i][1][p]
		
		if(default is None):
			raise Exception(f"Any of {', '.join(properties)} required!")
		else:
			return default
	
	def anyOf(i, properties):
		for p in properties:
			if(p in definition[i][1] and definition[i][1][p]):
				return True
		return False
	
	#
	# compute additional data for points
	#
	
	definition_data=[]
	for i in range(len(definition)):
		data={}
		
		pprev=definition[i-1] if i>0 or close else None
		pthis=definition[i]
		pnext=definition[(i+1)%len(definition)] if i<len(definition)-1 or close else None
		
		# compute incomming and outgoing vectors and their norm
		if(pprev is not None):
			vin=pthis[0]-pprev[0]
			data["vec_in"]=vin
			data["vec_in_norm"]=vin.norm()
		
		if(pnext is not None):
			vout=pnext[0]-pthis[0]
			data["vec_out"]=vout
			data["vec_out_norm"]=vout.norm()
			
		# compute tangential vector, that is the direction the 
		# line passes through the point. 
		vtang=PyCadCpp.Vec3(0, 0, 0)
		if("vec_in_norm" in data):
			vtang+=data["vec_in_norm"]
		if("vec_out_norm" in data):
			vtang+=data["vec_out_norm"]
		
		data["vec_tangential_in"]=getPropertyOfPoint(i, ["dir", "dir_in"], vtang).norm()
		data["vec_tangential_out"]=getPropertyOfPoint(i, ["dir", "dir_out"], vtang).norm()
		
		definition_data.append(data)
		
			
	# 
	# create curve segments
	# 
	debug=[]
	curve=None
	
	# create debug points
	for i in definition:
		p=PyCadCpp.gen.point_3d(i[0])
		p.setColor(PyCadCpp.Color(1, 0, 0, 1))
		debug.append(p)
	
	for i in range(len(definition)-1) if not close else range(-1, len(definition)-1):
		
		p1=definition[i]
		p1d=definition_data[i]
		
		p2=definition[i+1]
		p2d=definition_data[i+1]
		
		# compute points in between
		supports=[]
		if(anyOf(i, ["smooth_out", "smooth"])):
			weight=getPropertyOfPoint(i, ["weight", "weight_out"])
			v=p1[0]+p1d["vec_tangential_out"]*weight
			supports.append(v)
			
			pdbg=PyCadCpp.gen.point_3d(v)
			pdbg.setColor(PyCadCpp.Color(0, 0, 1, 1))
			debug.append(pdbg)
			
			ldbg=PyCadCpp.gen.line_3d([p1[0], v])
			ldbg.setColor(PyCadCpp.Color(0, 0, 1, 1))
			debug.append(ldbg)
		
		if("support" in p1[1]):
			supports+=p1[1]["support"]
			
			for j in p1[1]["support"]:
				pdbg=PyCadCpp.gen.point_3d(j)
				pdbg.setColor(PyCadCpp.Color(0, 1, 0, 1))
				debug.append(pdbg)
		
		if(anyOf(i+1, ["smooth_in", "smooth"])):
			weight=getPropertyOfPoint(i+1, ["weight", "weight_in"])
			v=p2[0]-p2d["vec_tangential_in"]*weight
			supports.append(v)
		
			pdbg=PyCadCpp.gen.point_3d(v)
			pdbg.setColor(PyCadCpp.Color(0, 0, 1, 1))
			debug.append(pdbg)
			
			ldbg=PyCadCpp.gen.line_3d([p2[0], v])
			ldbg.setColor(PyCadCpp.Color(0, 0, 1, 1))
			debug.append(ldbg)
		
		# build and add bezier
		c=PyCadCpp.gen.bezier_3d([p1[0]]+supports+[p2[0]])
		curve=c if curve is None else PyCadCpp.op.fuse(curve, c)
	
	debug.append(curve)
	return {"result": curve, "debug": debug}
