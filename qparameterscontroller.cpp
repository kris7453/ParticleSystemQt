#include "qparameterscontroller.h"

QparametersController::QparametersController(QToolBox *emiterMode, QWidget *drawableWidget,
                                             QWidget *gravityWidget, QWidget *radialWidget, QWidget *outlookWidget,
                                             QLabel *texturePath, QPushButton *textureButton, QPushButton *addSystemButton,
                                             QmainGLWidget *mainGLWidget, QObject *parent) : QObject(parent), glWidget(mainGLWidget)
{

    // Reset position button

    resetPositionButton = new QPushButton(tr("Resetuj pozycję"), drawableWidget);

    connect(resetPositionButton, &QPushButton::clicked, [this](){ controller->resetSystemPosition(); });


    // Colors widgets

    colorWidget= new std::pair<QPushButton*,rangeParameterWidget*>*[4]
    {
        new std::pair<QPushButton*,rangeParameterWidget*>(
                new QPushButton("Kolor startowy", outlookWidget),
                new rangeParameterWidget("Kanał alfa", 0,255,range::startColorAlpha, outlookWidget)),
        new std::pair<QPushButton*,rangeParameterWidget*>(
                new QPushButton("Kolor startowy rozbierzność", outlookWidget),
                new rangeParameterWidget("Kanał alfa", 0,255,range::startColorAlpha, outlookWidget)),
        new std::pair<QPushButton*,rangeParameterWidget*>(
                new QPushButton("Kolor końcowy", outlookWidget),
                new rangeParameterWidget("Kanał alfa", 0,255,range::endColorAlpha, outlookWidget)),
        new std::pair<QPushButton*,rangeParameterWidget*>(
                new QPushButton("Kolor końcowy rozbierznoość", outlookWidget),
                new rangeParameterWidget("Kanał alfa", 0,255,range::endColorAlpha, outlookWidget))
    };

    for (int i = 0; i < 4; i++)
    {
        colorWidget[i]->first->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        colorWidget[i]->first->setMinimumHeight(30);
    }

    connect(colorWidget[0]->first, &QPushButton::clicked, [this]()
    {
        QColor color = QColorDialog::getColor(controller->getActiveSystem()->getParticleStartColor());
        controller->setParticleStartColor(color);
        color.setAlpha(static_cast<int>(colorWidget[0]->second->getValue()));
        colorButtonSetColor(colorWidget[0],"Kolor startowy : ",color);
    });

    connect(colorWidget[1]->first, &QPushButton::clicked, [this]()
    {
        QColor color = QColorDialog::getColor(controller->getActiveSystem()->getParticleStartColorVariance());
        controller->setParticleStartColorVariance(color);
        color.setAlpha(static_cast<int>(colorWidget[1]->second->getValue()));
        colorButtonSetColor(colorWidget[1],"Kolor startowy rozbierzność : ",color);
    });

    connect(colorWidget[2]->first, &QPushButton::clicked, [this]()
    {
        QColor color = QColorDialog::getColor(controller->getActiveSystem()->getParticleEndColor());
        controller->setParticleEndColor(color);
        color.setAlpha(static_cast<int>(colorWidget[2]->second->getValue()));
        colorButtonSetColor(colorWidget[2],"Kolor końcowy : ",color);
    });

    connect(colorWidget[3]->first, &QPushButton::clicked, [this]()
    {
        QColor color = QColorDialog::getColor(controller->getActiveSystem()->getParticleEndColorVariance());
        controller->setParticleEndColorVariance(color);
        color.setAlpha(static_cast<int>(colorWidget[3]->second->getValue()));
        colorButtonSetColor(colorWidget[3],"Kolor końcowy rozbierzność : ",color);
    });

    connect(colorWidget[0]->second, &rangeParameterWidget::valueChanged, this, &QparametersController::setMainValue);
    connect(colorWidget[1]->second, &rangeParameterWidget::valueChanged, this, &QparametersController::setVarianceValue);
    connect(colorWidget[2]->second, &rangeParameterWidget::valueChanged, this, &QparametersController::setMainValue);
    connect(colorWidget[3]->second, &rangeParameterWidget::valueChanged, this, &QparametersController::setVarianceValue);

    // Other controllers

    int controllersCount = 15       ;
    values = new rangeParameterVarianceWidget *[controllersCount]
    {
        new rangeParameterVarianceWidget("Kąt + rozbierzność", "", -180, 180, "", 0, 180, range::angle, drawableWidget),
        new rangeParameterVarianceWidget("Czas życia cząstki(s) + rozbierzność", "", 0, 10, "", 0, 5, range::particleLife, drawableWidget),

        new rangeParameterVarianceWidget("Rozbierzność pozycji", "X", 0, 2000, "Y", 0, 2000, range::position, gravityWidget),
        new rangeParameterVarianceWidget("Prędkość + rozbierzność", "", -2000, 2000, "", 0, 1000, range::speed, gravityWidget),
        new rangeParameterVarianceWidget("Grawitacja X + rozbierzność", "", -1000, 1000, "", 0, 1000, range::gravityX, gravityWidget),
        new rangeParameterVarianceWidget("Grawitacja Y + rozbierzność", "", -1000, 1000, "", 0, 1000, range::gravityY, gravityWidget),
        new rangeParameterVarianceWidget("Przyspieszenie promieniowe + rozbierzność", "", -1000, 1000, "", 0, 1000, range::radialAccValue, gravityWidget),
        new rangeParameterVarianceWidget("Przyspieszenie styczne + rozbierzność", "", -1000, 1000, "", 0, 1000, range::tangentialAccValue, gravityWidget),

        new rangeParameterVarianceWidget("Promień startowy + rozbierzność", "", -1000, 1000, "", 0, 1000, range::startRadius, radialWidget),
        new rangeParameterVarianceWidget("Promień końcowy + rozbierzność", "", -1000, 1000, "", 0, 1000, range::endRadius, radialWidget),
        new rangeParameterVarianceWidget("Obrotów/s (stopnie) + rozbierzność", "", -1000, 1000, "", 0, 1000, range::rotatePerSec, radialWidget),

        new rangeParameterVarianceWidget("Rozmiar startowy + rozbierzność", "", 0, 200, "", 0, 200, range::startSize, outlookWidget),
        new rangeParameterVarianceWidget("Rozmiar końcowy + rozbierzność", "", 0, 200, "", 0, 200, range::endSize, outlookWidget),
        new rangeParameterVarianceWidget("Obrót startowy (stopnie) + rozbierzność", "", -1000, 1000, "", 0, 1000, range::startSpin, outlookWidget),
        new rangeParameterVarianceWidget("Obrót końcowy (stopnie) + rozbierzność", "", -1000, 1000, "", 0, 1000, range::endSpin, outlookWidget),
    };

    this->emiterMode = emiterMode;
    connect(emiterMode, &QToolBox::currentChanged, [this](int val){controller->setMode(val);});

    // Settings controllers in window

    QLayout *layout;

    layout = drawableWidget->layout();
    layout->addWidget(resetPositionButton);
    layout->addWidget(values[range::angle]);
    layout->addWidget(values[range::particleLife]);

    layout = gravityWidget->layout();
    layout->addWidget(values[range::position]); // main = x, variance = y
    layout->addWidget(values[range::speed]);
    layout->addWidget(values[range::gravityX]);
    layout->addWidget(values[range::gravityY]);
    layout->addWidget(values[range::radialAccValue]);
    layout->addWidget(values[range::tangentialAccValue]);
    layout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    layout = radialWidget->layout();
    layout->addWidget(values[range::startRadius]);
    layout->addWidget(values[range::endRadius]);
    layout->addWidget(values[range::rotatePerSec]);
    layout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    layout = outlookWidget->layout();
    layout->setSpacing(4);
    layout->addWidget(colorWidget[0]->first);
    layout->addWidget(colorWidget[0]->second);
    layout->addWidget(colorWidget[1]->first);
    layout->addWidget(colorWidget[1]->second);
    layout->addWidget(colorWidget[2]->first);
    layout->addWidget(colorWidget[2]->second);
    layout->addWidget(colorWidget[3]->first);
    layout->addWidget(colorWidget[3]->second);
    layout->addWidget(values[range::startSize]);
    layout->addWidget(values[range::endSize]);
    layout->addWidget(values[range::startSpin]);
    layout->addWidget(values[range::endSpin]);
    layout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    for (int i = 0; i < controllersCount; i++)
    {
        connect(values[i], &rangeParameterVarianceWidget::firstValueChanged, this, &QparametersController::setMainValue);
        connect(values[i], &rangeParameterVarianceWidget::secondValueChanged, this, &QparametersController::setVarianceValue);
    }

    this->texturePath = texturePath;

    connect( textureButton, &QPushButton::clicked, [this](){
        QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Wygląd cząstki"), QString(), tr("Image(*.png)") );
        if ( !filePath.isEmpty() )
        {
            this->texturePath->setText(filePath);
            controller->setActiveItemTexture(filePath);
        }
    });

    connect(addSystemButton, &QPushButton::clicked, [this](){
        bool ok;
        QString systemName = QInputDialog::getText(nullptr, tr("Nazwa systemu"), tr("Podaj nazwę systemu"), QLineEdit::Normal, QString(), &ok);

        if (ok)
        {
            QString resourcePath = QFileDialog::getOpenFileName(nullptr, tr("Wygląd cząstki"), QString(), tr("Image (*.png)") );

            glWidget->makeCurrent();
            controller->addParticleSystem(!resourcePath.isEmpty() ? &resourcePath : nullptr, systemName);
        }
    });
}

