// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/SetPassengerOfBlockPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds SetPassengerOfBlockPacket::getId() const noexcept { return MinecraftPacketIds::SetPassengerOfBlock; }

std::string_view SetPassengerOfBlockPacket::getName() const noexcept { return "SetPassengerOfBlockPacket"; }

void SetPassengerOfBlockPacket::write(BinaryStream& stream) const {
    stream.writeVarInt64(mPassenger);
    stream.writeOptional(mPassengerOfBlockData, &PassengerOfBlockArguments::write);
}

Result<> SetPassengerOfBlockPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readVarInt64(mPassenger));
    return stream.readOptional(mPassengerOfBlockData, &PassengerOfBlockArguments::read);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string SetPassengerOfBlockPacket::toString() const {
    return SCULK_FORMAT_PACKET(SCULK_FORMAT_FIELD(mPassenger), SCULK_FORMAT_FIELD(mPassengerOfBlockData));
}
#endif


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
