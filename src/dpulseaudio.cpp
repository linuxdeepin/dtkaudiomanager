#include "dpulseaudio.h"

DAUDIOMANAGER_BEGIN_NAMESPACE

DPlatformAudioPort::DPlatformAudioPort(DPlatformAudioCard *card)
    : m_card(card)
{
    if (m_card) {
        m_card->addPort(this);
    }
}

DPlatformAudioPort::~DPlatformAudioPort()
{

}

DPlatformAudioCard::DPlatformAudioCard()
{
}

DPlatformAudioCard::~DPlatformAudioCard()
{

}

void DPlatformAudioCard::addPort(DPlatformAudioPort *port)
{
    m_ports.append(QExplicitlySharedDataPointer(port));
}

void DPlatformAudioCard::reset()
{
    m_ports.clear();
}

void DPlatformAudioCard::update(pa_context *context, const pa_card_info *cardInfo, int eol, void *userData)
{
    Q_UNUSED(context);

    if (!eol)
    {
        auto card = reinterpret_cast< DPlatformAudioCard* >(userData);
        card->reset();
        pa2daCardInfo(cardInfo, card);
        card->emitCardChanged();
    }
}

void DPlatformAudioCard::pa2daCardInfo(const pa_card_info *i, DPlatformAudioCard *card)
{
    card->m_id = i->index;
    card->m_name = i->name;

    if (i->ports) {
        for (pa_card_port_info **p = i->ports; *p; p++) {
            auto port = new DPlatformAudioPort(card);
            port->m_name = (*p)->name;
            port->m_available = (*p)->available != PA_PORT_AVAILABLE_NO;
            port->m_description = (*p)->description;
            port->m_direction = (*p)->direction;
        }
    }
}

void DPlatformAudioCard::emitCardChanged()
{
    Q_EMIT m_manager->m_owner->cardUpdated(m_id);
}


DAudioManagerPrivate::DAudioManagerPrivate(DAudioManager *owner)
    : QThread(owner)
    , m_owner(owner)
{

}

DAudioManagerPrivate::~DAudioManagerPrivate()
{
}

void DAudioManagerPrivate::addCard(DPlatformAudioCard *card)
{
    card->m_manager = this;
    m_cards.append(QExplicitlySharedDataPointer(card));
    Q_EMIT m_owner->cardAdded(card->m_id);
}

DPlatformAudioCard *DAudioManagerPrivate::card(const quint32 idx)
{
    for (auto item : m_cards) {
        if (item->m_id == idx)
            return item.data();
    }
    return nullptr;
}

void DAudioManagerPrivate::removeCard(const quint32 idx)
{
    auto iter = std::find_if(m_cards.begin(), m_cards.end(), [idx](QExplicitlySharedDataPointer<DPlatformAudioCard> item) {
            return item->m_id == idx;
    });

    if (iter != m_cards.end()) {
        m_cards.erase(iter);
        Q_EMIT m_owner->cardRemoved(idx);
    }
}

void DAudioManagerPrivate::setReady(bool ready)
{
    if (m_ready == ready)
        return;

    m_ready = ready;
    Q_EMIT m_owner->readyChanged(m_ready);
}

void DAudioManagerPrivate::release()
{
    if (m) {
        pa_mainloop_quit(m, 0);
    }
}

int DAudioManagerPrivate::mainLoop()
{
    int ret = 1;
    proplist = pa_proplist_new();

    if (!(m = pa_mainloop_new())) {
        qWarning("pa_mainloop_new() failed.");
        exit(-1);
    }
    mainloop_api = pa_mainloop_get_api(m);

    if (!(context = pa_context_new_with_proplist(mainloop_api, nullptr, proplist))) {
        qWarning("pa_context_new() failed.");
        return -1;
    }

    pa_context_set_state_callback(context, &DAudioManagerPrivate::onContextStateChange, this);

    if (pa_context_connect(context, server, PA_CONTEXT_NOFLAGS, nullptr) < 0) {
        qWarning() << QString("pa_context_connect() failed: %s").arg(pa_strerror(pa_context_errno(context)));
        return -1;
    }

    if (pa_mainloop_run(m, &ret) < 0) {
        qWarning() << "pa_mainloop_run() failed.";
        return -1;
    }

    if (context)
        pa_context_unref(context);

    if (m) {
        pa_signal_done();
        pa_mainloop_free(m);
        m = nullptr;
    }

    pa_xfree(server);

    if (proplist)
        pa_proplist_free(proplist);

    return 0;
}

void DAudioManagerPrivate::onContextStateChange(pa_context *c, void *userData)
{
    auto manager = reinterpret_cast< DAudioManagerPrivate* >(userData);

    switch (pa_context_get_state(c)) {
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
        break;

    case PA_CONTEXT_READY: {
        pa_operation_unref(
                    pa_context_get_card_info_list(
                        manager->context, &DAudioManagerPrivate::cardList, manager));

        pa_context_set_subscribe_callback(
                    manager->context, &DAudioManagerPrivate::onContextSubscriptionEvent, userData);

        pa_operation_unref(
                    pa_context_subscribe(manager->context,
                                         PA_SUBSCRIPTION_MASK_CARD,
                                         nullptr,
                                         userData));
        manager->setReady(true);

    } break;

    case PA_CONTEXT_TERMINATED:
        manager->setReady(false);
        break;

    case PA_CONTEXT_FAILED:
    default:
        manager->setReady(false);
        qWarning("Connection failure: %s", pa_strerror(pa_context_errno(c)));
        break;
    }
}

void DAudioManagerPrivate::onContextSubscriptionEvent(pa_context *context, pa_subscription_event_type_t eventType, uint32_t idx, void *userData)
{
    Q_UNUSED(context);

    auto manager = reinterpret_cast<DAudioManagerPrivate *>(userData);
    Q_ASSERT(manager);

    switch (eventType & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
    case PA_SUBSCRIPTION_EVENT_CARD:
        processCardEvent(eventType & PA_SUBSCRIPTION_EVENT_TYPE_MASK, idx, manager);
        break;
    }
}

void DAudioManagerPrivate::processCardEvent(int event, uint32_t idx, DAudioManagerPrivate *manager)
{
    switch (event) {
    case PA_SUBSCRIPTION_EVENT_NEW: {
        pa_operation_unref(
                    pa_context_get_card_info_by_index(
                        manager->context, idx, &DAudioManagerPrivate::onCardInfo, manager));
        qDebug() << "card add";
    } break;

    case PA_SUBSCRIPTION_EVENT_CHANGE: {
        auto card = manager->card(idx);
        if (!card) {
            return;
        }
        pa_operation_unref(
                    pa_context_get_card_info_by_index(
                        manager->context, idx, &DPlatformAudioCard::update, card));
        qDebug() << "card change";
    } break;

    case PA_SUBSCRIPTION_EVENT_REMOVE: {
        qDebug() << "card remove";
        manager->removeCard(idx);
    } break;
    }
}

void DAudioManagerPrivate::onCardInfo(pa_context *context, const pa_card_info *cardInfo, int eol, void *userData)
{
    Q_UNUSED(context)
    if (!cardInfo)
        return;

    if (!eol) {

        auto manager = reinterpret_cast< DAudioManagerPrivate* >(userData);

        auto card = new DPlatformAudioCard();

        DPlatformAudioCard::pa2daCardInfo(cardInfo, card);

        manager->addCard(card);
    }
}

void DAudioManagerPrivate::cardList(pa_context *context, const pa_card_info *cardInfo, int eol, void *userData)
{
    onCardInfo(context, cardInfo, eol, userData);
}

DAUDIOMANAGER_END_NAMESPACE
