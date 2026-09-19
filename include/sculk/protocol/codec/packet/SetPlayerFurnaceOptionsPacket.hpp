// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/packet/IPacket.hpp"
#include "sculk/protocol/utility/Enum.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

class SetPlayerFurnaceOptionsPacket : public IPacket {
public:
    enum class FurnaceType : std::uint8_t { None, Furnace, BlastFurnace, Smoker, Count };
    enum class FurnaceLeftTabIndex : std::int32_t {
        None,
        RecipeFood,
        RecipeItems,
        RecipeBlocks,
        RecipeSearch,
        Inventory,
        Count
    };
    enum class FurnaceLayout : std::int32_t { None, InventoryOnly, Default, Count };

    FurnaceType         mFurnaceType{};
    FurnaceLeftTabIndex mLeftFurnaceTab{};
    bool                mFiltering{};
    FurnaceLayout       mLayout{};

    [[nodiscard]] MinecraftPacketIds getId() const noexcept override;
    [[nodiscard]] std::string_view   getName() const noexcept override;
    void                             write(BinaryStream& stream) const override;
    [[nodiscard]] Result<>           read(ReadOnlyBinaryStream& stream) override;
    SCULK_PROTOCOL_PACKET_TO_STRING()
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(SetPlayerFurnaceOptionsPacket::FurnaceType, 0, 4)
SCULK_PROTOCOL_ENUM_RANGE(SetPlayerFurnaceOptionsPacket::FurnaceLeftTabIndex, 0, 6)
SCULK_PROTOCOL_ENUM_RANGE(SetPlayerFurnaceOptionsPacket::FurnaceLayout, 0, 3)
SCULK_PROTOCOL_PACKET_FORMATTER(SetPlayerFurnaceOptionsPacket)
