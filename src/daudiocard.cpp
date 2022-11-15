// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "daudiocard.h"
#include "daudio_p.h"

#include "daudioport.h"

#include <QDebug>

AUDIOMANAGER_BEGIN_NAMESPACE

DAudioCard::DAudioCard(DPlatformAudioCard *d)
    : d(d)
{
    Q_ASSERT(d);
}

DAudioCard::~DAudioCard()
{

}

QList<DAudioPortPtr> DAudioCard::ports() const
{
    QList<DAudioPortPtr> result;
    for (auto item : d->m_ports)
        result << DAudioPortPtr(item->create());

    return result;
}

QList<DAudioPortPtr> DAudioCard::availablePorts() const
{
    QList<DAudioPortPtr> result;
    for (auto port : d->m_ports) {
        if (port->m_available) {
            result << DAudioPortPtr(port->create());
        }
    }
    return result;
}

QString DAudioCard::name() const
{
    return d->m_name;
}

quint32 DAudioCard::id() const
{
    return d->m_id;
}

bool DAudioCard::available() const
{
    return d->m_available;
}

AUDIOMANAGER_END_NAMESPACE
