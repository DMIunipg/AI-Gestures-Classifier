#ifndef MYODRAWFRAME_H
#define MYODRAWFRAME_H

#include <QGLWidget>
#include <QGLFunctions>
#include <QBasicTimer>
#include <QMatrix4x4>
#include <QtOpenGL/QGLShaderProgram>
#include <QtOpenGL/QGLBuffer>
#include "MyoManager.h"

/*!
 * \brief The MyoDrawFrame class
 */
class MyoDrawFrame : public QGLWidget, public QGLFunctions
{

    Q_OBJECT

public:

    /*!
     * \brief MyoDrawFrame
     * \param parent
     */
    MyoDrawFrame(QWidget* parent);

    /*!
     * \brief ~MyoDrawFrame
     */
    virtual ~MyoDrawFrame();

    /*!
     * \brief setMyoManager
     * \param manager
     */
    void setMyoManager(MyoManager* manager);

    /*!
     * \brief getMyoManager
     * \param manager
     */
    MyoManager* getMyoManager() const;

    /*!
     * \brief setDefaultRotation
     * \param rotation
     */
    void setDefaultRotation(const myo::Quaternion< float >& rotation);

    /*!
     * \brief setDefaultRotation
     * \param rotation
     */
    void setDefaultRotation(const myo::Quaternion< double >& rotation);

protected:

    /*!
     * \brief timerEvent
     * \param e
     */
    void timerEvent(QTimerEvent *e);

    /*!
     * \brief initializeGL
     */
    void initializeGL();

    /*!
     * \brief resizeGL
     * \param w
     * \param h
     */
    void resizeGL(int w, int h);

    /*!
     * \brief paintGL
     */
    void paintGL();

private:

    QBasicTimer mTimer;                  //! timer object (where is scheduled the update)
    MyoManager* mMyoManager { nullptr }; //! myo manager pointer
    GLuint mVBuffer { 0 };               //! OpenGL vertex buffer
    GLuint mIBuffer { 0 };               //! OpenGL index buffer

    /*!
     * \brief initGeometry
     */
    void initGeometry();

    /*!
     * \brief drawGeometry
     */
    void drawGeometry();

    /*!
     * \brief deleteGeometry
     */
    void deleteGeometry();

    myo::Quaternion< double > mDefaultRot;  //! default rotation
    QGLShaderProgram mProgramShader;        //! OpenGL shader program
    QMatrix4x4 mProjection;                 //! projection matrix
};

#endif // MYODRAWFRAME_H
