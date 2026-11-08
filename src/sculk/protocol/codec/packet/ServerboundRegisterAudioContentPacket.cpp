// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ServerboundRegisterAudioContentPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void AudioContentRegistrationEntry::write(BinaryStream& stream) const {
    stream.writeString(mAudioContentId);
    stream.writeString(mSharedMetadata);
    stream.writeString(mServerContent);
    stream.writeString(mPlaybackContent);
}

Result<> AudioContentRegistrationEntry::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mAudioContentId));
    _SCULK_READ(stream.readString(mSharedMetadata));
    _SCULK_READ(stream.readString(mServerContent));
    return stream.readString(mPlaybackContent);
}

MinecraftPacketIds ServerboundRegisterAudioContentPacket::getId() const noexcept {
    return MinecraftPacketIds::ServerboundRegisterAudioContent;
}

std::string_view ServerboundRegisterAudioContentPacket::getName() const noexcept {
    return "ServerboundRegisterAudioContentPacket";
}

void ServerboundRegisterAudioContentPacket::write(BinaryStream& stream) const {
    stream.writeArray(mRegistrations, &AudioContentRegistrationEntry::write);
}

Result<> ServerboundRegisterAudioContentPacket::read(ReadOnlyBinaryStream& stream) {
    return stream.readArray(mRegistrations, &AudioContentRegistrationEntry::read);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ServerboundRegisterAudioContentPacket::toString() const {
    return SCULK_FORMAT_PACKET(SCULK_FORMAT_FIELD(mRegistrations));
}
#endif


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
