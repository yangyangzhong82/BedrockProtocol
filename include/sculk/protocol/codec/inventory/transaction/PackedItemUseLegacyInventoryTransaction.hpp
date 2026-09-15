// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/inventory/container/LegacySetItemSlot.hpp"
#include "sculk/protocol/codec/inventory/transaction/InventoryTransactionData.hpp"
#include "sculk/protocol/codec/inventory/transaction/ItemUseInventoryTransaction.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

struct PackedItemUseLegacyInventoryTransaction {
    std::int32_t                                  mLegacyRequestRawId{};
    std::optional<std::vector<LegacySetItemSlot>> mLegacySetItemSlots{};
    ItemUseInventoryTransaction                   mItemUseTransaction{};
    bool                                          mHasActions{true};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
