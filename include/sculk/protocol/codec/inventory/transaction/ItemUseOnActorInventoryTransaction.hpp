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
#include <cstdint>

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

class ItemUseOnActorInventoryTransaction {
public:
    enum class ActionType : std::uint8_t {
        Interact     = 0,
        Attack       = 1,
        ItemInteract = 2,
    };

public:
    InventoryTransaction       mTransaction{};
    std::uint64_t              mRuntimeId{};
    ActionType                 mActionType{};
    HandSlot                   mHand{};
    std::int32_t               mSlot{};
    NetworkItemStackDescriptor mItem{};
    Vec3                       mFromPos{};
    Vec3                       mHitPos{};

public:
    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(ItemUseOnActorInventoryTransaction::ActionType, 0, 2)
