// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/inventory/HandSlot.hpp"
#include "InventoryTransaction.hpp"
#include "sculk/protocol/codec/inventory/item/NetworkItemStackDescriptor.hpp"
#include "sculk/protocol/codec/math/Vec3.hpp"
#include "sculk/protocol/utility/Enum.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

class ItemReleaseInventoryTransaction {
public:
    enum class ActionType : std::int32_t {
        Release = 0,
        Use     = 1,
    };

public:
    InventoryTransaction       mTransaction{};
    ActionType                 mActionType{};
    HandSlot                   mHand{};
    std::int32_t               mSlot{};
    NetworkItemStackDescriptor mItem{};
    Vec3                       mFromPos{};

public:
    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(ItemReleaseInventoryTransaction::ActionType, 0, 1)
