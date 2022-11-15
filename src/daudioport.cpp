// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "daudioport.h"
#include "daudio_p.h"

#include <QDebug>

DAUDIOMANAGER_BEGIN_NAMESPACE

DAudioPort::DAudioPort(DPlatformAudioPort *d)
    :d(d)
{
    Q_ASSERT(d);
}

DAudioPort::~DAudioPort()
{

}

bool DAudioPort::available() const
{
    return d->m_available;
}

int DAudioPort::direction() const
{
    return d->m_direction;
}

QString DAudioPort::name() const
{
    return d->m_name;
}

QString DAudioPort::description() const
{
    return d->m_description;
}

DAUDIOMANAGER_END_NAMESPACE
