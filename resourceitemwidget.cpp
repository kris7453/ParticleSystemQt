#include "resourceitemwidget.h"

    QdrawableListWidget *resourceItemWidget::listWidgetPointer;
    _icon *resourceItemWidget::icon;

resourceItemWidget::resourceItemWidget( QWidget *parent) : QWidget(parent)
{
}

resourceItemWidget::resourceItemWidget( PSystemAPI::pDrawableItem *item, QString name, QString *path, QWidget *parent) :
    name(name), path(path), itemPointer(item), QWidget(parent)
{
    iconImage = new QLabel(this);
    iconImage->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    iconImage->setMinimumSize(24,24);
    iconImage->setMaximumSize(24,24);
    iconImage->setPixmap(QPixmap( (path != nullptr) ? *path : QString(":/particles/star.png")));
    iconImage->setScaledContents(true);

    resourceName = new QLabel( this->name, this);
    resourceName->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
    resourceName->setMinimumSize(32,24);
    resourceName->setMaximumSize(16000000,24);

    createElements();
    connectElements();
    createButtonsConnections();
}

resourceItemWidget::resourceItemWidget(resourceItemWidget &item) : QWidget(item.parentWidget())
{

    name = item.name;
    path = item.path;
    itemPointer = item.itemPointer;

    iconImage = item.iconImage,
    iconImage->setParent(this);
    item.iconImage = nullptr;

    resourceName = item.resourceName;
    resourceName->setParent(this);
    item.resourceName = nullptr;

    createElements();
    clearElementsConnections();
    connectElements();
    createButtonsConnections();
    changeVisibilityIcon( itemPointer->isVisible() );
}

resourceItemWidget::~resourceItemWidget()
{
    clearButtonsConnections();
}

void resourceItemWidget::changeVisibilityIcon( bool isVisible)
{
    if (isVisible)
        visibilityBtn->setIcon(icon->visible);
    else
        visibilityBtn->setIcon(icon->hidden);
}

void resourceItemWidget::setLayersButtonsEnabled(bool upButton, bool downButton)
{
    layerUpBtn->setEnabled(upButton);
    layerDownBtn->setEnabled(downButton);
}

void resourceItemWidget::setIcon(QString *path)
{
    iconImage->setPixmap(QPixmap(*path));
}

void resourceItemWidget::setListWidgetPointer(QdrawableListWidget *listWidget)
{
    listWidgetPointer = listWidget;
    icon = listWidgetPointer->icon;
}

void resourceItemWidget::createElements()
{
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    widgetLayout = new QHBoxLayout(this);
    widgetLayout->setMargin(0);
    widgetLayout->setSpacing(0);

    visibilityBtn = new QPushButton(this);
    visibilityBtn->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    visibilityBtn->setMinimumSize(24,24);
    visibilityBtn->setMaximumSize(24,24);
    visibilityBtn->setIcon(icon->visible);

    closeBtn = new QPushButton(this);
    closeBtn->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    closeBtn->setMinimumSize(24,24);
    closeBtn->setMaximumSize(24,24);
    closeBtn->setIcon(icon->close);

    layerButtonsWidget = new QWidget(this);
    layerButtonsLayout = new QVBoxLayout(layerButtonsWidget);
    layerButtonsLayout->setMargin(0);
    layerButtonsLayout->setSpacing(0);

    layerUpBtn = new QPushButton(layerButtonsWidget);
    layerUpBtn->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    layerUpBtn->setMinimumSize(12,12);
    layerUpBtn->setMaximumSize(12,12);
    layerUpBtn->setIcon(icon->arrowUp);

    layerDownBtn = new QPushButton(layerButtonsWidget);
    layerDownBtn->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    layerDownBtn->setMinimumSize(12,12);
    layerDownBtn->setMaximumSize(12,12);
    layerDownBtn->setIcon(icon->arrowDown);

    renameBtn = new QPushButton(this);
    renameBtn->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    renameBtn->setMinimumSize(24,24);
    renameBtn->setMaximumSize(24,24);
    renameBtn->setIcon(icon->edit);
}

void resourceItemWidget::connectElements()
{
    widgetLayout->addWidget(iconImage);
    widgetLayout->addWidget(layerButtonsWidget);
    layerButtonsLayout->addWidget(layerUpBtn);
    layerButtonsLayout->addWidget(layerDownBtn);
    widgetLayout->addWidget(visibilityBtn);
    widgetLayout->addWidget(closeBtn);
    widgetLayout->addWidget(renameBtn);
    widgetLayout->addWidget(resourceName);
}

void resourceItemWidget::clearElementsConnections()
{
    widgetLayout->removeWidget(iconImage);
    widgetLayout->removeWidget(layerButtonsWidget);
    layerButtonsLayout->removeWidget(layerUpBtn);
    layerButtonsLayout->removeWidget(layerDownBtn);
    widgetLayout->removeWidget(visibilityBtn);
    widgetLayout->removeWidget(closeBtn);
    widgetLayout->removeWidget(renameBtn);
    widgetLayout->removeWidget(resourceName);
}

void resourceItemWidget::createButtonsConnections()
{
    visibilityConnection = connect(visibilityBtn, &QPushButton::clicked, [this](){ emit listWidgetPointer->changeVisibility(this);});
    closeConnection = connect(closeBtn, &QPushButton::clicked, [this](){ emit listWidgetPointer->close(this);});
    layerUpConnection = connect(layerUpBtn, &QPushButton::clicked, [this](){ emit listWidgetPointer->layerUp(this);});
    layerDownConnection = connect(layerDownBtn, &QPushButton::clicked, [this](){ emit listWidgetPointer->layerDown(this);});
    changeNameConnection = connect(renameBtn, &QPushButton::clicked, [this](){
        bool ok;
        QString name = QInputDialog::getText(this, tr("Nowa nazwa"), tr("Podaj nazwę obiektu"), QLineEdit::Normal,QString(), &ok);

        if ( ok )
        {
            this->name = name;
            resourceName->setText(name);
            emit listWidgetPointer->changeName(this, name);
        }
    });
}

void resourceItemWidget::clearButtonsConnections()
{
    disconnect( visibilityConnection );
    disconnect( closeConnection );
    disconnect( layerUpConnection);
    disconnect( layerDownConnection);
}

PSystemAPI::pDrawableItem *resourceItemWidget::getItemPointer()
{
    return itemPointer;
}
