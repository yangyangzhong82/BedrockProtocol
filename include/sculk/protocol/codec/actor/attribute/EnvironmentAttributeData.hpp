// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "AttributeData.hpp"
#include "EasingFunction.hpp"
#include "sculk/protocol/utility/BinaryStream.hpp"
#include "sculk/protocol/utility/ReadOnlyBinaryStream.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

struct NoiseAlignment {
    enum class Type : std::uint8_t { MinLocalTransitionEnd = 0 };
    Type          mType{};
    std::uint32_t mValue{};
};

struct EnvironmentAttributeData {
    std::string                  mAttributeName{};
    std::optional<AttributeData> mFromAttribute{};
    AttributeData                mAttribute{};
    std::optional<AttributeData> mToAttribute{};
    std::uint32_t                mCurrentTransitionTicks{};
    std::uint32_t                mTotalTransitionTicks{};
    EasingFunction               mEasing{};
    std::uint32_t                mLocalTransitionTicks{};
    bool                         mNoiseTransition{};
    NoiseAlignment               mNoiseAlignment{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
