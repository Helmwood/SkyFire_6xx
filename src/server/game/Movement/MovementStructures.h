/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MOVEMENT_STRUCTURES_H
#define _MOVEMENT_STRUCTURES_H

#include "Opcodes.h"
#include "Object.h"

class ByteBuffer;
class Unit;

enum MovementStatusElements
{
    MSEForcesCount,
    MSEHasCounter,
    MSEHasMovementFlags,
    MSEHasMovementFlags2,
    MSEHasTimestamp,
    MSEHasOrientation,
    MSEHasTransportData,
    MSEHasTransportPrevTime,
    MSEHasTransportVehicleId,
    MSEHasPitch,
    MSEHasFallData,
    MSEHasFallDirection,
    MSEHasSplineElevation,
    MSEHasSpline,

    MSEForces,
    MSECounter,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSETransportPrevTime,
    MSETransportVehicleId,
    MSEPitch,
    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,
    MSESplineElevation,
    MSEGuid128,
    MSETransportGuid128,

    MSEUintCount,

    // Special
    MSEZeroBit,         // writes bit value 1 or skips read bit
    MSEOneBit,          // writes bit value 0 or skips read bit
    MSEEnd,             // marks end of parsing
    MSEExtraElement,    // Used to signalize reading into ExtraMovementStatusElement, element sequence inside it is declared as separate array
                        // Allowed internal elements are: GUID markers (not transport), MSEExtraFloat, MSEExtraInt8
    MSEExtraFloat,
    MSEExtraInt8,
};

namespace Movement
{
    class PacketSender;

    class ExtraMovementStatusElement
    {
        friend class PacketSender;

    public:
        ExtraMovementStatusElement(MovementStatusElements const* elements) : _elements(elements), _index(0) { }

        void ReadNextElement(ByteBuffer& packet);
        void WriteNextElement(ByteBuffer& packet);

        struct
        {
            ObjectGuid guid;
            float floatData;
            int8  byteData;
        } Data;

    protected:
        void ResetIndex() { _index = 0; }

    private:
        MovementStatusElements const* _elements;
        uint32 _index;
    };

    class PacketSender
    {
    public:
        PacketSender(Unit* unit, Opcodes serverControl, Opcodes playerControl, Opcodes broadcast = SMSG_PLAYER_MOVE, ExtraMovementStatusElement* extras = NULL);

        void Send() const;

    private:
        ExtraMovementStatusElement* _extraElements;
        Unit* _unit;
        Opcodes _selfOpcode;
        Opcodes _broadcast;
    };

    bool PrintInvalidSequenceElement(MovementStatusElements element, char const* function);
}

MovementStatusElements const* GetMovementStatusElementsSequence(Opcodes opcode);

#endif
