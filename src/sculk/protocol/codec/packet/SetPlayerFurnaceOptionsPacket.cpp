// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/SetPlayerFurnaceOptionsPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds SetPlayerFurnaceOptionsPacket::getId() const noexcept {
    return MinecraftPacketIds::SetPlayerFurnaceOptions;
}

std::string_view SetPlayerFurnaceOptionsPacket::getName() const noexcept { return "SetPlayerFurnaceOptionsPacket"; }

void SetPlayerFurnaceOptionsPacket::write(BinaryStream& stream) const {
    stream.writeEnum(mFurnaceType, &BinaryStream::writeByte);
    stream.writeEnum(mLeftFurnaceTab, &BinaryStream::writeVarInt);
    stream.writeBool(mFiltering);
    stream.writeEnum(mLayout, &BinaryStream::writeVarInt);
}

Result<> SetPlayerFurnaceOptionsPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mFurnaceType, &ReadOnlyBinaryStream::readByte));
    _SCULK_READ(stream.readEnum(mLeftFurnaceTab, &ReadOnlyBinaryStream::readVarInt));
    _SCULK_READ(stream.readBool(mFiltering));
    return stream.readEnum(mLayout, &ReadOnlyBinaryStream::readVarInt);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string SetPlayerFurnaceOptionsPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mFurnaceType),
        SCULK_FORMAT_FIELD(mLeftFurnaceTab),
        SCULK_FORMAT_FIELD(mFiltering),
        SCULK_FORMAT_FIELD(mLayout)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
