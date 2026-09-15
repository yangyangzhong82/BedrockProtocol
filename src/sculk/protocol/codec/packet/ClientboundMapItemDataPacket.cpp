// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ClientboundMapItemDataPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds ClientboundMapItemDataPacket::getId() const noexcept {
    return MinecraftPacketIds::ClientboundMapItemData;
}

std::string_view ClientboundMapItemDataPacket::getName() const noexcept { return "ClientboundMapItemDataPacket"; }

void ClientboundMapItemDataPacket::write(BinaryStream& stream) const {
    stream.writeVarInt64(mMapId);
    stream.writeByte(mDimension);
    stream.writeBool(mIsLockedMap);
    mMapOrigin.write(stream);
    stream.writeOptional(mMapEntries, [](BinaryStream& stream, const auto& values) {
        stream.writeArray(values, &BinaryStream::writeVarInt64);
    });
    stream.writeOptional(mScale, &BinaryStream::writeByte);
    stream.writeOptional(mTrackedActors, [](BinaryStream& stream, const auto& values) {
        stream.writeArray(values, &MapTrackedActorUniqueId::write);
    });
    stream.writeOptional(mDecorationList, [](BinaryStream& stream, const auto& values) {
        stream.writeArray(values, &MapDecoration::write);
    });
    stream.writeOptional(mTextureWidth, &BinaryStream::writeVarInt);
    stream.writeOptional(mTextureHeight, &BinaryStream::writeVarInt);
    stream.writeOptional(mXTexCoordinate, &BinaryStream::writeVarInt);
    stream.writeOptional(mYTexCoordinate, &BinaryStream::writeVarInt);
    stream.writeOptional(mPixels, [](BinaryStream& stream, const auto& values) {
        stream.writeArray(values, &BinaryStream::writeUnsignedInt);
    });
}

Result<> ClientboundMapItemDataPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readVarInt64(mMapId));
    _SCULK_READ(stream.readByte(mDimension));
    _SCULK_READ(stream.readBool(mIsLockedMap));
    _SCULK_READ(mMapOrigin.read(stream));
    _SCULK_READ(stream.readOptional(mMapEntries, [](ReadOnlyBinaryStream& stream, auto& values) {
        return stream.readArray(values, &ReadOnlyBinaryStream::readVarInt64);
    }));
    _SCULK_READ(stream.readOptional(mScale, &ReadOnlyBinaryStream::readByte));
    _SCULK_READ(stream.readOptional(mTrackedActors, [](ReadOnlyBinaryStream& stream, auto& values) {
        return stream.readArray(values, &MapTrackedActorUniqueId::read);
    }));
    _SCULK_READ(stream.readOptional(mDecorationList, [](ReadOnlyBinaryStream& stream, auto& values) {
        return stream.readArray(values, &MapDecoration::read);
    }));
    _SCULK_READ(stream.readOptional(mTextureWidth, &ReadOnlyBinaryStream::readVarInt));
    _SCULK_READ(stream.readOptional(mTextureHeight, &ReadOnlyBinaryStream::readVarInt));
    _SCULK_READ(stream.readOptional(mXTexCoordinate, &ReadOnlyBinaryStream::readVarInt));
    _SCULK_READ(stream.readOptional(mYTexCoordinate, &ReadOnlyBinaryStream::readVarInt));
    _SCULK_READ(stream.readOptional(mPixels, [](ReadOnlyBinaryStream& stream, auto& values) {
        return stream.readArray(values, &ReadOnlyBinaryStream::readUnsignedInt);
    }));
    return {};
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ClientboundMapItemDataPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mMapId),
        SCULK_FORMAT_FIELD(mDimension),
        SCULK_FORMAT_FIELD(mIsLockedMap),
        SCULK_FORMAT_FIELD(mMapOrigin),
        SCULK_FORMAT_FIELD(mMapEntries),
        SCULK_FORMAT_FIELD(mScale),
        SCULK_FORMAT_FIELD(mTrackedActors),
        SCULK_FORMAT_FIELD(mDecorationList),
        SCULK_FORMAT_FIELD(mTextureWidth),
        SCULK_FORMAT_FIELD(mTextureHeight),
        SCULK_FORMAT_FIELD(mXTexCoordinate),
        SCULK_FORMAT_FIELD(mYTexCoordinate),
        SCULK_FORMAT_FIELD(mPixels)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
