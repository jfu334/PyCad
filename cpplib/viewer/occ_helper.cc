#include "occ_helper.h"
#include <OpenGl_GraphicDriver.hxx>

namespace PyCadCpp::viewer::helper
{
	namespace 
	{
		static Handle(Graphic3d_GraphicDriver) driver;
	}
	
	
	Handle(V3d_Viewer) newViewer(Handle(Aspect_DisplayConnection) displayConnection)
	{
		if(driver.IsNull())
			driver=new OpenGl_GraphicDriver(displayConnection);
		
		return new V3d_Viewer(driver);
	}
}
