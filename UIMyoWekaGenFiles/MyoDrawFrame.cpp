#include "MyoDrawFrame.h"
#include "MyoDialog.h"
#include <QQuaternion>
#include <QVector2D>
#include <QVector4D>
MyoDrawFrame::MyoDrawFrame(QWidget *parent)
:QGLWidget(parent)
{

}
MyoDrawFrame::~MyoDrawFrame()
{
    mTimer.stop();
    deleteGeometry();
}

void MyoDrawFrame::setMyoManager(MyoManager* myoManager)
{
    mMyoManager = myoManager;
}

void MyoDrawFrame::setDefaultRotation(const myo::Quaternion< float >& rotation)
{
    mDefaultRot = myo::Quaternion< double >(rotation.x(),
                                            rotation.y(),
                                            rotation.z(),
                                            rotation.w());

}

void MyoDrawFrame::setDefaultRotation(const myo::Quaternion< double >& rotation)
{
    mDefaultRot = rotation;
}

void MyoDrawFrame::initializeGL()
{
    //init opengl calls
    initializeGLFunctions();

    //qt backgound
    qglClearColor(QColor{"#302F2F"});


    #define GLSL(x) #x
    //init shaders
    mProgramShader.addShaderFromSourceCode(QGLShader::Vertex,
    GLSL(
      attribute vec4  a_position;
      uniform   mat4  mvp_matrix;
      varying   float z_position;
      void main()
      {
          // Calculate vertex position in screen space
          gl_Position = mvp_matrix * a_position;
          //pass to fragment z value of the model
          z_position  = a_position.z;
      }
    ));
    mProgramShader.addShaderFromSourceCode(QGLShader::Fragment,
    GLSL(
      varying   float z_position;
      void main()
      {
         #if 1
         float  factor = (z_position+1.0) * 0.5;
         #else
         float  factor = z_position<= 0.5 ? 1.0 : 0.0 ;
         #endif
         float ifactor = 1.0 - factor;
         vec3 color   = vec3(factor,0.25,ifactor);
         gl_FragColor = vec4(color ,1.0);
      }
    ));
    // Linking shader pipeline
    assert (mProgramShader.link());
    // Binding shader pipeline for use
    assert (mProgramShader.bind());

    // set opengl state
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    //init geometry
    initGeometry();

    //start loop
    mTimer.start(16, this);
}

void MyoDrawFrame::resizeGL(int w, int h)
{
    // Set OpenGL viewport
    glViewport(0, 0, w, h);

    // Calculate aspect ratio
    qreal aspect = (qreal)w / ((qreal)h?h:1);

    // Reset projection
    mProjection.setToIdentity();

    // Set perspective projection
    mProjection.perspective(45.0, aspect, 3.0, 7.0);
}

void MyoDrawFrame::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 matrix;

    //applay translation
    matrix.translate(0.0, 0.0, -5.0);

    //applay rotation
    if(mMyoManager)
    {
        auto row = mMyoManager->getCurrentRow();
        //get quad
        auto qmyo = row.getQuaternion().normalized();
        //get eulers
        auto eulers = row.getEulerAngles();
        //to quad
        QQuaternion quad(QVector4D{
            qmyo.z(),
            qmyo.y(),
            qmyo.w(),
            qmyo.x(),
        });
        matrix.rotate(quad);
        //get dialog
        auto dialog =  dynamic_cast<MyoDialog*>(parent()->parent());
        //show inputs
        if(dialog)
        {
            dialog->showEulers
            (
                (eulers.mPitch * 180.0) / M_PI,
                (eulers.mYaw   * 180.0) / M_PI,
                (eulers.mRoll  * 180.0) / M_PI
            );
            dialog->showEmg(row.getEmg());
        }
    }
    //default rotation
    else
    {
        //to quad
        QQuaternion quad(QVector4D{
            (float)mDefaultRot.z(),
            (float)mDefaultRot.y(),
            (float)mDefaultRot.w(),
            (float)mDefaultRot.x(),
        });
        matrix.rotate(quad);
    }

    // Set modelview-projection matrix
    mProgramShader.setUniformValue("mvp_matrix", mProjection * matrix);

    // Draw geometry
    drawGeometry();
}


//vertex structure
struct VertexData
{
    QVector3D position;
};

void MyoDrawFrame::initGeometry()
{
    // Generate VBO
    glGenBuffers(1, &mVBuffer);
    // Generate IBO
    glGenBuffers(1, &mIBuffer);
    //vertex list
    VertexData vertices[] =
    {
        // Vertex data for face 0
        QVector3D(-0.25, -0.25,  1.0),  // v0
        QVector3D( 0.25, -0.25,  1.0),  // v1
        QVector3D(-0.25,  0.25,  1.0),  // v2
        QVector3D( 0.25,  0.25,  1.0),  // v3
        QVector3D( 0.0,   0.0,  -1.0)   // v4

    };

    //index list
    GLushort indices[] =
    {
        //front
        0,1,4,
        2,3,4,
        0,2,4,
        1,3,4,
        //back
        0,1,3,
        0,2,3
    };


    // Transfer vertex data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, mVBuffer);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(VertexData), vertices, GL_STATIC_DRAW);

    // Transfer index data to IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(GLushort), indices, GL_STATIC_DRAW);
}

void MyoDrawFrame::drawGeometry()
{
    //enable vbo and ibo
    glBindBuffer(GL_ARRAY_BUFFER, mVBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBuffer);

    //id of attribute "position"
    int vertexLocation = mProgramShader.attributeLocation("a_position");
    mProgramShader.enableAttributeArray(vertexLocation);
    //set buffer info
    glVertexAttribPointer(vertexLocation, 3,
                          GL_FLOAT,
                          GL_FALSE, sizeof(VertexData),
                          (const void *)0);

    // Draw geometry
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);

}
void MyoDrawFrame::deleteGeometry()
{
    if(context())
    {
        context()->makeCurrent();
        // delete VBO
        if(mVBuffer) glDeleteBuffers(1, &mVBuffer);
        // delete IBO
        if(mVBuffer) glDeleteBuffers(1, &mIBuffer);
    }
}


void MyoDrawFrame::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    updateGL();
}
