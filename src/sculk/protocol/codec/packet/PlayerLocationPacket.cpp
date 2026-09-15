// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/PlayerLocationPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds PlayerLocationPacket::getId() const noexcept { return MinecraftPacketIds::PlayerLocation; }

std::string_view PlayerLocationPacket::getName() const noexcept { return "PlayerLocationPacket"; }

void PlayerLocationPacket::write(BinaryStream& stream) const {
    stream.writeVarInt64(mActorUniqueId);
    stream.writeEnum(mType, &BinaryStream::writeUnsignedVarInt);
    stream.writeEnum(mType, &BinaryStream::writeVarInt);
    if (mType == Type::PlayerLocationCoordinates) {
        mPosition.write(stream);
    }
}

Result<> PlayerLocationPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readVarInt64(mActorUniqueId));
    std::uint32_t variant{};
    _SCULK_READ(stream.readUnsignedVarInt(variant));
    _SCULK_READ(stream.readEnum(mType, &ReadOnlyBinaryStream::readVarInt));
    if (variant > 1 || variant != static_cast<std::uint32_t>(mType)) {
        return error_utils::makeError("Invalid player location variant");
    }
    mPosition = {};
    if (mType == Type::PlayerLocationCoordinates) {
        return mPosition.read(stream);
    }
    return {};
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string PlayerLocationPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mType),
        SCULK_FORMAT_FIELD(mActorUniqueId),
        SCULK_FORMAT_FIELD(mPosition)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
