/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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

#ifndef ZIGBEECLUSTERIASWD_H
#define ZIGBEECLUSTERIASWD_H

#include <QObject>

#include "zcl/zigbeecluster.h"
#include "zcl/zigbeeclusterreply.h"

class ZigbeeNode;
class ZigbeeNetwork;
class ZigbeeNodeEndpoint;
class ZigbeeNetworkReply;

class ZigbeeClusterIasWd: public ZigbeeCluster
{
    Q_OBJECT

    friend class ZigbeeNode;
    friend class ZigbeeNetwork;

public:
    enum Attribute {
        AttributeMaxDuration = 0x0000,
    };
    Q_ENUM(Attribute)

    enum WarningMode {
        WarningModeStop = 0x00,
        WarningModeBurglar = 0x10,
        WarningModeFire = 0x20,
        WarningModeEmergency = 0x30,
        WarningModePolicePanic = 0x40,
        WarningModeFirePanic = 0x50,
        WarningModeEmergencyPanic = 0x60,
    };
    Q_ENUM(WarningMode)

    enum SirenLevel {
        SirenLevelLow = 0x00,
        SirenLevelMedium = 0x01,
        SirenLevelHigh = 0x02,
        SirenLevelVeryHigh = 0x03
    };
    Q_ENUM(SirenLevel)

    enum StrobeLevel {
        StrobeLevelLow = 0x00,
        StrobeLevelMedium = 0x01,
        StrobeLevelHigh = 0x02,
        StrobeLevelVeryHigh = 0x03
    };
    Q_ENUM(StrobeLevel)

    enum SquawkMode {
        SquawkModeSystemArmed = 0x00,
        SquawkModeSystemDisarmed = 0x10
    };
    Q_ENUM(SquawkMode)

    enum SquawkLevel {
        SquawkLevelLow = 0x00,
        SquawkLevelMedium = 0x01,
        SquawkLevelHigh = 0x02,
        SquawkLevelVeryHigh = 0x03
    };
    Q_ENUM(SquawkLevel)

    enum ServerCommand {
        ServerCommandStartWarning = 0x00, // M
        ServerCommandSquawk = 0x01 // M
    };
    Q_ENUM(ServerCommand)

    explicit ZigbeeClusterIasWd(ZigbeeNetwork *network, ZigbeeNode *node, ZigbeeNodeEndpoint *endpoint, Direction direction, QObject *parent = nullptr);

    quint16 maxDuration() const;
    ZigbeeClusterReply* setMaxDuration(quint16 maxDuration);

    ZigbeeClusterReply* startWarning(WarningMode warningMode, bool strobeEnabled, SirenLevel sirenLevel, quint16 duration, quint8 strobeDutyCycle, StrobeLevel strobeLevel);
    ZigbeeClusterReply* squawk(SquawkMode squawkMode, bool strobeEnabled, SquawkLevel squawkLevel);

signals:
    void maxDurationChanged(quint16 maxDuration);

private:
    void setAttribute(const ZigbeeClusterAttribute &attribute) override;

private:
    quint16 m_maxDuration = 240;
};

#endif // ZIGBEECLUSTERIASWD_H
