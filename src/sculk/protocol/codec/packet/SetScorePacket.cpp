// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/SetScorePacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

namespace {
constexpr std::string_view scoreNames[] = {"remove", "changeplayer", "changeentity", "changefakeplayer"};
}

void SetScorePacket::ScoreInfo::write(BinaryStream& stream) const {
    const auto type = static_cast<std::uint32_t>(mIdentityType);
    stream.writeUnsignedVarInt(type);
    stream.writeString(type < 4 ? scoreNames[type] : "");
    stream.writeVarInt64(mScoreboardId);
    if (mIdentityType == IdentityType::Invalid) {
        stream.writeOptional(mObjectiveName, &BinaryStream::writeString);
        return;
    }
    stream.writeString(mObjectiveName.value_or(""));
    stream.writeSignedInt(mScoreValue);
    if (mIdentityType == IdentityType::FakePlayer) {
        stream.writeString(mFakePlayerName);
    } else {
        stream.writeVarInt64(mActorUniqueId);
    }
}

Result<> SetScorePacket::ScoreInfo::read(ReadOnlyBinaryStream& stream) {
    std::uint32_t type{};
    _SCULK_READ(stream.readUnsignedVarInt(type));
    if (type >= 4) {
        return error_utils::makeError("Invalid score variant");
    }
    std::string name{};
    _SCULK_READ(stream.readString(name));
    if (name != scoreNames[type]) {
        return error_utils::makeError("Mismatched score variant name");
    }
    mIdentityType = static_cast<IdentityType>(type);
    _SCULK_READ(stream.readVarInt64(mScoreboardId));
    mScoreValue    = 0;
    mActorUniqueId = 0;
    mFakePlayerName.clear();
    if (mIdentityType == IdentityType::Invalid) {
        return stream.readOptional(mObjectiveName, &ReadOnlyBinaryStream::readString);
    }
    _SCULK_READ(stream.readString(mObjectiveName.emplace()));
    _SCULK_READ(stream.readSignedInt(mScoreValue));
    if (mIdentityType == IdentityType::FakePlayer) {
        return stream.readString(mFakePlayerName);
    }
    return stream.readVarInt64(mActorUniqueId);
}

MinecraftPacketIds SetScorePacket::getId() const noexcept { return MinecraftPacketIds::SetScore; }

std::string_view SetScorePacket::getName() const noexcept { return "SetScorePacket"; }

void SetScorePacket::write(BinaryStream& stream) const { stream.writeArray(mScoresInfo, &ScoreInfo::write); }

Result<> SetScorePacket::read(ReadOnlyBinaryStream& stream) { return stream.readArray(mScoresInfo, &ScoreInfo::read); }

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string SetScorePacket::toString() const { return SCULK_FORMAT_PACKET(SCULK_FORMAT_FIELD(mScoresInfo)); }
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
