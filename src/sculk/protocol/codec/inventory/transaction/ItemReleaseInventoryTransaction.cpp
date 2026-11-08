// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/inventory/transaction/ItemReleaseInventoryTransaction.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void ItemReleaseInventoryTransaction::write(BinaryStream& stream) const {
    mTransaction.write(stream);
    stream.writeEnum(mActionType, &BinaryStream::writeVarInt);
    stream.writeVarInt(mSlot);
    mItem.writeCereal(stream);
    mFromPos.write(stream);
    stream.writeEnum(mHand, &BinaryStream::writeByte);
}

Result<> ItemReleaseInventoryTransaction::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mTransaction.read(stream));
    _SCULK_READ(stream.readEnum(mActionType, &ReadOnlyBinaryStream::readVarInt));
    _SCULK_READ(stream.readVarInt(mSlot));
    _SCULK_READ(mItem.readCereal(stream));
    _SCULK_READ(mFromPos.read(stream));
    return stream.readEnum(mHand, &ReadOnlyBinaryStream::readByte);
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
