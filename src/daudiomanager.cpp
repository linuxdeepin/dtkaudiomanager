// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "daudio_p.h"

#include "daudiomanager.h"

#include <QDebug>
#include <QLoggingCategory>

DAUDIOMANAGER_BEGIN_NAMESPACE

// daudiomanager log category
#ifndef QT_DEBUG
Q_LOGGING_CATEGORY(amLog, "dtk.audiomanager", QtInfoMsg)
#else
Q_LOGGING_CATEGORY(amLog, "dtk.audiomanager")
#endif

DAudioManager::DAudioManager(QObject *parent)
    : QObject(parent)
    , d(new DAudioManagerPrivate(this))
{
    Q_ASSERT(this->d);
    d->start();
}


DAudioManager::~DAudioManager()
{
    d->release();
    d->wait(300);
}

QList<DAudioCardPtr> DAudioManager::cards() const
{
    d->ensureReady();

    QList<DAudioCardPtr> result;
    for (auto item : d->m_cards) {
        result << DAudioCardPtr(item->create());
    }
    return result;
}

DAudioCardPtr DAudioManager::cardById(quint32 idx)
{
    d->ensureReady();

    if (auto card = d->card(idx))
        return DAudioCardPtr(card->create());

    return nullptr;
}

bool DAudioManager::ready() const
{
    return d->m_ready;
}
DAUDIOMANAGER_END_NAMESPACE
