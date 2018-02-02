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
            pDrawableItem( QString *resourceImagePath, QString name);
            static void initialize(QOpenGLFunctions_3_3_Core *glFunct);
            virtual void draw() = 0;

            void setPosition( QVector2D position);
            void translatePosition( QVector2D position);
            void saveLastPosition();
            void restoreLastPosition();

            bool isVisible();
            bool changeVisibility();
            bool isParticleSystem();

            bool operator==(const pDrawableItem& item);

            void setName(const QString &value);
            void setAngle( short angle);

            QString getName();
            int getAngle();

        protected:
            static QOpenGLFunctions_3_3_Core *oGLFunct;
            static GLuint bilboard;

        //private:
            QVector2D position;
            QVector2D lastPosition;
            short angle;

            GLuint vao;
            GLuint propertiesBuffer;

            QOpenGLTexture *texture;
            QString name;

            bool visibility;
            bool particleSystem;
    };
}

#endif // PDRAWABLEITEM_H