// ------------------------------------------------ constructor end ---------------------------------------------------------

void QparametersController::setDrawableController(PSystemAPI::pDrawItemsController *controller)
{
    this->controller = controller;
}

// Functions called by pDrawableItemsController class object to set widgets value from active item when changed

void QparametersController::changeValues(PSystemAPI::pDrawableItem *item)
{
    values[range::angle]->setFirstValue(item->getAngle());
    //qDebug() << "Set angle to " << item->getAngle();
}

void QparametersController::changeValues(PSystemAPI::pParticleSystem *system)
{
    changeValues(reinterpret_cast<PSystemAPI::pDrawableItem*>(system));

    emiterMode->setCurrentIndex(system->getSystemMode());

    values[range::angle]->setSecondValue(system->getAngleVariance());
    values[range::particleLife]->setFirstValue(system->getParticleLife());
    values[range::particleLife]->setSecondValue(system->getParticleLifeVariance());

    values[range::position]->setFirstValue(system->getPositionVarianceX());
    values[range::position]->setSecondValue(system->getPositionVarianceY());
    values[range::speed]->setFirstValue(system->getSpeed());
    values[range::speed]->setSecondValue(system->getSpeedVariance());
    values[range::gravityX]->setFirstValue(system->getGravityX());
    values[range::gravityX]->setSecondValue(system->getGravityXVariance());
    values[range::gravityY]->setFirstValue(system->getGravityY());
    values[range::gravityY]->setSecondValue(system->getGravityYVariance());
    values[range::radialAccValue]->setFirstValue(system->getRadialAccValue());
    values[range::radialAccValue]->setSecondValue(system->getRadialAccVariance());
    values[range::tangentialAccValue]->setFirstValue(system->getTangentialAccValue());
    values[range::tangentialAccValue]->setSecondValue(system->getTangentialAccVariance());

    values[range::startRadius]->setFirstValue(system->getStartRadius());
    values[range::startRadius]->setSecondValue(system->getStartRadiusVariance());
    values[range::endRadius]->setFirstValue(system->getEndRadius());
    values[range::endRadius]->setSecondValue(system->getEndRadiusVariance());
    values[range::rotatePerSec]->setFirstValue(system->getRotatePerSec());
    values[range::rotatePerSec]->setSecondValue(system->getRotatePerSecVariance());

    values[range::startSize]->setFirstValue(system->getParticleStartSize());
    values[range::startSize]->setSecondValue(system->getParticleStartSizeVariance());
    values[range::endSize]->setFirstValue(system->getParticleEndSize());
    values[range::endSize]->setSecondValue(system->getParticleEndSizeVariance());
    values[range::startSpin]->setFirstValue(system->getParticleStartSpin());
    values[range::startSpin]->setSecondValue(system->getParticleStartSpinVariance());
    values[range::endSpin]->setFirstValue(system->getParticleEndSpin());
    values[range::endSpin]->setSecondValue(system->getParticleEndSpinVariance());

    colorButtonSetColor(colorWidget[0], "Kolor startowy : ", system->getParticleStartColor());
    colorButtonSetColor(colorWidget[1], "Kolor startowy rozbierzność : ", system->getParticleStartColorVariance());
    colorButtonSetColor(colorWidget[2], "Kolor końcowy : ", system->getParticleEndColor());
    colorButtonSetColor(colorWidget[3], "Kolor końcowy rozbierzność : ", system->getParticleEndColorVariance());
}

