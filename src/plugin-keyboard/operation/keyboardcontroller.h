//SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
//SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "keyboardmodel.h"
#include "keyboardwork.h"

#include <QObject>
namespace dccV25 {

class KeyboardController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint repeatInterval READ repeatInterval WRITE setRepeatInterval NOTIFY repeatIntervalChanged FINAL)
    Q_PROPERTY(uint repeatDelay READ repeatDelay WRITE setRepeatDelay NOTIFY repeatDelayChanged FINAL)
    Q_PROPERTY(bool numLock READ numLock WRITE setNumLock NOTIFY numLockChanged FINAL)
    Q_PROPERTY(bool capsLock READ capsLock WRITE setCapsLock NOTIFY capsLockChanged FINAL)
    Q_PROPERTY(QMap<QString, QString> keyboardLayouts READ keyboardLayouts NOTIFY keyboardLayoutsChanged FINAL)

public:
    explicit KeyboardController(QObject *parent = nullptr);

    uint repeatInterval() const;
    void setRepeatInterval(uint newRepeatInterval);

    uint repeatDelay() const;
    void setRepeatDelay(uint newRepeatDelay);

    bool numLock() const;
    void setNumLock(bool newNumLock);

    bool capsLock() const;
    void setCapsLock(bool newCapsLock);

    QMap<QString, QString> keyboardLayouts() const;

public Q_SLOTS:
    void addUserLayout(const QString &layout);
    void deleteUserLayout(const QString &layout);

signals:
    void repeatIntervalChanged();
    void repeatDelayChanged();
    void numLockChanged();
    void capsLockChanged();
    void keyboardLayoutsChanged();

private:
    uint m_repeatInterval;
    uint m_repeatDelay;
    bool m_numLock;
    bool m_capsLock;
    KeyboardWorker *m_work = nullptr;
    KeyboardModel *m_model = nullptr;
};

}
#endif // KEYBOARDCONTROLLER_H
