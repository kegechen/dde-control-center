//SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
//SPDX-License-Identifier: GPL-3.0-or-later

#include "keyboardcontroller.h"
#include "dccfactory.h"
namespace dccV25 {
DCC_FACTORY_CLASS(KeyboardController)

KeyboardController::KeyboardController(QObject *parent)
    : QObject(parent)
{
    m_model = new KeyboardModel(this);
    m_work = new KeyboardWorker(m_model, this);

    connect(m_model, &KeyboardModel::repeatIntervalChanged, this, &KeyboardController::repeatIntervalChanged);
    connect(m_model, &KeyboardModel::repeatDelayChanged, this, &KeyboardController::repeatDelayChanged);
    connect(m_model, &KeyboardModel::numLockChanged, this, &KeyboardController::numLockChanged);
    connect(m_model, &KeyboardModel::capsLockChanged, this, &KeyboardController::capsLockChanged);
    connect(m_model, &KeyboardModel::userLayoutChanged, this, &KeyboardController::keyboardLayoutsChanged);

    QMetaObject::invokeMethod(m_work, "active", Qt::QueuedConnection);
}

uint KeyboardController::repeatInterval() const
{
    return m_model->repeatInterval();
}

void KeyboardController::setRepeatInterval(uint newRepeatInterval)
{
    if (repeatInterval() == newRepeatInterval)
        return;

    m_work->setRepeatInterval(newRepeatInterval);
}

uint KeyboardController::repeatDelay() const
{
    return m_model->repeatDelay();
}

void KeyboardController::setRepeatDelay(uint newRepeatDelay)
{
    if (repeatDelay() == newRepeatDelay)
        return;

    m_work->setRepeatDelay(newRepeatDelay);
}

bool KeyboardController::numLock() const
{
    return m_model->numLock();
}

void KeyboardController::setNumLock(bool newNumLock)
{
    if (numLock() == newNumLock)
        return;

    m_work->setNumLock(newNumLock);
}

bool KeyboardController::capsLock() const
{
    return m_model->capsLock();
}

void KeyboardController::setCapsLock(bool newCapsLock)
{
    if (capsLock() == newCapsLock)
        return;

    m_work->setCapsLock(newCapsLock);
}

QMap<QString, QString> KeyboardController::keyboardLayouts() const
{
    return m_model->kbLayout();
}

void KeyboardController::addUserLayout(const QString &layout)
{
    m_work->addUserLayout(layout);
}

void KeyboardController::deleteUserLayout(const QString &layout)
{
    m_work->delUserLayout(layout);
}

}

#include "keyboardcontroller.moc"
