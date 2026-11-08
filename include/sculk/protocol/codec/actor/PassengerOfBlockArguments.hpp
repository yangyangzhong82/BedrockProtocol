// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/level/block/BlockPos.hpp"
#include "sculk/protocol/codec/math/Vec3.hpp"
#include "sculk/protocol/utility/Enum.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

struct PassengerOfBlockArguments {
    enum class EmoteType : std::uint8_t { Standing = 0, Riding = 1, Laying = 2 };

    BlockPos  mBlockPos{};
    Vec3      mOffset{};
    float     mRotation{};
    float     mRotationLimit{};
    EmoteType mEmoteType{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(PassengerOfBlockArguments::EmoteType, 0, 2)
