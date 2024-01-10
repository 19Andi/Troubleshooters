#include "MenuScene.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include<QPushButton>
// Defineste dimensiunile initiale
#define BUTTON_STYLE "QPushButton { \
    background-color: #7B7B7B; \
    color: white; \
    border: 2px solid #343434; \
    border-radius: 8px; \
    padding: 4px 8px; \
} \
QPushButton:hover { \
    background-color: #555555; \
} \
QPushButton:pressed { \
    background-color: #3F3F3F; \
    color: white; \
}"
MenuScene::MenuScene(QObject* parent, int initialWidth, int initialHeight) : QGraphicsScene(parent)
{
    Width = initialWidth;
    Height = initialHeight;
    // Adaug? imaginea de fundal
    addBackground(Width, Height);
    // Adaug? butoanele
    QPushButton* playButton = createButton("Play", 100, 100, 100, 100);
    QGraphicsProxyWidget* proxyButton1 = new QGraphicsProxyWidget();
    proxyButton1->setWidget(playButton);
    addItem(proxyButton1);
    connect(playButton, &QPushButton::clicked, this, &MenuScene::playClicked);

    QPushButton* loadButton = createButton("Load", 100, 100, 100, 300);
    QGraphicsProxyWidget* proxyButton2 = new QGraphicsProxyWidget();
    proxyButton2->setWidget(loadButton);
    addItem(proxyButton2);

    QPushButton* exitButton = createButton("Exit", 100, 100, 100, 500);
    QGraphicsProxyWidget* proxyButton3 = new QGraphicsProxyWidget();
    proxyButton3->setWidget(exitButton);
    addItem(proxyButton3);

    // addButton("Load", 200, SLOT(loadClicked()));
     //addButton("Exit", 300, SLOT(exitClicked()));
     //addButton("Settings", 400, SLOT(settingsClicked()));
}

void MenuScene::settingsClicked()
{
    // Implementare pentru settingsClicked

}

void MenuScene::loadClicked()
{
    // Implementare pentru loadClicked

}

void MenuScene::exitClicked()
{
    // Implementare pentru exitClicked

}
QPushButton* MenuScene::createButton(const QString& text, qreal buttonWidth, qreal buttonHeight, qreal buttonX, qreal buttonY)
{
    QPushButton* button = new QPushButton(text, nullptr);
    button->setStyleSheet(BUTTON_STYLE);
    button->setGeometry(buttonX, buttonY, buttonWidth, buttonHeight);
    return button;
}

void MenuScene::resizeBackground(int width, int height)
{
    if (backgroundItem) {
        // Redimensioneaz? imaginea pentru a se potrivi cu noile dimensiuni
        QPixmap resizedImage = originalBackgroundImage.scaled(width, height);

        // Afi?eaz? dimensiunile redimensionate ale imaginii
        qDebug() << "Dimensiuni redimensionate:" << resizedImage.width() << "x" << resizedImage.height();

        // Seteaz? noua imagine redimensionat? la obiectul QGraphicsPixmapItem
        backgroundItem->setPixmap(resizedImage);
    }
}

void MenuScene::addBackground(int Width, int Height)
{
    originalBackgroundImage.load("../assets/hatz.png");
    // Creaz? un obiect QGraphicsPixmapItem cu imaginea de fundal
    backgroundItem = new QGraphicsPixmapItem(originalBackgroundImage);

    // Seteaz? pozi?ia pentru a acoperi �ntreaga scen?
    backgroundItem->setPos(0, 0);
    resizeBackground(Width, Height);
    // Adaug? imaginea de fundal la scen?
    addItem(backgroundItem);
}
