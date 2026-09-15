// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/SubChunkPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void SubChunkPacket::SubChunkPosOffset::write(BinaryStream& stream) const {
    stream.writeSignedChar(mX);
    stream.writeSignedChar(mY);
    stream.writeSignedChar(mZ);
}

Result<> SubChunkPacket::SubChunkPosOffset::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readSignedChar(mX));
    _SCULK_READ(stream.readSignedChar(mY));
    return stream.readSignedChar(mZ);
}

void SubChunkPacket::HeightmapData::write(BinaryStream& stream) const {
    stream.writeEnum(mHeightMapType, &BinaryStream::writeByte);
    stream.writeOptional(mSubchunkHeightMap, [](BinaryStream& stream, const auto& data) {
        stream.writeBytes(&data, sizeof(data));
    });
    stream.writeEnum(mRenderHeightMapType, &BinaryStream::writeByte);
    stream.writeOptional(mRenderHeightMap, [](BinaryStream& stream, const auto& data) {
        stream.writeBytes(&data, sizeof(data));
    });
}

Result<> SubChunkPacket::HeightmapData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mHeightMapType, &ReadOnlyBinaryStream::readByte));
    _SCULK_READ(stream.readOptional(mSubchunkHeightMap, [](ReadOnlyBinaryStream& stream, auto& data) {
        return stream.readBytes(&data, sizeof(data));
    }));
    _SCULK_READ(stream.readEnum(mRenderHeightMapType, &ReadOnlyBinaryStream::readByte));
    return stream.readOptional(mRenderHeightMap, [](ReadOnlyBinaryStream& stream, auto& data) {
        return stream.readBytes(&data, sizeof(data));
    });
}

void SubChunkPacket::SubChunkPacketData::write(BinaryStream& stream) const {
    mSubChunkPosOffset.write(stream);
    stream.writeEnum(mResult, &BinaryStream::writeByte);
    stream.writeOptional(mSerializedSubChunk, &BinaryStream::writeString);
    mHeightMapData.write(stream);
    stream.writeOptional(mBlobId, &BinaryStream::writeUnsignedInt64);
}

Result<> SubChunkPacket::SubChunkPacketData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mSubChunkPosOffset.read(stream));
    _SCULK_READ(stream.readEnum(mResult, &ReadOnlyBinaryStream::readByte));
    _SCULK_READ(stream.readOptional(mSerializedSubChunk, &ReadOnlyBinaryStream::readString));
    _SCULK_READ(mHeightMapData.read(stream));
    return stream.readOptional(mBlobId, &ReadOnlyBinaryStream::readUnsignedInt64);
}

MinecraftPacketIds SubChunkPacket::getId() const noexcept { return MinecraftPacketIds::SubChunk; }

std::string_view SubChunkPacket::getName() const noexcept { return "SubChunkPacket"; }

void SubChunkPacket::write(BinaryStream& stream) const {
    stream.writeBool(mCacheEnabled);
    stream.writeVarInt(mDimensionType);
    mCenterPos.writeCereal(stream);
    stream.writeArray(mSubChunkData, &SubChunkPacketData::write);
}

Result<> SubChunkPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readBool(mCacheEnabled));
    _SCULK_READ(stream.readVarInt(mDimensionType));
    _SCULK_READ(mCenterPos.readCereal(stream));
    return stream.readArray(mSubChunkData, &SubChunkPacketData::read);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string SubChunkPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mDimensionType),
        SCULK_FORMAT_FIELD(mSubChunkData),
        SCULK_FORMAT_FIELD(mCenterPos)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
