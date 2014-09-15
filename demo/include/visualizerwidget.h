#ifndef VISUALIZERWIDGET_H
#define VISUALIZERWIDGET_H

#include <QGLWidget>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QSemaphore>
#include "earprocessor.h"


/**
  * @class VisualizerWidget
  * @brief View counterpart for the EAR processor.
  * This class is the view counterpart for the EAR processor,
  * which acts as the model class.
  */
class VisualizerWidget : public QGLWidget {
    Q_OBJECT
public:
    /**
      * Constructs a new view that will be attached to the given EAR processor.
      * @param earProcessor EAR processor to which this view will be attached.
      * @param parent Parent widget for the Qt framework.
      */
    VisualizerWidget(EarProcessor *earProcessor, QWidget *parent = 0);

protected:
    /** Reimplemented from QGLWidget. */
    void initializeGL();

    /**
      * Reimplemented from QGLWidget.
      * @param w Width of the new OpenGL viewport.
      * @param h Height of the new OpenGL viewport.
      */
    void resizeGL(int w, int h);

    /**
      * Reimplemented from QGLWidget.
      */
    void paintGL();

    /**
      * Reimplemented from QGLWidget.
      * @param wheelEvent Wheel event.
      */
    void wheelEvent(QWheelEvent *wheelEvent);

    /**
      * Reimplemented from QGLWidget.
      * @param mouseEvent Mouse event.
      */
    void mousePressEvent(QMouseEvent *mouseEvent);

    /**
      * Reimplemented from QGLWidget.
      * @param mouseEvent Mouse event.
      */
    void mouseReleaseEvent(QMouseEvent *mouseEvent);

    /**
      * Reimplemented from QGLWidget
      * @param mouseEvent Mouse event.
      */
    void mouseMoveEvent(QMouseEvent *mouseEvent);

private:
    /**
      * Draws a bezier line through all given control points.
      * @param controlPoints Array pointer to the first element of control points.
      * @param n Number of control points to use.
      */
    void drawBezier(GLfloat *controlPoints, int n);

    /** Model class this view is attached to. */
    EarProcessor *m_earProcessor;

    /** Helper array to calculate frequency in logarithmic scale. */
    double m_logarithmicFrequencies[41];

    /** Helper array to determine which control points are centered at a logarithmic scale. */
    int m_centralControlPoints[41];

    /** Semaphore for multithreaded access to the zoom state. */
    QSemaphore *m_zoomSemaphore;

    /** Semaphore for multithreaded acces to the position state. */
    QSemaphore *m_positionSemaphore;

    /** Zoom state. */
    double m_zoom;

    /** Position state. */
    double m_position[2];

    /** Left mouse button down state. */
    bool m_leftMouseButtonDown;

    /** Last mouse position for stored for dragging. */
    double m_lastMousePosition[2];
};

#endif // VISUALIZERWIDGET_H
