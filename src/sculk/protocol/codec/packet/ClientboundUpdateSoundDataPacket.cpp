// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ClientboundUpdateSoundDataPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds ClientboundUpdateSoundDataPacket::getId() const noexcept {
    return MinecraftPacketIds::ClientboundUpdateSoundData;
}

std::string_view ClientboundUpdateSoundDataPacket::getName() const noexcept {
    return "ClientboundUpdateSoundDataPacket";
}

void ClientboundUpdateSoundDataPacket::SoundUpdate::write(BinaryStream& stream) const {
    stream.writeEnum(mType, &BinaryStream::writeUnsignedVarInt);
    if (mType >= Type::SetVolume && mType <= Type::SeekTo) {
        stream.writeFloat(mValue);
    }
    if (mType == Type::Fade) {
        stream.writeFloat(mTargetVolume);
    }
}

Result<> ClientboundUpdateSoundDataPacket::SoundUpdate::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mType, &ReadOnlyBinaryStream::readUnsignedVarInt));
    if (mType > Type::Resume) {
        return error_utils::makeError("Invalid sound update variant");
    }
    mValue        = 0;
    mTargetVolume = 0;
    if (mType >= Type::SetVolume && mType <= Type::SeekTo) {
        _SCULK_READ(stream.readFloat(mValue));
    }
    if (mType == Type::Fade) {
        _SCULK_READ(stream.readFloat(mTargetVolume));
    }
    return {};
}

void ClientboundUpdateSoundDataPacket::write(BinaryStream& stream) const {
    stream.writeUnsignedInt64(mHandle);
    stream.writeOptional(mStop, &SoundUpdate::write);
    stream.writeOptional(mSetVolume, &SoundUpdate::write);
    stream.writeOptional(mSetPitch, &SoundUpdate::write);
    stream.writeOptional(mFade, &SoundUpdate::write);
    stream.writeOptional(mSeekTo, &SoundUpdate::write);
    stream.writeOptional(mPause, &SoundUpdate::write);
    stream.writeOptional(mResume, &SoundUpdate::write);
}

Result<> ClientboundUpdateSoundDataPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readUnsignedInt64(mHandle));
    _SCULK_READ(stream.readOptional(mStop, &SoundUpdate::read));
    _SCULK_READ(stream.readOptional(mSetVolume, &SoundUpdate::read));
    _SCULK_READ(stream.readOptional(mSetPitch, &SoundUpdate::read));
    _SCULK_READ(stream.readOptional(mFade, &SoundUpdate::read));
    _SCULK_READ(stream.readOptional(mSeekTo, &SoundUpdate::read));
    _SCULK_READ(stream.readOptional(mPause, &SoundUpdate::read));
    _SCULK_READ(stream.readOptional(mResume, &SoundUpdate::read));
    return {};
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ClientboundUpdateSoundDataPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mHandle),
        SCULK_FORMAT_FIELD(mStop),
        SCULK_FORMAT_FIELD(mSetVolume),
        SCULK_FORMAT_FIELD(mSetPitch),
        SCULK_FORMAT_FIELD(mFade),
        SCULK_FORMAT_FIELD(mSeekTo),
        SCULK_FORMAT_FIELD(mPause),
        SCULK_FORMAT_FIELD(mResume)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
