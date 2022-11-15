// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DAUDIOPORT_H
#define DAUDIOPORT_H

#include <audiomanager_global.h>

#include <QObject>
#include <QPointer>

AUDIOMANAGER_BEGIN_NAMESPACE
class DPlatformAudioPort;
class LIBAUDIOMANAGERSHARED_EXPORT DAudioPort : public QObject
{
    Q_OBJECT

public:
    explicit DAudioPort(DPlatformAudioPort *d);
    virtual ~DAudioPort() override;

    int direction() const;
    QString name() const;
    QString description() const;
    bool available() const;

public:
    Q_DISABLE_COPY(DAudioPort)
    friend class DPlatformAudioPort;
    QExplicitlySharedDataPointer<DPlatformAudioPort> d;
};
AUDIOMANAGER_END_NAMESPACE

#endif
