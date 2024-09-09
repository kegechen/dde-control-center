//SPDX-FileCopyrightText: 2018 - 2023 UnionTech Software Technology Co., Ltd.
//
//SPDX-License-Identifier: GPL-3.0-or-later
#ifndef POWERINTERFACE_H
#define POWERINTERFACE_H

#include "powermodel.h"
#include "powerworker.h"
#include "poweroperatormodel.h"

class PowerInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PowerModel *model READ getModel NOTIFY powerModelChanged)
    Q_PROPERTY(PowerWorker *worker READ getWorker NOTIFY powerWorkerChanged)

    Q_PROPERTY(PowerOperatorModel *powerLidModel MEMBER m_powerLidClosedOperatorModel)
    Q_PROPERTY(PowerOperatorModel *powerPressModel MEMBER m_powerPressedOperatorModel)
    Q_PROPERTY(PowerOperatorModel *batteryLidModel MEMBER m_batteryLidClosedOperatorModel)
    Q_PROPERTY(PowerOperatorModel *batteryPressModel MEMBER m_batteryPressedOperatorModel)
public:
    explicit PowerInterface(QObject *parent = nullptr);
    PowerModel *getModel() const { return m_model; };
    PowerWorker *getWorker() const { return m_worker; };
signals:
    void powerModelChanged(PowerModel *model);
    void powerWorkerChanged(PowerWorker *worker);

private:
    PowerModel *m_model;
    PowerWorker *m_worker;

    PowerOperatorModel *m_powerLidClosedOperatorModel;
    PowerOperatorModel *m_powerPressedOperatorModel;
    PowerOperatorModel *m_batteryLidClosedOperatorModel;
    PowerOperatorModel *m_batteryPressedOperatorModel;
};

#endif // POWERINTERFACE_H