// Redirection from widgets to parameters setters

void QparametersController::setMainValue(int itemId, double value)
{
    switch( itemId )
    {
        case range::angle: controller->setAngle(value); break;
        case range::particleLife: controller->setParticleLife(value); break;

        case range::position: controller->setPositionVarianceX(value); break;
        case range::speed: controller->setSpeed(value); break;
        case range::gravityX: controller->setGravityX(value); break;
        case range::gravityY: controller->setGravityY(value); break;
        case range::radialAccValue: controller->setRadialAccValue(value); break;
        case range::tangentialAccValue: controller->setTangentialAccValue(value); break;

        case range::startRadius: controller->setStartRadius(value); break;
        case range::endRadius: controller->setEndRadius(value); break;
        case range::rotatePerSec: controller->setRotatePerSec(value); break;

        case range::startSize: controller->setParticleStartSize(value); break;
        case range::endSize: controller->setParticleEndSize(value); break;
        case range::startSpin: controller->setParticleStartSpin(value); break;
        case range::endSpin: controller->setParticleEndSpin(value); break;

        case range::startColorAlpha: controller->setParticleStartColorAlpha(value);
                                     colorButtonSetColor(colorWidget[0], "Kolor startowy : ", controller->getParticleStartColor());
                                     break;
        case range::endColorAlpha: controller->setParticleEndColorAlpha(value);
                                   colorButtonSetColor(colorWidget[2], "Kolor końcowy : ", controller->getParticleEndColor());
                                   break;
    }

}

