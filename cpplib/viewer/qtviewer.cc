#include "qtviewer.h"

#include <iostream>
#include <QMouseEvent>

#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Xw_Window.hxx>

#include <AIS_Point.hxx>
#include <AIS_Shape.hxx>
#include <AIS_ColoredShape.hxx>
#include <Prs3d_PointAspect.hxx>

#include <Image_PixMap.hxx>
#include <Image_AlienPixMap.hxx>

#include "occ_helper.h"

#include "objects.h"

namespace PyCadCpp::viewer
{
	
	QtViewer::QtViewer()
	{

	}
	
	void QtViewer::init()
	{
		Handle(Aspect_DisplayConnection) displayConnection=new Aspect_DisplayConnection();
		auto window_handle=static_cast<WId>(winId());
		Handle(Xw_Window) window=new Xw_Window(displayConnection, (Window)window_handle);
		
		_occViewer=helper::newViewer(displayConnection);
		_occView=_occViewer->CreateView();
		
		_occView->SetWindow(window);
		if(!window->IsMapped()) window->Map();
		
		_occViewer->SetDefaultLights();
		_occViewer->SetLightOn();
		
		_occView->SetBackgroundColor(Quantity_Color(1, 1, 229/255.0, Quantity_TOC_RGB));
		
		_occView->MustBeResized();
		_occView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_BLACK, 0.14, V3d_ZBUFFER);
		_occView->SetImmediateUpdate(Standard_False);
		
		_occView->Camera()->SetProjectionType(Graphic3d_Camera::Projection::Projection_Perspective);
		_occView->SetBackFacingModel(V3d_TOBM_ALWAYS_DISPLAYED);
		_occView->SetShadingModel(V3d_FLAT);
		
// 		_occView->ChangeRenderingParams().Method=Graphic3d_RM_RAYTRACING;
		_occView->ChangeRenderingParams().TransparencyMethod=Graphic3d_RTM_BLEND_OIT;
		_occView->ChangeRenderingParams().IsAntialiasingEnabled=true;
// 		_occView->ChangeRenderingParams().NbMsaaSamples=8;
		_occView->ChangeRenderingParams().RenderResolutionScale=2;
		_occView->ChangeRenderingParams().Resolution=96;
		
		_occContext=new AIS_InteractiveContext(_occViewer);
		_occContext->SetDisplayMode(AIS_Shaded, Standard_True);
		
		_occContext->SetDeviationCoefficient(5e-5);
// 		_occContext->SetDeviationAngle(5e-5);
		
		_occContext->DefaultDrawer()->SetFaceBoundaryDraw(true);
		
		_navHandler.setDistance(100);
		updateByNavHandler();
		
		
		_initialized=true;
	}
	
	void QtViewer::update()
	{
		if(!_initialized)
			return;
		
		_occView->MustBeResized();
		_occView->Redraw();
	}
	
	void QtViewer::setTarget(double x, double y, double z)
	{
		_navHandler.setTarget(Vec3(x, y, z));
		updateByNavHandler();
	}
	void QtViewer::setCamera(double azimuth, double elevation, double distance)
	{
		_navHandler.setAzimuth(azimuth);
		_navHandler.setElevation(elevation);
		_navHandler.setDistance(distance);
		updateByNavHandler();
	}
	
	void QtViewer::saveImage(std::string path)
	{
		Image_AlienPixMap image;
		
		_occView->ToPixMap(image, size().width(), size().height());
		
		// it's wrong to hard code this, should retrieve the format somehow. 
		// without, colors are swapped. 
		image.SetFormat(Image_PixMap::ImgBGR);
		
		image.Save(path.c_str());
	}
	
	void QtViewer::addObject(ViewObject* object)
	{
		if(!_initialized)
			return;
		
		Handle(AIS_ColoredShape) aisShape=new AIS_ColoredShape(object->object()->shape());
		
		Graphic3d_MaterialAspect material;
		material.SetReflectionModeOff(Graphic3d_TOR_SPECULAR);
		material.SetReflectionModeOff(Graphic3d_TOR_EMISSION);
		
		auto color=object->color();
		auto occ_color=Quantity_Color(color.r(), color.g(), color.b(), Quantity_TOC_RGB);
		
		aisShape->SetMaterial(material);
		aisShape->SetColor(occ_color);
	
		Handle(Prs3d_PointAspect) myPointAspect=new Prs3d_PointAspect(Aspect_TOM_POINT, occ_color, 20);
		aisShape->Attributes()->SetPointAspect(myPointAspect); 
	
		_occContext->Display(aisShape, Standard_True);
		_occContext->SetTransparency(aisShape, 1-color.alpha(), Standard_True);
	}
	void QtViewer::removeObject(ViewObject* object)
	{
		// TODO
	}
	void QtViewer::clearObjects()
	{
		if(!_initialized)
			return;
		
		_occContext->RemoveAll(Standard_True);
	}
	
	void QtViewer::paintEvent(QPaintEvent* event_)
	{
		if(!_initialized)
			return;
		
		_occView->Redraw();
	}
	void QtViewer::resizeEvent(QResizeEvent* event_)
	{
		if(!_initialized)
			return;
		
		_occView->MustBeResized();
		_occView->Redraw();
	}

	void QtViewer::mousePressEvent(QMouseEvent* event_)
	{
		if(!_initialized)
			return;
		
		if(_uiDragMode==DragMode::Inactive)
		{
			if(event_->button()==Qt::LeftButton)
				_uiDragMode=DragMode::Rotate;
			else if(event_->button()==Qt::RightButton)
				_uiDragMode=DragMode::Move;
			else if(event_->button()==Qt::MiddleButton)
				_uiDragMode=DragMode::Move2;
			
			_uiDragPositionLast=event_->screenPos();
		}
	}
	void QtViewer::mouseReleaseEvent(QMouseEvent* event_)
	{
		if(!_initialized)
			return;
		
		_uiDragMode=DragMode::Inactive;
	}
	void QtViewer::mouseMoveEvent(QMouseEvent * event_)
	{
		if(!_initialized)
			return;
		
		if(_uiDragMode!=DragMode::Inactive)
		{
			auto s=size();
			auto pos=event_->screenPos();
			
			double dx=(pos.x()-_uiDragPositionLast.x())/s.width();
			double dy=(pos.y()-_uiDragPositionLast.y())/s.height();
			_uiDragPositionLast=pos;
			
			if(_uiDragMode==DragMode::Rotate)
				_navHandler.rotateView(dx, dy, 0);
			else if(_uiDragMode==DragMode::Move)
				_navHandler.moveView(dx, dy, 0);
			else if(_uiDragMode==DragMode::Move2)
				_navHandler.moveView(0, 0, -dy);
			
			updateByNavHandler();
		}
	}
	void QtViewer::wheelEvent(QWheelEvent * event_)
	{
		if(!_initialized)
			return;
		
		double delta=event_->angleDelta().y()/8.0/360.0;
		_navHandler.rotateView(0, 0, -delta);
		updateByNavHandler();
	}
	

	void QtViewer::updateByNavHandler()
	{
		auto cam=_navHandler.camera();
		_occView->Camera()->SetEye(gp_Pnt(cam.x(), cam.y(), cam.z()));
		
		auto t=_navHandler.target();
		_occView->Camera()->SetCenter(gp_Pnt(t.x(), t.y(), t.z()));
		
		auto up=_navHandler.up();
		_occView->Camera()->SetUp(gp_Dir(up.x(), up.y(), up.z()));
		
		_occView->ZFitAll();
		_occView->Redraw();
	}
	
	
}
