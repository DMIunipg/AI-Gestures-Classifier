#ifndef MYODRAWFRAME_H
#define MYODRAWFRAME_H

#include <QGLWidget>
#include <QGLFunctions>
#include <QBasicTimer>
#include <QMatrix4x4>
#include <QtOpenGL/QGLShaderProgram>
#include <QtOpenGL/QGLBuffer>
#include "MyoManager.h"

class MyoDrawFrame : public QGLWidget, public QGLFunctions
{

    Q_OBJECT

public:

    MyoDrawFrame(QWidget* parent);
    virtual ~MyoDrawFrame();
    void setMyoManager(MyoManager* manager);
    void setDefaultRotation(const myo::Quaternion< float >& rotation);
    void setDefaultRotation(const myo::Quaternion< double >& rotation);

protected:

    void timerEvent(QTimerEvent *e);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:

    //loop scheduler
    QBasicTimer mTimer;
    //myo ref
    MyoManager* mMyoManager {nullptr};
    //buffers
    GLuint mVBuffer { 0 };
    GLuint mIBuffer { 0 };
    void initGeometry();
    void drawGeometry();
    void deleteGeometry();
    //default rotation
    myo::Quaternion< double > mDefaultRot;
    //shader
    QGLShaderProgram          mProgramShader;
    //projection
    QMatrix4x4 mProjection;
};

#endif // MYODRAWFRAME_H
