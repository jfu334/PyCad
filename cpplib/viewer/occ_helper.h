#pragma once
#include <V3d_Viewer.hxx>
#include <Aspect_DisplayConnection.hxx>

namespace PyCadCpp::viewer::helper
{
	/* NOTE: There seems to be a header conflict between Qt and OCC. So when including
	 * the graphics driver stuff from occ, we can't include qt headers and vice versa. 
	 * 
	 * To circument this, we do this handling in a separate source that does not 
	 * include Qt. 
	 * 
	 * See: https://www.opencascade.com/content/problem-openglglfunctionshxx-occ-version-681
	 */
	
	Handle(V3d_Viewer) newViewer(Handle(Aspect_DisplayConnection) displayConnection);
}
