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

            void setPosition( QVector2D position);
            void translatePosition( QVector2D position);
            void saveLastPosition();
            void restoreLastPosition();

        protected:
            static QOpenGLFunctions_3_3_Core *oGLFunct;
            static GLuint bilboard;

        //private:
            QVector2D position;
            QVector2D lastPosition;

            GLuint vao;
            GLuint propertiesBuffer;
    };
}

#endif // PDRAWABLEITEM_H
