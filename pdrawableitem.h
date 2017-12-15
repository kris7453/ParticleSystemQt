#ifndef PDRAWABLEITEM_H
#define PDRAWABLEITEM_H

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <GL/gl.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>

using namespace std;

namespace PSystemAPI
{
    class pDrawableItem
    {
        public:
            static void initialize(QOpenGLFunctions_3_3_Core *glFunct);
            virtual void draw() = 0;

        protected:
            static QOpenGLFunctions_3_3_Core *oGLFunct;
            static GLuint bilboard;

        //private:
            QVector2D position;

            GLuint vao;
            GLuint propertiesBuffer;
    };
}

#endif // PDRAWABLEITEM_H
