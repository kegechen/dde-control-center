/*
 * Copyright (C) 2011 ~ 2023 Deepin Technology Co., Ltd.
 *
 * Author:     duanhongyu <duanhongyu@uniontech.com>

 * Maintainer: duanhongyu <duanhongyu@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "privacysecuritydbusproxy.h"
#include "widgets/dccdbusinterface.h"

#include <QDBusArgument>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QDBusMetaType>
#include <QDBusReply>

#include <QDebug>

const static QString PermissionService = QStringLiteral("org.desktopspec.permission");
const static QString PermissionPath = QStringLiteral("/org/desktopspec/permission");
const static QString PermissionInterface = QStringLiteral("org.desktopspec.permission");

DCC_USE_NAMESPACE
PrivacySecurityDBusProxy::PrivacySecurityDBusProxy(QObject *parent)
    : QObject(parent)
    , m_privacyInter(new DCCDBusInterface(PermissionService, PermissionPath, PermissionInterface, QDBusConnection::sessionBus(), this))
{
    connect(this, &PrivacySecurityDBusProxy::PermissionInfoChanged, this, &PrivacySecurityDBusProxy::getPermissionInfo);
}

void PrivacySecurityDBusProxy::getPermissionInfo()
{
    qDebug() << " PrivacySecurityDBusProxy::GetPermissionInfo ";
    // 使用异步回调展示数据
    QList<QVariant> argumentList;
    m_privacyInter->callWithCallback(QStringLiteral("GetPermissionInfo"), argumentList, this, SIGNAL(permissionInfoLoadFinished(QString)));
}

void PrivacySecurityDBusProxy::setPermissionInfo(const QString &appId, const QString &permissionGroup, const QString &permissionId, const QString &value)
{
    QDBusPendingCall pcall = m_privacyInter->asyncCall(QStringLiteral("SetPermissionInfo"), appId, permissionGroup, permissionId, value);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pcall, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher, permissionGroup, permissionId](QDBusPendingCallWatcher *watch){
        QDBusPendingReply<bool> reply = *watch;
        if (!reply.isError() && !reply.value()) {
            Q_EMIT permissionInfoReset(permissionGroup, permissionId);
        } else if (!reply.isError() && reply.value()) {
            this->getPermissionInfo();
        } else {
            qDebug() << "setPermissionInfo ==> value: " << reply.value() << "ERROR: " << reply.error();
        }
        watcher->deleteLater();
    });
}

void PrivacySecurityDBusProxy::getPermissionEnable(const QString &permissionGroup, const QString &permissionId)
{
    QList<QVariant> argumentList;
    argumentList << permissionGroup << permissionId;
    m_privacyInter->callWithCallback(QStringLiteral("GetPermissionEnable"), argumentList, this, SIGNAL(permissionEnableLoadFinished(bool)));
}

void PrivacySecurityDBusProxy::setPermissionEnable(const QString &permissionGroup, const QString &permissionId, bool enable)
{
    QDBusPendingCall pcall = m_privacyInter->asyncCall(QStringLiteral("SetPermissionEnable"), permissionGroup, permissionId, enable);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pcall, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher, permissionGroup, permissionId](QDBusPendingCallWatcher *watch){
        QDBusPendingReply<bool> reply = *watch;
        if (!reply.isError() && !reply.value()) {
            Q_EMIT permissionEnableReset(permissionGroup, permissionId);
        } else {
            qDebug() << "setPermissionEnable ==> value: " << reply.value() << "ERROR: " << reply.error();
        }
        watcher->deleteLater();
    });
}
