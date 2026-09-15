// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ResourcePackClientResponsePacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

namespace {
constexpr std::string_view responseNames[] =
    {"cancel", "downloading", "downloadingfinished", "resourcepackstackfinished"};
}

MinecraftPacketIds ResourcePackClientResponsePacket::getId() const noexcept {
    return MinecraftPacketIds::ResourcePackClientResponse;
}

std::string_view ResourcePackClientResponsePacket::getName() const noexcept {
    return "ResourcePackClientResponsePacket";
}

void ResourcePackClientResponsePacket::write(BinaryStream& stream) const {
    const auto variant = static_cast<std::uint32_t>(mResponse - 1);
    stream.writeUnsignedVarInt(variant);
    stream.writeString(variant < 4 ? responseNames[variant] : "");
    if (mResponse == Downloading) {
        stream.writeArray(mPackIds, &BinaryStream::writeString);
    }
}

Result<> ResourcePackClientResponsePacket::read(ReadOnlyBinaryStream& stream) {
    std::uint32_t variant{};
    _SCULK_READ(stream.readUnsignedVarInt(variant));
    if (variant >= 4) {
        return error_utils::makeError("Invalid resource pack response");
    }
    std::string name{};
    _SCULK_READ(stream.readString(name));
    if (name != responseNames[variant]) {
        return error_utils::makeError("Mismatched resource pack response name");
    }
    mResponse = static_cast<std::uint8_t>(variant + 1);
    mPackIds.clear();
    if (mResponse == Downloading) {
        return stream.readArray(mPackIds, &ReadOnlyBinaryStream::readString);
    }
    return {};
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ResourcePackClientResponsePacket::toString() const {
    return SCULK_FORMAT_PACKET(SCULK_FORMAT_FIELD(mResponse), SCULK_FORMAT_FIELD(mPackIds));
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
