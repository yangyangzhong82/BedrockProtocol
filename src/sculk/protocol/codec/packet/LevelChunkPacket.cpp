// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/LevelChunkPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds LevelChunkPacket::getId() const noexcept { return MinecraftPacketIds::LevelChunk; }

std::string_view LevelChunkPacket::getName() const noexcept { return "LevelChunkPacket"; }

void LevelChunkPacket::write(BinaryStream& stream) const {
    mPosition.write(stream);
    stream.writeVarInt(mDimensionId);
    stream.writeUnsignedVarInt(mSubChunksCount);
    stream.writeBool(mClientNeedsToRequestSubchunks);
    if (mClientNeedsToRequestSubchunks) {
        stream.writeVarInt(mClientRequestSubChunkLimit);
    }
    stream.writeBool(mCacheEnabled);
    stream.writeArray(mCacheBlobs, &BinaryStream::writeUnsignedInt64);
    stream.writeString(mSerializedChunk);
}

Result<> LevelChunkPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mPosition.read(stream));
    _SCULK_READ(stream.readVarInt(mDimensionId));
    _SCULK_READ(stream.readUnsignedVarInt(mSubChunksCount));
    _SCULK_READ(stream.readBool(mClientNeedsToRequestSubchunks));
    mClientRequestSubChunkLimit = 0;
    if (mClientNeedsToRequestSubchunks) {
        _SCULK_READ(stream.readVarInt(mClientRequestSubChunkLimit));
    }
    _SCULK_READ(stream.readBool(mCacheEnabled));
    _SCULK_READ(stream.readArray(mCacheBlobs, &ReadOnlyBinaryStream::readUnsignedInt64));
    return stream.readString(mSerializedChunk);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string LevelChunkPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mPosition),
        SCULK_FORMAT_FIELD(mDimensionId),
        SCULK_FORMAT_FIELD(mIsChunkInTickRange),
        SCULK_FORMAT_FIELD(mSerializedChunk),
        SCULK_FORMAT_FIELD(mSubChunksCount),
        SCULK_FORMAT_FIELD(mClientNeedsToRequestSubchunks),
        SCULK_FORMAT_FIELD(mClientRequestSubChunkLimit),
        SCULK_FORMAT_FIELD(mCacheEnabled),
        SCULK_FORMAT_FIELD(mCacheBlobs)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
