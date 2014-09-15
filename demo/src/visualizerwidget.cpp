#include "visualizerwidget.h"
#include <cmath>
#include <QMessageBox>

VisualizerWidget::VisualizerWidget(EarProcessor *earProcessor, QWidget *parent)
    : QGLWidget(parent) {
    m_earProcessor = earProcessor;

    for(int i = 0; i <= 40; i ++) {
        m_logarithmicFrequencies[i] = (22050 * pow(10, (float)i / 16 + 7.5) / pow(10, 10.0));
        m_centralControlPoints[i] = (int)m_logarithmicFrequencies[i] * 2048 / 22500;
    }

    m_zoom = 1.0;
    m_position[0] = 0.0;
    m_position[1] = 0.0;

    m_zoomSemaphore = new QSemaphore(1);
    m_positionSemaphore = new QSemaphore(1);

    m_leftMouseButtonDown = false;

    setMouseTracking(true);
}

void VisualizerWidget::initializeGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineStipple(1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
}

void VisualizerWidget::resizeGL(int w, int h) {
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void VisualizerWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.5f, 0.5f, -5.0f);
    glScalef(0.8, 0.8, 0.8);

    {
        SemaphoreLocker locker(m_zoomSemaphore);
        glScalef(m_zoom, m_zoom, m_zoom);
    }

    {
        SemaphoreLocker locker(m_positionSemaphore);
        glTranslatef(m_position[0], m_position[1], 0.0);
    }

    glTranslatef(-0.5f, -0.5f, -5.0f);

    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(0.0, 0.5);
        glVertex2f(1.0, 0.5);

        glColor3f(0.1, 0.1, 0.1);
        glVertex2f(1.0, 0.0);
        glVertex2f(0.0, 0.0);
    glEnd();

    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINES);
        for(int columns = 0; columns <= 40; columns ++) {
            if(columns % 4 == 0) {
                glColor3f(0.3, 0.3, 0.5);
            } else {
                glColor3f(0.3, 0.3, 0.3);
            }
            glVertex2f(1.0 * (float)columns / 40.0, (columns % 4 == 0) ? -0.03 : 0.0);
            glVertex2f(1.0 * (float)columns / 40.0, 1.0);
        }
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    for(int columns = 0; columns <= 10; columns ++) {
        renderText(1.0 * (float)columns / 10.0 + 0.01, -0.05, 0.0,
                   QString("%1 Hz").arg((int)m_logarithmicFrequencies[columns * 4]));
    }


    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINES);
        for(int rows = 0; rows <= 40; rows ++) {
            if(rows % 2 == 0) {
                glColor3f(0.3, 0.3, 0.5);
            } else {
                glColor3f(0.3, 0.3, 0.3);
            }

            glVertex2f((rows % 2 == 0) ? -0.02 : 0.0, 1.0 * (float)rows / 40.0);
            glVertex2f(1.0, 1.0 * (float)rows / 40.0);
        }
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    for(int rows = 0; rows <= 20; rows ++) {
        renderText(-0.075, 1.0 - 1.0 * (float)rows / 20.0, 0.0, QString("-%1 dB").arg(rows));
    }

    DigitalEqualizer *leftEqualizer = m_earProcessor->leftEqualizer();
    DigitalEqualizer *rightEqualizer = m_earProcessor->rightEqualizer();
    double *leftControls;
    double *rightControls;
    float boxSize = 0.008;

    // Since we have less controls than control points in the equalizers, we have to take
    // the intermediate values.
    double leftAverage[41];
    double rightAverage[41];

    // Lock the controls, we are going to read and we don't want anyone to write to them
    // while we are reading.
    leftEqualizer->acquireControls();
    rightEqualizer->acquireControls();
    leftControls = leftEqualizer->controls();
    rightControls = rightEqualizer->controls();

    for(int i = 0; i <= 40; i++) {
        leftAverage[i] = 0;
        rightAverage[i] = 0;

        // In case we are at the first control, we take all values from the first to the mid of the
        // the first and second control into account.
        if(i == 0) {
            for(int j = 1; j < (m_centralControlPoints[i] + m_centralControlPoints[i + 1]) / 2; j++) {
                leftAverage[i] += leftControls[j];
                rightAverage[i] += rightControls[j];
            }
            leftAverage[i] /= (double)(m_centralControlPoints[i] + m_centralControlPoints[i + 1]) / 2 - 1.5;
            rightAverage[i] /= (double)(m_centralControlPoints[i] + m_centralControlPoints[i + 1]) / 2 - 1.5;
        // Similarly, we have to iterate over all control values from the mid of the last and one before the last
        // until the last control value.
        } else if(i == 40) {
            for(int j = (m_centralControlPoints[i] + m_centralControlPoints[i - 1]) / 2; j < 2048; j++) {
                leftAverage[i] += leftControls[j];
                rightAverage[i] += rightControls[j];
            }
            leftAverage[i] /= (double)(2048 - (m_centralControlPoints[i] + m_centralControlPoints[i - 1]) / 2);
            rightAverage[i] /= (double)(2048 - (m_centralControlPoints[i] + m_centralControlPoints[i - 1]) / 2);
        // For all other control values, iterate from mid to mid, ie. take the mid of the current and previous
        // control value and the current and next one.
        } else {
            for(int j = (m_centralControlPoints[i] + m_centralControlPoints[i - 1]) / 2;
                    j < (m_centralControlPoints[i] + m_centralControlPoints[i + 1]) / 2; j++) {
                leftAverage[i] += leftControls[j];
                rightAverage[i] += rightControls[j];
            }
            leftAverage[i] /= (double)((m_centralControlPoints[i] + m_centralControlPoints[i + 1]) / 2 - (m_centralControlPoints[i] + m_centralControlPoints[i - 1]) / 2);
            rightAverage[i] /= (double)((m_centralControlPoints[i] + m_centralControlPoints[i + 1]) / 2 - (m_centralControlPoints[i] + m_centralControlPoints[i - 1]) / 2);
        }
    }

    leftEqualizer->releaseControls();
    rightEqualizer->releaseControls();


    GLfloat points[41][3];
    for(int i = 0; i <= 40; i++) {
        float value = rightAverage[i];
        points[i][0] = (float)i / 40.0;
        points[i][1] = log(value) / log(10.0) * 10.0 / 20.0 + 1.0;
        points[i][2] = 0.0;
    }
    glColor4f(1.0, 0.0, 0.0, 0.8);
    drawBezier(&points[0][0], 21);
    drawBezier(&points[20][0], 20);

    glColor4f(1.0, 0.0, 0.0, 1.0);
    renderText(0.5, 1.05, 0.0, "Right Channel");
    glBegin(GL_QUADS);
        for(int i = 0; i <= 40; i++) {
            float value = rightAverage[i];
            float xMidValue = (float)i / 40.0;
            float yMidValue = log(value) / log(10.0) * 10.0 / 20.0 + 1.0;
            glVertex2f(xMidValue - boxSize, yMidValue - boxSize / 3.0);
            glVertex2f(xMidValue + boxSize, yMidValue - boxSize / 3.0);
            glVertex2f(xMidValue + boxSize, yMidValue + boxSize / 3.0);
            glVertex2f(xMidValue - boxSize, yMidValue + boxSize / 3.0);
        }
    glEnd();

    for(int i = 0; i <= 40; i++) {
        float value = leftAverage[i];
        points[i][0] = (float)i / 40.0;
        points[i][1] = log(value) / log(10.0) * 10.0 / 20.0 + 1.0;
        points[i][2] = 0.0;
    }
    glColor4f(1.0, 1.0, 1.0, 0.8);
    drawBezier(&points[0][0], 21);
    drawBezier(&points[20][0], 20);

    glColor4f(1.0, 1.0, 1.0, 1.0);
    renderText(0.3, 1.05, 0.0, "Left Channel");
    glBegin(GL_QUADS);
        for(int i = 0; i <= 40; i++) {
            float value = leftAverage[i];
            float xMidValue = (float)i / 40.0;
            float yMidValue = log(value) / log(10.0) * 10.0 / 20.0 + 1.0;
            glVertex2f(xMidValue - boxSize, yMidValue - boxSize / 3.0);
            glVertex2f(xMidValue + boxSize, yMidValue - boxSize / 3.0);
            glVertex2f(xMidValue + boxSize, yMidValue + boxSize / 3.0);
            glVertex2f(xMidValue - boxSize, yMidValue + boxSize / 3.0);
        }
    glEnd();
}

