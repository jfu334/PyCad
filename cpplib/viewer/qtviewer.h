#pragma once
#include <QtOpenGL/QGLWidget>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>

#include "viewobject.h"
#include "navigationHandler.h"

namespace PyCadCpp::viewer
{
	class QtViewer : public QGLWidget
	{
		Q_OBJECT
		
	public:
		QtViewer();
		
		void init();
		
		// objects to display
		void addObject(ViewObject* object);
		void removeObject(ViewObject* object);
		void clearObjects();
		
		void update();
		
		virtual void paintEvent(QPaintEvent* event_);
		virtual void resizeEvent(QResizeEvent* event_);
		
		virtual void mousePressEvent(QMouseEvent* event_);
		virtual void mouseReleaseEvent(QMouseEvent* event_);
		virtual void mouseMoveEvent(QMouseEvent * event_);
		virtual void wheelEvent(QWheelEvent * event_);

	private:
		bool _initialized;
		
		Handle(V3d_Viewer) _occViewer;
		Handle(V3d_View) _occView;
		Handle(AIS_InteractiveContext) _occContext;
		
		enum class DragMode {Inactive, Rotate, Move, Move2};
		DragMode _uiDragMode;
		QPointF _uiDragPositionLast;
		
		NavigationHandler _navHandler;
		
		void updateByNavHandler();
	};
}
 
