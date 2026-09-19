// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ServerboundPackSettingChangePacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds ServerboundPackSettingChangePacket::getId() const noexcept {
    return MinecraftPacketIds::ServerboundPackSettingChange;
}

std::string_view ServerboundPackSettingChangePacket::getName() const noexcept {
    return "ServerboundPackSettingChangePacket";
}

void ServerboundPackSettingChangePacket::write(BinaryStream& stream) const {
    mPackId.write(stream);
    stream.writeString(mPackSettingName);
    stream.writeVariant(mPackSettingValue, [&stream](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, float>) {
            stream.writeFloat(value);
        } else if constexpr (std::is_same_v<T, bool>) {
            stream.writeBool(value);
        } else if constexpr (std::is_same_v<T, std::string>) {
            stream.writeString(value);
        } else {
            stream.writeArray(value, &BinaryStream::writeString);
        }
    });
}

Result<> ServerboundPackSettingChangePacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mPackId.read(stream));
    _SCULK_READ(stream.readString(mPackSettingName));
    return stream.readVariant(mPackSettingValue, [&stream](auto& value) -> Result<> {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, float>) {
            return stream.readFloat(value);
        } else if constexpr (std::is_same_v<T, bool>) {
            return stream.readBool(value);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return stream.readString(value);
        } else {
            return stream.readArray(value, &ReadOnlyBinaryStream::readString);
        }
    });
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ServerboundPackSettingChangePacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mPackId),
        SCULK_FORMAT_FIELD(mPackSettingName),
        SCULK_FORMAT_FIELD(mPackSettingValue)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
