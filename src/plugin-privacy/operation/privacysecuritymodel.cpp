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

#include "privacysecuritymodel.h"

#include <QDebug>

#include <widgets/servicecontrolitems.h>

using namespace DCC_PRIVACY_NAMESPACE;
PrivacySecurityModel::PrivacySecurityModel(QObject *parent)
    : QObject(parent)
{
    // TODO: linglong数据不一致 做调整
    m_serviceToCategory.insert("Camera"      , ServiceCategory::Camera);
    m_serviceToCategory.insert("Microphone"  , ServiceCategory::Microphone);
    m_serviceToCategory.insert("UserFolders" , ServiceCategory::UserFolders);
    m_serviceToCategory.insert("Calendar"    , ServiceCategory::Calendar);
    m_serviceToCategory.insert("Screenshots" , ServiceCategory::Screenshots);

    m_moduleInfo = {
        DCC_PRIVACY_NAMESPACE::DATE("camera",tr("Camera"), "dcc_camera", ServiceCategory::Camera),
        DCC_PRIVACY_NAMESPACE::DATE("microphone",tr("Microphone"), "dcc_microphone", ServiceCategory::Microphone),
        DCC_PRIVACY_NAMESPACE::DATE("userFolders",tr("User Folders"), "folder", ServiceCategory::UserFolders),
        DCC_PRIVACY_NAMESPACE::DATE("calendar",tr("Calendar"), "dde-calendar", ServiceCategory::Calendar),
        DCC_PRIVACY_NAMESPACE::DATE("screenCapture",tr("Screen Capture"), "deepin-screen-recorder", ServiceCategory::Screenshots),
    };

    initServiceItems();
}

QList<DATE> PrivacySecurityModel::getModuleInfo()
{
    return m_moduleInfo;
}

ServiceCategory PrivacySecurityModel::getCategory(const QString &serviceName)
{
    return m_serviceToCategory[serviceName];
}

void PrivacySecurityModel::initServiceItems()
{
    qDebug() << "Get serviceItems: " << m_serviceToCategory.values().size();
    for (ServiceCategory cate : m_serviceToCategory.values()) {
        qDebug() << "set Cate: " << cate;
        m_groupService.append(new ServiceControlItems(cate, this));
    }
    qDebug() << m_groupService.size();
}

QString PrivacySecurityModel::getDaemonDefineName(ServiceCategory category)
{
    return m_serviceToCategory.key(category);
}

ServiceControlItems *PrivacySecurityModel::getServiceItem(const QString &daemonDefineName)
{
    for (ServiceControlItems *item : m_groupService) {
        if (item->currentCategory() == m_serviceToCategory[daemonDefineName]) {
            return item;
        }
    }
    return nullptr;
}

ServiceControlItems *PrivacySecurityModel::getServiceItem(ServiceCategory category)
{
    for (ServiceControlItems *item : m_groupService) {
        if (item->currentCategory() == category) {
            return item;
        }
    }
    return nullptr;
}

