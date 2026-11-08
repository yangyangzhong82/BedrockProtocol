// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/actor/attribute/EnvironmentAttributeData.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void NoiseAlignment::write(BinaryStream& stream) const {
    stream.writeEnum(mType, &BinaryStream::writeByte);
    stream.writeUnsignedVarInt(mValue);
}

Result<> NoiseAlignment::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mType, &ReadOnlyBinaryStream::readByte));
    return stream.readUnsignedVarInt(mValue);
}

void ConstantAttributeData::write(BinaryStream& stream) const { mAttribute.write(stream); }

Result<> ConstantAttributeData::read(ReadOnlyBinaryStream& stream) { return mAttribute.read(stream); }

void TransitionSettingsData::write(BinaryStream& stream) const {
    stream.writeUnsignedVarInt(mTotalTransitionTicks);
    stream.writeUnsignedVarInt(mCurrentTransitionTicks);
    stream.writeEnum(mEasing, &BinaryStream::writeUnsignedVarInt);
    stream.writeString(mClockName);
}

Result<> TransitionSettingsData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readUnsignedVarInt(mTotalTransitionTicks));
    _SCULK_READ(stream.readUnsignedVarInt(mCurrentTransitionTicks));
    _SCULK_READ(stream.readEnum(mEasing, &ReadOnlyBinaryStream::readUnsignedVarInt));
    return stream.readString(mClockName);
}

void TransitionAttributeData::write(BinaryStream& stream) const {
    mFromAttribute.write(stream);
    mToAttribute.write(stream);
    mSettings.write(stream);
}

Result<> TransitionAttributeData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mFromAttribute.read(stream));
    _SCULK_READ(mToAttribute.read(stream));
    return mSettings.read(stream);
}

void NoiseTransitionSettingsData::write(BinaryStream& stream) const {
    stream.writeUnsignedVarInt(mTotalTransitionTicks);
    stream.writeUnsignedVarInt(mCurrentTransitionTicks);
    stream.writeEnum(mEasing, &BinaryStream::writeUnsignedVarInt);
    stream.writeString(mClockName);
    stream.writeUnsignedVarInt(mLocalTransitionTicks);
    stream.writeString(mNoiseName);
    mNoiseAlignment.write(stream);
}

Result<> NoiseTransitionSettingsData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readUnsignedVarInt(mTotalTransitionTicks));
    _SCULK_READ(stream.readUnsignedVarInt(mCurrentTransitionTicks));
    _SCULK_READ(stream.readEnum(mEasing, &ReadOnlyBinaryStream::readUnsignedVarInt));
    _SCULK_READ(stream.readString(mClockName));
    _SCULK_READ(stream.readUnsignedVarInt(mLocalTransitionTicks));
    _SCULK_READ(stream.readString(mNoiseName));
    return mNoiseAlignment.read(stream);
}

void NoiseTransitionAttributeData::write(BinaryStream& stream) const {
    mFromAttribute.write(stream);
    mToAttribute.write(stream);
    mSettings.write(stream);
}

Result<> NoiseTransitionAttributeData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mFromAttribute.read(stream));
    _SCULK_READ(mToAttribute.read(stream));
    return mSettings.read(stream);
}

void EnvironmentAttributeData::write(BinaryStream& stream) const {
    stream.writeString(mAttributeName);
    stream.writeVariant(mPayload, [&stream](const auto& payload) { payload.write(stream); });
}

Result<> EnvironmentAttributeData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mAttributeName));
    return stream.readVariant(mPayload, [&stream](auto& payload) { return payload.read(stream); });
}


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