void VisualizerWidget::wheelEvent(QWheelEvent *wheelEvent) {
    int delta = wheelEvent->delta();
    SemaphoreLocker locker(m_zoomSemaphore);
    m_zoom += (double)delta / 5000;
    if(m_zoom < 1.0) {
        m_zoom = 1.0;
        SemaphoreLocker plocker(m_positionSemaphore);
        m_position[0] *= 0.9;
        m_position[1] *= 0.9;
    }
}

void VisualizerWidget::mousePressEvent(QMouseEvent *mouseEvent) {
    if(mouseEvent->button() == Qt::LeftButton) {
        m_leftMouseButtonDown = true;
    }
}

void VisualizerWidget::mouseReleaseEvent(QMouseEvent *mouseEvent) {
    if(mouseEvent->button() == Qt::LeftButton) {
        m_leftMouseButtonDown = false;
    }
}


void VisualizerWidget::mouseMoveEvent(QMouseEvent *mouseEvent) {
    if(m_leftMouseButtonDown) {
        double dx = mouseEvent->x() - m_lastMousePosition[0];
        double dy = mouseEvent->y() - m_lastMousePosition[1];
        {
            SemaphoreLocker locker(m_positionSemaphore);
            m_position[0] += dx / 1000;
            m_position[1] -= dy / 1000;
        }
    }
    m_lastMousePosition[0] = mouseEvent->x();
    m_lastMousePosition[1] = mouseEvent->y();
}

void VisualizerWidget::drawBezier(GLfloat *controlPoints, int n) {
    glDisable(GL_LINE_STIPPLE);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, n, controlPoints);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);
       for(int i = 0; i <= n * 4; i++)
          glEvalCoord1f((GLfloat)i / ((GLfloat)n * 4.0));
    glEnd();
    glDisable(GL_MAP1_VERTEX_3);
    glEnable(GL_LINE_STIPPLE);
}
