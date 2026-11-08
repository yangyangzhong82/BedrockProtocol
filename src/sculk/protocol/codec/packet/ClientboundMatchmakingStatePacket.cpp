// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ClientboundMatchmakingStatePacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void MatchmakingStateOptions::write(BinaryStream& stream) const {
    stream.writeOptional(mTriggeringPlayerName, &BinaryStream::writeString);
    stream.writeOptional(mTriggeredByLocalPlayer, &BinaryStream::writeBool);
}

Result<> MatchmakingStateOptions::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readOptional(mTriggeringPlayerName, &ReadOnlyBinaryStream::readString));
    return stream.readOptional(mTriggeredByLocalPlayer, &ReadOnlyBinaryStream::readBool);
}

MinecraftPacketIds ClientboundMatchmakingStatePacket::getId() const noexcept {
    return MinecraftPacketIds::ClientboundMatchmakingState;
}

std::string_view ClientboundMatchmakingStatePacket::getName() const noexcept {
    return "ClientboundMatchmakingStatePacket";
}

void ClientboundMatchmakingStatePacket::write(BinaryStream& stream) const {
    stream.writeEnum(mState, &BinaryStream::writeByte);
    stream.writeString(mDestinationName);
    stream.writeOptional(mOptions, &MatchmakingStateOptions::write);
}

Result<> ClientboundMatchmakingStatePacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mState, &ReadOnlyBinaryStream::readByte));
    _SCULK_READ(stream.readString(mDestinationName));
    return stream.readOptional(mOptions, &MatchmakingStateOptions::read);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ClientboundMatchmakingStatePacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mState),
        SCULK_FORMAT_FIELD(mDestinationName),
        SCULK_FORMAT_FIELD(mOptions)
    );
}
#endif


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
