#ifndef SAFEUPDATEITEM_H
#define SAFEUPDATEITEM_H

#include "updatesettingitem.h"

namespace dcc {
namespace update {


class SafeUpdateItem: public UpdateSettingItem
{
    Q_OBJECT
public:
    explicit SafeUpdateItem(QWidget *parent = nullptr);

    void init();
};

}
}

#endif // SAFEUPDATEITEM_H
