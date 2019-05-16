#ifndef ZIGBEENETWORK_H
#define ZIGBEENETWORK_H

#include <QObject>
#include <QSettings>

#include "zigbeenode.h"
#include "zigbeesecurityconfiguration.h"

class ZigbeeNetwork : public ZigbeeNode
{
    Q_OBJECT

public:
    enum ControllerType {
        ControlerTypeNxp
    };
    Q_ENUM(ControllerType)

    enum State {
        StateUninitialized,
        StateDisconnected,
        StateStarting,
        StateRunning,
        StateStopping
    };
    Q_ENUM(State)

    enum Error {
        ErrorNoError,
        ErrorHardwareUnavailable,
        ErrorZigbeeError
    };
    Q_ENUM(Error)

    explicit ZigbeeNetwork(ControllerType controllerType, QObject *parent = nullptr);

    State state() const;
    ControllerType controlerType() const;

    Error error() const;

    QString settingsFilenName() const;
    void setSettingsFileName(const QString &settingsFileName);

    // Serial port configuration
    QString serialPortName() const;
    void setSerialPortName(const QString &serialPortName);

    qint32 serialBaudrate() const;
    void setSerialBaudrate(qint32 baudrate);

    // Network configurations
    quint64 extendedPanId() const;
    void setExtendedPanId(quint64 extendedPanId);

    uint channel() const;
    void setChannel(uint channel);

    ZigbeeSecurityConfiguration securityConfiguration() const;
    void setSecurityConfiguration(const ZigbeeSecurityConfiguration &securityConfiguration);

    QList<ZigbeeNode *> nodes() const;

    ZigbeeNode *coordinatorNode() const;

    ZigbeeNode *getZigbeeNode(quint16 shortAddress) const;
    ZigbeeNode *getZigbeeNode(const ZigbeeAddress &address) const;

    bool hasNode(quint16 shortAddress) const;
    bool hasNode(const ZigbeeAddress &address) const;


private:
    ControllerType m_controllerType = ControlerTypeNxp;
    State m_state = StateUninitialized;
    Error m_error = ErrorNoError;

    // Serial port configuration
    QString m_serialPortName = "/dev/ttyUSB0";
    qint32 m_serialBaudrate = 115200;

    // Network configurations
    quint64 m_extendedPanId = 0;
    uint m_channel = 0;
    ZigbeeSecurityConfiguration m_securityConfiguration;
    ZigbeeNode::NodeType m_nodeType = ZigbeeNode::NodeTypeCoordinator;

    QString m_settingsFileName = "/etc/nymea/nymea-zigbee.conf";
    QList<ZigbeeNode *> m_nodes;
    QList<ZigbeeNode *> m_uninitializedNodes;

    void addNodeInternally(ZigbeeNode *node);
    void removeNodeInternally(ZigbeeNode *node);

protected:
    void saveNetwork();
    void loadNetwork();
    void clearSettings();

    void saveNode(ZigbeeNode *node);
    void removeNodeFromSettings(ZigbeeNode *node);

    void addNode(ZigbeeNode *node);
    void addUnitializedNode(ZigbeeNode *node);
    void removeNode(ZigbeeNode *node);

    ZigbeeNode *createNode();

    void setState(State state);
    void setError(Error error);

signals:
    void settingsFileNameChanged(const QString &settingsFileName);
    void serialPortNameChanged(const QString &serialPortName);
    void serialBaudrateChanged(qint32 serialBaudrate);

    void extendedPanIdChanged(quint64 extendedPanId);
    void channelChanged(uint channel);
    void securityConfigurationChanged(const ZigbeeSecurityConfiguration &securityConfiguration);

    void nodeAdded(ZigbeeNode *node);
    void nodeRemoved(ZigbeeNode *node);

    void permitJoiningChanged(bool permitJoining);
    void stateChanged(State state);
    void errorOccured(Error error);

private slots:
    void onNodeStateChanged(ZigbeeNode::State state);
    void onNodeClusterAttributeChanged(ZigbeeCluster *cluster, const ZigbeeClusterAttribute &attribute);

public slots:
    virtual void startNetwork() = 0;
    virtual void stopNetwork() = 0;
    virtual void factoryResetNetwork() = 0;

};

#endif // ZIGBEENETWORK_H
