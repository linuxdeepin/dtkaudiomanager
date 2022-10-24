// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "daudiostream_daemon.h"
#include "daudiodevice.h"
#include "daemonhelpers.hpp"

#include <QDBusArgument>
#include <QDebug>

DAUDIOMANAGER_BEGIN_NAMESPACE
DDAemonStreamInterface::DDAemonStreamInterface(DDBusInterface *inter, DPlatformAudioStream *owner)
    : m_inter(inter)
    , m_owner(owner)
{
    connect(this, &DDAemonStreamInterface::MuteChanged, m_owner, &DPlatformAudioStream::muteChanged);
    connect(this, &DDAemonStreamInterface::FadeChanged, m_owner, &DPlatformAudioStream::fadeChanged);
    connect(this, &DDAemonStreamInterface::VolumeChanged, m_owner, &DPlatformAudioStream::volumeChanged);
    connect(this, &DDAemonStreamInterface::BalanceChanged, m_owner, &DPlatformAudioStream::balanceChanged);

    connect(this, &DDAemonStreamInterface::SupportBalanceChanged, m_owner, &DPlatformAudioStream::supportBalanceChanged);
    connect(this, &DDAemonStreamInterface::SupportFadeChanged, m_owner, &DPlatformAudioStream::supportFadeChanged);
}

bool DDAemonStreamInterface::mute() const
{
    return qdbus_cast<bool>(m_inter->property("Mute"));
}

double DDAemonStreamInterface::fade() const
{
    return qdbus_cast<double>(m_inter->property("Fade"));
}

double DDAemonStreamInterface::volume() const
{
    return qdbus_cast<double>(m_inter->property("Volume"));
}

double DDAemonStreamInterface::balance() const
{
    return qdbus_cast<double>(m_inter->property("Balance"));
}

bool DDAemonStreamInterface::supportBalance() const
{
    return qdbus_cast<bool>(m_inter->property("SupportBalance"));
}

bool DDAemonStreamInterface::supportFade() const
{
    return qdbus_cast<bool>(m_inter->property("SupportFade"));
}

void DDAemonStreamInterface::setMute(bool mute)
{
    m_inter->call("SetMute", mute);
}

void DDAemonStreamInterface::setFade(double fade)
{
    m_inter->call("SetFade", fade);
}

void DDAemonStreamInterface::setVolume(double volume)
{
    m_inter->call("SetVolume", volume, m_owner->isPlay());
}

void DDAemonStreamInterface::setBalance(double balance)
{
    m_inter->call("SetBalance", balance, m_owner->isPlay());
}

DDaemonInputStream::DDaemonInputStream(const QString &path, DPlatformAudioOutputDevice *parent)
    : DPlatformAudioInputStream (parent)
    , m_interface(new DDAemonStreamInterface(
                      DDaemonInternal::newAudioInterface2(
                          this, path, DDaemonInternal::AudioServiceSinkInputInterface),
                      this))
{
    setName(DDaemonInternal::streamName(path));
}

DDaemonInputStream::~DDaemonInputStream()
{

}

bool DDaemonInputStream::mute() const
{
    return m_interface->mute();
}

double DDaemonInputStream::fade() const
{
    return m_interface->fade();
}

double DDaemonInputStream::volume() const
{
    return m_interface->volume();
}

double DDaemonInputStream::balance() const
{
    return m_interface->balance();
}

bool DDaemonInputStream::supportBalance() const
{
    return m_interface->supportBalance();
}

bool DDaemonInputStream::supportFade() const
{
    return m_interface->supportFade();
}

void DDaemonInputStream::setMute(bool mute)
{
    m_interface->setMute(mute);
}

void DDaemonInputStream::setFade(double fade)
{
    m_interface->setFade(fade);
}

void DDaemonInputStream::setVolume(double volume)
{
    m_interface->setVolume(volume);
}

void DDaemonInputStream::setBalance(double balance)
{
    m_interface->setBalance(balance);
}

DDaemonOutputStream::DDaemonOutputStream(const QString &path, DPlatformAudioInputDevice *parent)
    : DPlatformAudioOutputStream (parent)
    , m_interface(new DDAemonStreamInterface(
                      DDaemonInternal::newAudioInterface2(
                          this, path, DDaemonInternal::AudioServiceSourceOutputInterface),
                      this))
{
    setName(DDaemonInternal::streamName(path));
}

DDaemonOutputStream::~DDaemonOutputStream()
{

}

bool DDaemonOutputStream::mute() const
{
    return m_interface->mute();
}

double DDaemonOutputStream::fade() const
{
    return m_interface->fade();
}

double DDaemonOutputStream::volume() const
{
    return m_interface->volume();
}

double DDaemonOutputStream::balance() const
{
    return m_interface->balance();
}

bool DDaemonOutputStream::supportBalance() const
{
    return m_interface->supportBalance();
}

bool DDaemonOutputStream::supportFade() const
{
    return m_interface->supportFade();
}

void DDaemonOutputStream::setMute(bool mute)
{
    m_interface->setMute(mute);
}

void DDaemonOutputStream::setFade(double fade)
{
    m_interface->setFade(fade);
}

void DDaemonOutputStream::setVolume(double volume)
{
    m_interface->setVolume(volume);
}

void DDaemonOutputStream::setBalance(double balance)
{
    m_interface->setBalance(balance);
}

DAUDIOMANAGER_END_NAMESPACE
