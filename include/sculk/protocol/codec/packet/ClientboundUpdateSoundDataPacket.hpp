// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/packet/IPacket.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

class ClientboundUpdateSoundDataPacket : public IPacket {
public:
    std::uint64_t mHandle{};
    struct SoundUpdate {
        enum class Type : std::uint32_t { Stop, SetVolume, SetPitch, Fade, SeekTo, Pause, Resume };
        Type  mType{};
        float mValue{};        // Volume, pitch, fade duration, or seek position (seconds).
        float mTargetVolume{}; // Only present for Fade, after the duration.

        void                   write(BinaryStream& stream) const;
        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };
    std::optional<SoundUpdate> mStop{};
    std::optional<SoundUpdate> mSetVolume{};
    std::optional<SoundUpdate> mSetPitch{};
    std::optional<SoundUpdate> mFade{};
    std::optional<SoundUpdate> mSeekTo{};
    std::optional<SoundUpdate> mPause{};
    std::optional<SoundUpdate> mResume{};

public:
    [[nodiscard]] MinecraftPacketIds getId() const noexcept override;

    [[nodiscard]] std::string_view getName() const noexcept override;

    void write(BinaryStream& stream) const override;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream) override;

    SCULK_PROTOCOL_PACKET_TO_STRING()
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_PACKET_FORMATTER(ClientboundUpdateSoundDataPacket)