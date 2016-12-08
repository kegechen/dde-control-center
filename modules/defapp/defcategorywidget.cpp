#include "defcategorywidget.h"
#include "nextpagewidget.h"
#include "settingsgroup.h"
#include "nextpagewidget.h"
#include "switchwidget.h"

using namespace dcc;

DefCategoryWidget::DefCategoryWidget(const QString &name, QWidget *parent)
    : ModuleWidget()
{
    Q_UNUSED(parent);
    m_userGroup = new SettingsGroup;
    m_addWidget  = new DefCategoryAddWidget(this);
    m_headWidget = new SettingsHead;
    m_headWidget->setTitle(name);
    m_userGroup->insertItem(0, m_headWidget);
    m_userGroup->insertItem(1, m_addWidget);
    m_centeralLayout->addWidget(m_userGroup);

    connect(m_addWidget,  &DefCategoryAddWidget::addUserItem,  this, &DefCategoryWidget::requestAddUserApp);
    connect(m_headWidget, &SettingsHead::editChanged, this, &DefCategoryWidget::slotEditMode);
    connect(m_addWidget, &DefCategoryAddWidget::requestFrameAutoHide, this, &DefCategoryWidget::requestFrameAutoHide);
}

void DefCategoryWidget::setCategory(Category *const category)
{
    m_category = category;
    connect(category, &Category::itemsChanged, this, &DefCategoryWidget::AppsItemChanged);
    connect(category, &Category::userItemChanged, this, &DefCategoryWidget::UserItemChanged);
    connect(category, &Category::defaultChanged, this, &DefCategoryWidget::onDefaultAppSet);
    connect(category, &Category::AdduserItem, this, &DefCategoryWidget::addUserItem);

    QList<QJsonObject> it = category->getappItem();
    for (const QJsonObject list : it) {
        addItem(list);
    }
    it = category->getuserItem();
    for (const QJsonObject list : it) {
        addUserItem(list);
    }
    onDefaultAppSet(category->getDefault());
    m_categoryName = category->getName();
    m_addWidget->setCategory(m_categoryName);
}

void DefCategoryWidget::setDefault()
{
    OptionWidget *item = qobject_cast<OptionWidget *>(sender());
    emit requestSetDefaultApp(m_category->getName(), m_valueMap.value(item));
    onDefaultAppSet(m_valueMap.value(item)["Id"].toString());
}

void DefCategoryWidget::addItem(const QJsonObject &item)
{
    m_optionWidget = new OptionWidget;
    m_optionWidget->setItem(item);
    m_optionWidget->setMime(m_category->getName());
    m_optionWidget->setFixedHeight(30);
    m_userGroup->insertItem(1, m_optionWidget);
    m_mainMap.insert(m_optionWidget->id(), m_optionWidget);
    m_valueMap.insert(m_optionWidget, item);
    connect(m_optionWidget, &OptionWidget::setDefault, this, &DefCategoryWidget::setDefault);
    connect(m_optionWidget, &OptionWidget::removeItem, this, &DefCategoryWidget::removeItem);
}

void DefCategoryWidget::addUserItem(const QJsonObject &item)
{
    m_optionWidget = new OptionWidget;
    m_optionWidget->setItem(item);
    m_optionWidget->setMime(m_category->getName());
    m_optionWidget->setUserCheck(true);
    m_optionWidget->setFixedHeight(30);
    m_userGroup->insertItem(1, m_optionWidget);
    m_userMap.insert(m_optionWidget->id(), m_optionWidget);
    m_valueMap.insert(m_optionWidget, item);
    connect(m_optionWidget, &OptionWidget::setDefault, this, &DefCategoryWidget::setDefault);
    connect(m_optionWidget, &OptionWidget::removeItem, this, &DefCategoryWidget::removeItem);
}

void DefCategoryWidget::removeItem(const QJsonObject &item)
{
    OptionWidget *w = m_valueMap.key(item);
    m_userGroup->removeItem(w);
    w->deleteLater();
    m_userMap.remove(m_userMap.key(m_valueMap.key(item)));
    m_valueMap.remove(m_valueMap.key(item));
    emit requestDelUserApp(m_categoryName, item);
}

void DefCategoryWidget::onDefaultAppSet(const QString &id)
{
    for (OptionWidget *item : m_valueMap.keys()) {
        if (item->id() == id) {
            item->setChecked(true);
        } else {
            item->setChecked(false);
        }
    }
}

void DefCategoryWidget::slotEditMode(bool edit)
{
    if (m_userMap.size() != 0) {
        for (OptionWidget *item : m_userMap.values()) {
            item->setDelete(edit);
        }
    }
}

void DefCategoryWidget::AppsItemChanged(const QList<QJsonObject> &list)
{
    for (OptionWidget *item : m_mainMap.values()) {
        m_userGroup->removeItem(item);
        item->deleteLater();
    }
    for (const QJsonObject item : list) {
        addItem(item);
    }
    onDefaultAppSet(m_category->getDefault());
}


void DefCategoryWidget::UserItemChanged(const QList<QJsonObject> &list)
{
    for (OptionWidget *item : m_userMap.values()) {
        m_userMap.remove(m_userMap.key(item));
        m_valueMap.remove(item);
        m_userGroup->removeItem(item);
        item->deleteLater();
    }

    for (const QJsonObject item : list) {
        addUserItem(item);
    }
    onDefaultAppSet(m_category->getDefault());
}
