// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QCoreApplication>

#include <DAudioManager>

#include <QDebug>
#include <QTime>
#include <QTimer>

AUDIOMANAGER_USE_NAMESPACE;
void printCard(const DAudioCard *card)
{
    qInfo() << card->name() << card->id();
    qInfo() << "Ports size:" << card->ports().size();
    for (auto port : card->ports()) {
        qInfo() << port->name() << port->description() << port->direction() << port->available();
    }
}

void printCards(const DAudioManager &manager)
{
    auto cards = manager.cards();
    qInfo() << "Cards size:" << cards.count();
    for (auto card : cards) {
        printCard(card.data());
    }
}
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    DAudioManager manager;

    QObject::connect(&manager, &DAudioManager::cardAdded, [&manager](quint32 idx){
        qInfo() << "----------cardAdded----------" << idx;
        if (auto card = manager.cardById(idx)) {
            printCard(card.data());
        }
    });

    QObject::connect(&manager, &DAudioManager::cardRemoved, [&manager](quint32 idx){
        qInfo() << "----------cardRemoved----------" << idx;
        printCards(manager);
    });

    QObject::connect(&manager, &DAudioManager::cardUpdated, [&manager](quint32 idx){
        qInfo() << "----------cardUpdated----------" << idx;
        if (auto card = manager.cardById(idx)) {
            printCard(card.data());
        }
    });

    printCards(manager);
    return app.exec();
}
