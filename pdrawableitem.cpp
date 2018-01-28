#include "pdrawableitem.h"

namespace PSystemAPI
{
    QOpenGLFunctions_3_3_Core *pDrawableItem::oGLFunct;
    GLuint pDrawableItem::bilboard;

    pDrawableItem::pDrawableItem(QString *resourceImagePath, QString name) : name(name)
    {
        texture = new QOpenGLTexture(QImage((resourceImagePath != nullptr) ? *resourceImagePath : QString(":/particles/star.png")).mirrored());
        texture->generateMipMaps();
        texture->setMinificationFilter(QOpenGLTexture::Nearest);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);

        visibility = true;
    }

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

    void pDrawableItem::setPosition(QVector2D position)
    {
        this->position = position;
    }

    void pDrawableItem::translatePosition(QVector2D position)
    {
        //qDebug() << "position " << position.x() << " "  << position.y();
        position.setY(-position.y());
        this->position = lastPosition + position;
    }

    void pDrawableItem::saveLastPosition()
    {
        lastPosition = position;
    }

    void pDrawableItem::restoreLastPosition()
    {
        position = lastPosition;
    }

    bool pDrawableItem::isVisible()
    {
        return visibility;
    }

    bool pDrawableItem::changeVisibility()
    {
        return visibility ^= true;
    }

    bool pDrawableItem::operator==(const pDrawableItem &item)
    {
        return reinterpret_cast<int>(this) == reinterpret_cast<int>(&item);
    }
    
    QString pDrawableItem::getName()
    {
        return name;
    }
    
    void pDrawableItem::setName(const QString &value)
    {
        name = value;
    }
}
