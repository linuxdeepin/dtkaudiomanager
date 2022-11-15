// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DAUDIOCARD_H
#define DAUDIOCARD_H

#include <dtkaudiomanager_global.h>
#include <daudioport.h>

#include <QObject>
#include <QSharedPointer>

DAUDIOMANAGER_BEGIN_NAMESPACE
class DAudioPort;
class DPlatformAudioCard;
using DAudioPortPtr = QSharedPointer<DAudioPort>;
class LIBDTKAUDIOMANAGERSHARED_EXPORT DAudioCard : public QObject
{
    Q_OBJECT
public:

    explicit DAudioCard(DPlatformAudioCard *d);
    virtual ~DAudioCard() override;

    QList<DAudioPortPtr> ports() const;
    QList<DAudioPortPtr> availablePorts() const;
    QString name() const;
    quint32 id() const;
    bool available() const;

protected:
    friend class DPlatformAudioCard;
    QExplicitlySharedDataPointer<DPlatformAudioCard> d;
};

DAUDIOMANAGER_END_NAMESPACE

#endif
