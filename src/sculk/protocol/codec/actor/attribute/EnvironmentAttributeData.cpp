// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/actor/attribute/EnvironmentAttributeData.hpp"
#include <algorithm>

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

namespace {
constexpr std::string_view easingNames[] = {
    "linear",       "spring",      "in_quad",     "out_quad",      "in_out_quad",   "in_cubic",    "out_cubic",
    "in_out_cubic", "in_quart",    "out_quart",   "in_out_quart",  "in_quint",      "out_quint",   "in_out_quint",
    "in_sine",      "out_sine",    "in_out_sine", "in_expo",       "out_expo",      "in_out_expo", "in_circ",
    "out_circ",     "in_out_circ", "in_bounce",   "out_bounce",    "in_out_bounce", "in_back",     "out_back",
    "in_out_back",  "in_elastic",  "out_elastic", "in_out_elastic"
};
}

void EnvironmentAttributeData::write(BinaryStream& stream) const {
    stream.writeString(mAttributeName);
    stream.writeOptional(mFromAttribute, &AttributeData::write);
    mAttribute.write(stream);
    stream.writeOptional(mToAttribute, &AttributeData::write);
    stream.writeUnsignedInt(mCurrentTransitionTicks);
    stream.writeUnsignedInt(mTotalTransitionTicks);
    const auto easing = static_cast<std::size_t>(mEasing);
    stream.writeString(easing < std::size(easingNames) ? easingNames[easing] : easingNames[0]);
    stream.writeUnsignedInt(mLocalTransitionTicks);
    stream.writeBool(mNoiseTransition);
    stream.writeEnum(mNoiseAlignment.mType, &BinaryStream::writeByte);
    stream.writeUnsignedVarInt(mNoiseAlignment.mValue);
}

Result<> EnvironmentAttributeData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mAttributeName));
    _SCULK_READ(stream.readOptional(mFromAttribute, &AttributeData::read));
    _SCULK_READ(mAttribute.read(stream));
    _SCULK_READ(stream.readOptional(mToAttribute, &AttributeData::read));
    _SCULK_READ(stream.readUnsignedInt(mCurrentTransitionTicks));
    _SCULK_READ(stream.readUnsignedInt(mTotalTransitionTicks));
    std::string easing;
    _SCULK_READ(stream.readString(easing));
    auto found = std::find(std::begin(easingNames), std::end(easingNames), easing);
    if (found == std::end(easingNames)) {
        return error_utils::makeError("Invalid environment attribute easing");
    }
    mEasing = static_cast<EasingFunction>(found - std::begin(easingNames));
    _SCULK_READ(stream.readUnsignedInt(mLocalTransitionTicks));
    _SCULK_READ(stream.readBool(mNoiseTransition));
    _SCULK_READ(stream.readEnum(mNoiseAlignment.mType, &ReadOnlyBinaryStream::readByte));
    return stream.readUnsignedVarInt(mNoiseAlignment.mValue);
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
