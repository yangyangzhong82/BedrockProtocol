// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ClientboundStonecutterSetRecipePacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds ClientboundStonecutterSetRecipePacket::getId() const noexcept {
    return MinecraftPacketIds::ClientboundStonecutterSetRecipe;
}

std::string_view ClientboundStonecutterSetRecipePacket::getName() const noexcept {
    return "ClientboundStonecutterSetRecipePacket";
}

void ClientboundStonecutterSetRecipePacket::write(BinaryStream& stream) const {
    stream.writeVarInt64(mPlayerId);
    stream.writeByte(mContainerId);
    stream.writeVarInt(mRecipeIndex);
}

Result<> ClientboundStonecutterSetRecipePacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readVarInt64(mPlayerId));
    _SCULK_READ(stream.readByte(mContainerId));
    return stream.readVarInt(mRecipeIndex);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ClientboundStonecutterSetRecipePacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mPlayerId),
        SCULK_FORMAT_FIELD(mContainerId),
        SCULK_FORMAT_FIELD(mRecipeIndex)
    );
}
#endif


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
