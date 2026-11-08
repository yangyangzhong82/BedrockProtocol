// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "AttributeData.hpp"
#include "EasingFunction.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

struct NoiseAlignment {
    enum class Type : std::uint8_t { MinLocalTransitionEnd = 0 };
    Type          mType{};
    std::uint32_t mValue{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct ConstantAttributeData {
    AttributeData mAttribute{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct TransitionSettingsData {
    std::uint32_t  mTotalTransitionTicks{};
    std::uint32_t  mCurrentTransitionTicks{};
    EasingFunction mEasing{};
    std::string    mClockName{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct TransitionAttributeData {
    AttributeData          mFromAttribute{};
    AttributeData          mToAttribute{};
    TransitionSettingsData mSettings{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct NoiseTransitionSettingsData {
    std::uint32_t  mTotalTransitionTicks{};
    std::uint32_t  mCurrentTransitionTicks{};
    EasingFunction mEasing{};
    std::string    mClockName{};
    std::uint32_t  mLocalTransitionTicks{};
    std::string    mNoiseName{};
    NoiseAlignment mNoiseAlignment{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct NoiseTransitionAttributeData {
    AttributeData               mFromAttribute{};
    AttributeData               mToAttribute{};
    NoiseTransitionSettingsData mSettings{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct EnvironmentAttributeData {
    std::string                                                                                mAttributeName{};
    std::variant<ConstantAttributeData, TransitionAttributeData, NoiseTransitionAttributeData> mPayload{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(NoiseAlignment::Type, 0, 0)
