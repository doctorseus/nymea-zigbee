/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea-zigbee.
* This project including source code and documentation is protected by copyright law, and
* remains the property of nymea GmbH. All rights, including reproduction, publication,
* editing and translation, are reserved. The use of this project is subject to the terms of a
* license agreement to be concluded with nymea GmbH in accordance with the terms
* of use of nymea GmbH, available under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the terms of the GNU
* Lesser General Public License as published by the Free Software Foundation; version 3.
* this project is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License along with this project.
* If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under contact@nymea.io
* or see our FAQ/Licensing Information on https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ZIGBEENETWORK_H
#define ZIGBEENETWORK_H

#include <QObject>
#include <QSettings>

#include "zigbeenode.h"
#include "zigbeechannelmask.h"
#include "zigbeebridgecontroller.h"
#include "zigbeesecurityconfiguration.h"

class ZigbeeNetwork : public QObject
{
    Q_OBJECT

public:
    enum State {
        StateUninitialized,
        StateOffline,
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

    explicit ZigbeeNetwork(QObject *parent = nullptr);

    State state() const;

    Error error() const;

    QString settingsFilenName() const;
    void setSettingsFileName(const QString &settingsFileName);

    virtual ZigbeeBridgeController *bridgeController() const = 0;

    // Serial port configuration
    QString serialPortName() const;
    void setSerialPortName(const QString &serialPortName);

    qint32 serialBaudrate() const;
    void setSerialBaudrate(qint32 baudrate);

    // Network configurations
    quint16 panId();
    void setPanId(quint16 panId);

    quint64 extendedPanId() const;
    void setExtendedPanId(quint64 extendedPanId);

    quint32 channel() const;
    void setChannel(quint32 channel);

    ZigbeeChannelMask channelMask() const;
    void setChannelMask(const ZigbeeChannelMask &channelMask);

    ZigbeeSecurityConfiguration securityConfiguration() const;
    void setSecurityConfiguration(const ZigbeeSecurityConfiguration &securityConfiguration);

    bool permitJoining() const;
    void setPermitJoining(bool permitJoining);


    // Network nodes
    QList<ZigbeeNode *> nodes() const;

    ZigbeeNode *coordinatorNode() const;

    ZigbeeNode *getZigbeeNode(quint16 shortAddress) const;
    ZigbeeNode *getZigbeeNode(const ZigbeeAddress &address) const;

    bool hasNode(quint16 shortAddress) const;
    bool hasNode(const ZigbeeAddress &address) const;

private:
    State m_state = StateUninitialized;

    // Serial port configuration
    QString m_serialPortName = "/dev/ttyUSB0";
    qint32 m_serialBaudrate = 115200;

    // Network configurations
    quint16 m_panId = 0;
    quint64 m_extendedPanId = 0;
    quint32 m_channel = 0;
    ZigbeeChannelMask m_channelMask = ZigbeeChannelMask(ZigbeeChannelMask::ChannelConfigurationAllChannels);
    ZigbeeNode::NodeType m_nodeType = ZigbeeNode::NodeTypeCoordinator;

    QString m_settingsFileName = "/etc/nymea/nymea-zigbee.conf";
    QList<ZigbeeNode *> m_nodes;
    QList<ZigbeeNode *> m_uninitializedNodes;

    void addNodeInternally(ZigbeeNode *node);
    void removeNodeInternally(ZigbeeNode *node);

protected:
    Error m_error = ErrorNoError;
    ZigbeeNode *m_coordinatorNode = nullptr;
    bool m_permitJoining = false;
    ZigbeeSecurityConfiguration m_securityConfiguration;

    virtual ZigbeeNode *createNode(QObject *parent) = 0;
    virtual void setPermitJoiningInternal(bool permitJoining) = 0;

    void saveNetwork();
    void loadNetwork();
    void clearSettings();

    void saveNode(ZigbeeNode *node);
    void removeNodeFromSettings(ZigbeeNode *node);

    void addNode(ZigbeeNode *node);
    void addUnitializedNode(ZigbeeNode *node);
    void removeNode(ZigbeeNode *node);

    void setState(State state);
    void setError(Error error);

    bool networkConfigurationAvailable() const;

    virtual ZigbeeNetworkReply *sendRequest(const ZigbeeNetworkRequest &request) = 0;

    // Network reply methods
    ZigbeeNetworkReply *createNetworkReply(const ZigbeeNetworkRequest &request = ZigbeeNetworkRequest());
    void setReplyResponseData(ZigbeeNetworkReply *reply, const QByteArray &responseData);
    void setReplyResponseError(ZigbeeNetworkReply *reply, Zigbee::ZigbeeStatus zigbeeStatus = Zigbee::ZigbeeStatusSuccess);
    void finishNetworkReply(ZigbeeNetworkReply *reply, ZigbeeNetworkReply::Error error = ZigbeeNetworkReply::ErrorNoError);

signals:
    void settingsFileNameChanged(const QString &settingsFileName);
    void serialPortNameChanged(const QString &serialPortName);
    void serialBaudrateChanged(qint32 serialBaudrate);

    void panIdChanged(quint16 panId);
    void extendedPanIdChanged(quint64 extendedPanId);
    void channelChanged(uint channel);
    void channelMaskChanged(const ZigbeeChannelMask &channelMask);
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
    virtual void reset() = 0;
    virtual void factoryResetNetwork() = 0;

};

QDebug operator<<(QDebug debug, ZigbeeNetwork *network);


#endif // ZIGBEENETWORK_H
