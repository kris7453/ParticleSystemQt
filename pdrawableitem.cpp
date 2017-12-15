#include "pdrawableitem.h"

namespace PSystemAPI
{
    QOpenGLFunctions_3_3_Core *pDrawableItem::oGLFunct;
    GLuint pDrawableItem::bilboard;

    void PSystemAPI::pDrawableItem::initialize(QOpenGLFunctions_3_3_Core *glFunct)
    {
        oGLFunct = glFunct;

        GLfloat box[] = {   -0.5f, -0.5f, 0.0f, 0.0f,
                             0.5f, -0.5f, 1.0f, 0.0f,
                            -0.5f,  0.5f, 0.0f, 1.0f,
                             0.5f,  0.5f, 1.0f, 1.0f};

        oGLFunct->glGenBuffers(1, &bilboard);

        oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, bilboard);
        oGLFunct->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*16, box, GL_STATIC_DRAW);
    }
}
