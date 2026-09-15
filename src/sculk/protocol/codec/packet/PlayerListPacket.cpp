// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/PlayerListPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void PlayerListEntry::write(BinaryStream& stream) const {
    mUUID.write(stream);
    stream.writeVarInt64(mActorUniqueId);
    stream.writeString(mPlayerName);
    stream.writeString(mXuid);
    stream.writeString(mPlatformChatId);
    stream.writeSignedInt(mBuildPlatform);
    mSerializedSkin.write(stream);
    stream.writeBool(mIsTeacher);
    stream.writeBool(mIsHost);
    stream.writeBool(mIsSubClient);
    stream.writeSignedInt(mColor);
}

Result<> PlayerListEntry::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mUUID.read(stream));
    _SCULK_READ(stream.readVarInt64(mActorUniqueId));
    _SCULK_READ(stream.readString(mPlayerName));
    _SCULK_READ(stream.readString(mXuid));
    _SCULK_READ(stream.readString(mPlatformChatId));
    _SCULK_READ(stream.readSignedInt(mBuildPlatform));
    _SCULK_READ(mSerializedSkin.read(stream));
    _SCULK_READ(stream.readBool(mIsTeacher));
    _SCULK_READ(stream.readBool(mIsHost));
    _SCULK_READ(stream.readBool(mIsSubClient));
    return stream.readSignedInt(mColor);
}

MinecraftPacketIds PlayerListPacket::getId() const noexcept { return MinecraftPacketIds::PlayerList; }

std::string_view PlayerListPacket::getName() const noexcept { return "PlayerListPacket"; }

void PlayerListPacket::write(BinaryStream& stream) const {
    stream.writeArray(mPlayerEntryList, [](const PlayerListEntry& entry, BinaryStream& stream) {
        stream.writeUnsignedVarInt(entry.mAction == ActionType::Add ? 1 : 0);
        stream.writeEnum(entry.mAction, &BinaryStream::writeByte);
        if (entry.mAction == ActionType::Add) {
            entry.write(stream);
        } else {
            entry.mUUID.write(stream);
        }
    });
}

Result<> PlayerListPacket::read(ReadOnlyBinaryStream& stream) {
    return stream.readArray(mPlayerEntryList, [](PlayerListEntry& entry, ReadOnlyBinaryStream& stream) -> Result<> {
        std::uint32_t variant{};
        _SCULK_READ(stream.readUnsignedVarInt(variant));
        _SCULK_READ(stream.readEnum(entry.mAction, &ReadOnlyBinaryStream::readByte));
        if (variant > 1 || static_cast<std::uint32_t>(entry.mAction) != 1 - variant) {
            return error_utils::makeError("Invalid player list entry variant");
        }
        if (entry.mAction == ActionType::Add) {
            return entry.read(stream);
        }
        return entry.mUUID.read(stream);
    });
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string PlayerListPacket::toString() const { return SCULK_FORMAT_PACKET(SCULK_FORMAT_FIELD(mPlayerEntryList)); }
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
