// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DAUDIOMANAGER_H
#define DAUDIOMANAGER_H

#include <dtkaudiomanager_global.h>
#include <daudiocard.h>
#include <daudioport.h>

#include <QObject>
#include <QSharedPointer>
DAUDIOMANAGER_BEGIN_NAMESPACE

class DAudioCard;
using DAudioCardPtr = QSharedPointer<DAudioCard>;

class DAudioManagerPrivate;
class LIBDTKAUDIOMANAGERSHARED_EXPORT DAudioManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)

public:
    explicit DAudioManager(QObject *parent = nullptr);
    virtual ~DAudioManager() override;

    QList<DAudioCardPtr> cards() const;
    DAudioCardPtr cardById(quint32 idx);

    bool ready() const;

Q_SIGNALS:
    void cardAdded(quint32 idx);
    void cardRemoved(quint32 idx);
    void cardUpdated(quint32 idx);

    void readyChanged(bool ready);

private:
    Q_DISABLE_COPY(DAudioManager)
    QScopedPointer<DAudioManagerPrivate> d;
};
DAUDIOMANAGER_END_NAMESPACE

#endif
