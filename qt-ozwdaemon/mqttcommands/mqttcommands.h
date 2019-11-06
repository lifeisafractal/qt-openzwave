#ifndef MQTTCOMMANDS_H
#define MQTTCOMMANDS_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttSubscription>

#include <qt-openzwave/qtopenzwave.h>
#include <qt-openzwave/qtozwmanager.h>
#include <qt-openzwave/qtozwvalueidmodel.h>
#include "mqttpublisher.h"



class mqttpublisher;

class MqttCommand : public QObject {
    Q_OBJECT
public:
    void Setup(QMqttSubscription *);
    void messageReceived(QMqttMessage msg);
    virtual bool processMessage(QJsonDocument) = 0;
    virtual QString GetCommand() = 0;
signals:
    void sendCommandUpdate(QString, QJsonObject);
protected:
    MqttCommand(QObject *parent = nullptr);
    QTOZWManager *getOZWManager();
    mqttpublisher *getMqttPublisher();
    bool checkNode(QJsonDocument, QString);
    bool checkValue(QJsonDocument, QString);
    QVariant getValueData(quint64, QTOZW_ValueIds::ValueIdColumns);
    bool setValue(quint64, QVariant);
    QVector<QString> m_requiredStringFields;
    QVector<QString> m_requiredIntFields;
    QVector<QString> m_requiredBoolFields;

private:
    QMqttSubscription *m_subscription;
};


typedef MqttCommand* (*pfnCreateCommand_t)(QObject *);


class MqttCommands : public QObject {
    Q_OBJECT
public:
    MqttCommands(QObject *parent = nullptr);
    void Register(QString command, pfnCreateCommand_t _create);
    void setupCommands();
    void setupSubscriptions(QMqttClient *, QString);
private:
    QMap<QString, pfnCreateCommand_t> m_commands;
};


#endif
