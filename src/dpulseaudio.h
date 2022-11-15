// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include "daudiomanager.h"

#include "daudioport.h"
#include "daudiocard.h"

#include <pulse/pulseaudio.h>
#include <pulse/context.h>

#include <QPointer>
#include <QDebug>
#include <QThread>

DAUDIOMANAGER_BEGIN_NAMESPACE

class DPlatformAudioCard;
class DPlatformAudioPort : public QSharedData
{
public:
    explicit DPlatformAudioPort(DPlatformAudioCard *card);
    ~DPlatformAudioPort();
    DAudioPort *create()
    {
        return new DAudioPort(this);
    }

    DPlatformAudioCard *m_card = nullptr;
    QString m_name;
    QString m_description;
    int m_direction;
    bool m_available;
};

class DPlatformAudioCard : public QSharedData
{
public:
    explicit DPlatformAudioCard();
    ~DPlatformAudioCard();

    DAudioCard *create()
    {
        return new DAudioCard(this);
    }
    void addPort(DPlatformAudioPort *port);
    void reset();

    static void update(pa_context *context, const pa_card_info *cardInfo, int eol, void *userData);

    static void pa2daCardInfo(const pa_card_info*i, DPlatformAudioCard *card);

    void emitCardChanged();

    QString m_name;
    quint32 m_id;
    bool m_available;
    QList<QExplicitlySharedDataPointer<DPlatformAudioPort>> m_ports;

    DAudioManagerPrivate *m_manager = nullptr;
};


class DAudioManagerPrivate : public QThread
{
public:
    explicit DAudioManagerPrivate(DAudioManager *owner);
    virtual ~DAudioManagerPrivate() override;

    DPlatformAudioCard *card(const quint32 idx);
    void addCard(DPlatformAudioCard *card);
    void removeCard(const quint32 idx);

    void updateCard();
    QList<QExplicitlySharedDataPointer<DPlatformAudioCard>> m_cards;
    DAudioManager *m_owner;

    void setReady(bool ready);

    void ensureReady() const
    {
        if (!m_ready) {
            const_cast<DAudioManagerPrivate *>(this)->wait(300);
        }
    }
    void release();

protected:
    virtual void run() override
    {
        mainLoop();
    }

public:
    int mainLoop();
    static void onContextStateChange(pa_context* c, void* userData);

    static void onContextSubscriptionEvent(
        pa_context* context, pa_subscription_event_type_t eventType, uint32_t idx, void* userData);

    static void processCardEvent(int event, uint32_t idx, DAudioManagerPrivate* manager);

    static void onCardInfo(
        pa_context* context, const pa_card_info* cardInfo, int eol, void* userData);
    static void cardList(
        pa_context* context, const pa_card_info* cardInfo, int eol, void* userData);

    pa_mainloop *m = nullptr;
    pa_context *context = nullptr;
    pa_mainloop_api *mainloop_api = nullptr;
    pa_proplist *proplist = nullptr;
    char *server = nullptr;
    bool m_ready = false;
};

DAUDIOMANAGER_END_NAMESPACE
