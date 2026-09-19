// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/inventory/transaction/PackedItemUseLegacyInventoryTransaction.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void PackedItemUseLegacyInventoryTransaction::write(BinaryStream& stream) const {
    stream.writeVarInt(mLegacyRequestRawId);
    stream.writeOptional(mLegacySetItemSlots, [](BinaryStream& stream, const auto& slots) {
        stream.writeArray(slots, &LegacySetItemSlot::write);
    });
    mItemUseTransaction.write(stream);
}

Result<> PackedItemUseLegacyInventoryTransaction::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readVarInt(mLegacyRequestRawId));
    _SCULK_READ(stream.readOptional(mLegacySetItemSlots, [](ReadOnlyBinaryStream& stream, auto& slots) {
        return stream.readArray(slots, &LegacySetItemSlot::read);
    }));
    return mItemUseTransaction.read(stream);
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
