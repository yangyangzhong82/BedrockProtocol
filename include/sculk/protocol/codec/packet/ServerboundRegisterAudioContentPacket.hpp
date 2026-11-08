// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/packet/IPacket.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

struct AudioContentRegistrationEntry {
    std::string mAudioContentId{};
    std::string mSharedMetadata{};
    std::string mServerContent{};
    std::string mPlaybackContent{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

class ServerboundRegisterAudioContentPacket : public IPacket {
public:
    std::vector<AudioContentRegistrationEntry> mRegistrations{};

    [[nodiscard]] MinecraftPacketIds getId() const noexcept override;
    [[nodiscard]] std::string_view   getName() const noexcept override;
    void                             write(BinaryStream& stream) const override;
    [[nodiscard]] Result<>           read(ReadOnlyBinaryStream& stream) override;
    SCULK_PROTOCOL_PACKET_TO_STRING()
};


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_PACKET_FORMATTER(ServerboundRegisterAudioContentPacket)
