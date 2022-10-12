#ifndef WACOMMODULE_H
#define WACOMMODULE_H

#include <QObject>
#include "interface/pagemodule.h"

class WacomModel;

class WacomModule : public DCC_NAMESPACE::PageModule
{
    Q_OBJECT
public:
    explicit WacomModule(QObject *parent = nullptr);
    ~WacomModule() override {}

    QWidget *initModeModule(ModuleObject *module);
    QWidget *initPressureModule(ModuleObject *module);

private:
    WacomModel *m_model;
};

#endif // WACOMMODULE_H
