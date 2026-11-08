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

struct MatchmakingStateOptions {
    std::optional<std::string> mTriggeringPlayerName{};
    std::optional<bool>        mTriggeredByLocalPlayer{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

class ClientboundMatchmakingStatePacket : public IPacket {
public:
    enum class State : std::uint8_t {
        Idle = 0,
        Matchmaking,
        MatchFound,
        Canceled,
        PlayerLeftParty,
        PlayerLeftServer,
        ServerShutdown,
        TimedOut,
        RequeueAsParty
    };

    State                                  mState{};
    std::string                            mDestinationName{};
    std::optional<MatchmakingStateOptions> mOptions{};

    [[nodiscard]] MinecraftPacketIds getId() const noexcept override;
    [[nodiscard]] std::string_view   getName() const noexcept override;
    void                             write(BinaryStream& stream) const override;
    [[nodiscard]] Result<>           read(ReadOnlyBinaryStream& stream) override;
    SCULK_PROTOCOL_PACKET_TO_STRING()
};


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(ClientboundMatchmakingStatePacket::State, 0, 8)

SCULK_PROTOCOL_PACKET_FORMATTER(ClientboundMatchmakingStatePacket)