void QparametersController::setVarianceValue(int itemId, double value)
{
    switch( itemId )
    {
        case range::angle: controller->setAngleVariance(value); break;
        case range::particleLife: controller->setParticleLifeVariance(value); break;

        case range::position: controller->setPositionVarianceY(value); break;
        case range::speed: controller->setSpeedVariance(value); break;
        case range::gravityX: controller->setGravityXVariance(value); break;
        case range::gravityY: controller->setGravityYVariance(value); break;
        case range::radialAccValue: controller->setRadialAccVariance(value); break;
        case range::tangentialAccValue: controller->setTangentialAccVariance(value); break;

        case range::startRadius: controller->setStartRadiusVariance(value); break;
        case range::endRadius: controller->setEndRadiusVariance(value); break;
        case range::rotatePerSec: controller->setRotatePerSecVariance(value); break;

        case range::startSize: controller->setParticleStartSizeVariance(value); break;
        case range::endSize: controller->setParticleEndSizeVariance(value); break;
        case range::startSpin: controller->setParticleStartSpinVariance(value); break;
        case range::endSpin: controller->setParticleEndSpinVariance(value); break;

        case range::startColorAlpha: controller->setParticleStartColorVarianceAlpha(value);
                                     colorButtonSetColor(colorWidget[1], "Kolor startowy rozbierzność : ", controller->getParticleStartColorVariance());
                                     break;
        case range::endColorAlpha: controller->setParticleEndColorVarianceAlpha(value);
                                   colorButtonSetColor(colorWidget[3], "Kolor końcowy rozbierzność : ", controller->getParticleEndColorVariance());
                                   break;
    }
}

void QparametersController::colorButtonSetColor(std::pair<QPushButton*,rangeParameterWidget*> *pair, QString text, QColor color)
{
    QString colText = QVariant(color).toString();
    int r,g,b;

    r = 255- color.red();
    if ( r > 86 && r < 172)
        r += 50;

    g = 255- color.green();
    if ( g > 86 && g < 172)
        g += 50;

    b = 255- color.blue();
    if ( b > 86 && b < 172)
        b += 50;

    QString colTextInvert = QVariant( QColor(r,g,b)).toString();

    pair->first->setText(text+" (ARGB) "+colText);
    pair->first->setStyleSheet("background-color: "+colText+"; color: "+colTextInvert+"; border: 2px solid #ffffff;");
    pair->second->setValue(color.alpha());
}

