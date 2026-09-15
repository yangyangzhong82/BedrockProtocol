// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/inventory/item/ItemStackResponse.hpp"
#include "sculk/protocol/codec/utility/Cereal.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void ItemStackResponseSlotInfo::write(BinaryStream& stream) const {
    stream.writeByte(mRequestedSlot);
    stream.writeByte(mSlot);
    stream.writeByte(mAmount);
    writeDoubleOptional(stream, mNetId, &BinaryStream::writeVarInt);
    stream.writeString(mCustomName);
    stream.writeOptional(mFilteredCustomName, &BinaryStream::writeString);
    stream.writeVarInt(mDurationCorrection);
}

Result<> ItemStackResponseSlotInfo::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readByte(mRequestedSlot));
    _SCULK_READ(stream.readByte(mSlot));
    _SCULK_READ(stream.readByte(mAmount));
    _SCULK_READ(readDoubleOptional(stream, mNetId, &ReadOnlyBinaryStream::readVarInt));
    _SCULK_READ(stream.readString(mCustomName));
    _SCULK_READ(stream.readOptional(mFilteredCustomName, &ReadOnlyBinaryStream::readString));
    _SCULK_READ(stream.readVarInt(mDurationCorrection));
    if (mDurationCorrection < -32768 || mDurationCorrection > 32767) {
        return error_utils::makeError("Durability correction out of range");
    }
    return {};
}

void ItemStackResponseContainerInfo::write(BinaryStream& stream) const {
    mContainerName.write(stream);
    stream.writeArray(mSlots, &ItemStackResponseSlotInfo::write);
}

Result<> ItemStackResponseContainerInfo::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mContainerName.read(stream));
    return stream.readArray(mSlots, &ItemStackResponseSlotInfo::read);
}

void ItemStackResponseInfo::write(BinaryStream& stream) const {
    stream.writeEnum(mResult, &BinaryStream::writeByte);
    stream.writeVarInt(mRequestId);
    writeDoubleOptional(stream, mContainers, [](BinaryStream& stream, const auto& containers) {
        stream.writeArray(containers, &ItemStackResponseContainerInfo::write);
    });
}

Result<> ItemStackResponseInfo::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mResult, &ReadOnlyBinaryStream::readByte));
    _SCULK_READ(stream.readVarInt(mRequestId));
    return readDoubleOptional(stream, mContainers, [](ReadOnlyBinaryStream& stream, auto& containers) {
        return stream.readArray(containers, &ItemStackResponseContainerInfo::read);
    });
}

void ItemStackResponse::write(BinaryStream& stream) const {
    stream.writeArray(mResponses, &ItemStackResponseInfo::write);
}

Result<> ItemStackResponse::read(ReadOnlyBinaryStream& stream) {
    return stream.readArray(mResponses, &ItemStackResponseInfo::read);
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
