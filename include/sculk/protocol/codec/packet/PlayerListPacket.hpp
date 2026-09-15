// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/actor/player/PlayerListEntry.hpp"
#include "sculk/protocol/codec/packet/IPacket.hpp"
#include "sculk/protocol/utility/Enum.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

class PlayerListPacket : public IPacket {
public:
    using ActionType = PlayerListEntry::ActionType;

public:
    std::vector<PlayerListEntry> mPlayerEntryList{};

public:
    [[nodiscard]] MinecraftPacketIds getId() const noexcept override;

    [[nodiscard]] std::string_view getName() const noexcept override;

    void write(BinaryStream& stream) const override;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream) override;

    SCULK_PROTOCOL_PACKET_TO_STRING()
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(PlayerListPacket::ActionType, 0, 1)

SCULK_PROTOCOL_PACKET_FORMATTER(PlayerListPacket)