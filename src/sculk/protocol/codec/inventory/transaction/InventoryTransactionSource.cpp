// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/inventory/transaction/InventoryTransactionSource.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void InventoryTransactionSource::write(BinaryStream& stream) const {
    stream.writeEnum(mType, &BinaryStream::writeUnsignedVarInt);
    stream.writeOptional(mContainerId, &BinaryStream::writeByte);
    stream.writeOptional(mBitFlags, &BinaryStream::writeUnsignedVarInt);
}

Result<> InventoryTransactionSource::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mType, &ReadOnlyBinaryStream::readUnsignedVarInt));
    _SCULK_READ(stream.readOptional(mContainerId, &ReadOnlyBinaryStream::readByte));
    return stream.readOptional(mBitFlags, &ReadOnlyBinaryStream::readUnsignedVarInt);
}

void InventoryTransactionSource::writeLegacy(BinaryStream& stream) const {
    stream.writeEnum(mType, &BinaryStream::writeUnsignedVarInt);
    switch (mType) {
    case InventoryTransactionSourceType::ContainerInventory:
    case InventoryTransactionSourceType::NonImplementedFeatureTODO:
        stream.writeVarInt(mContainerId.value_or(0));
        break;
    case InventoryTransactionSourceType::WorldInteraction:
        stream.writeUnsignedVarInt(mBitFlags.value_or(0));
        break;
    default:
        break;
    }
}

Result<> InventoryTransactionSource::readLegacy(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mType, &ReadOnlyBinaryStream::readUnsignedVarInt));
    switch (mType) {
    case InventoryTransactionSourceType::ContainerInventory:
    case InventoryTransactionSourceType::NonImplementedFeatureTODO: {
        int containerId{};
        _SCULK_READ(stream.readVarInt(containerId));
        mContainerId.emplace() = static_cast<std::uint8_t>(containerId);
        return {};
    }
    case InventoryTransactionSourceType::WorldInteraction: {
        return stream.readUnsignedVarInt(mBitFlags.emplace());
    }
    default:
        return {};
    }
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
