// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/actor/player/SerializedSkin.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void SerializedSkin::Animation::write(BinaryStream& stream) const {
    stream.writeUnsignedInt(mWidth);
    stream.writeUnsignedInt(mHeight);
    stream.writeString(mSkinImageBytes);
    stream.writeEnum(mAnimationType, &BinaryStream::writeUnsignedVarInt);
    stream.writeFloat(mFrameCount);
    stream.writeEnum(mAnimationExpression, &BinaryStream::writeUnsignedVarInt);
}

Result<> SerializedSkin::Animation::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readUnsignedInt(mWidth));
    _SCULK_READ(stream.readUnsignedInt(mHeight));
    _SCULK_READ(stream.readString(mSkinImageBytes));
    _SCULK_READ(stream.readEnum(mAnimationType, &ReadOnlyBinaryStream::readUnsignedVarInt));
    _SCULK_READ(stream.readFloat(mFrameCount));
    return stream.readEnum(mAnimationExpression, &ReadOnlyBinaryStream::readUnsignedVarInt);
}

void SerializedSkin::PersonaPiece::write(BinaryStream& stream) const {
    stream.writeString(mPieceId);
    stream.writeEnum(mPieceType, &BinaryStream::writeUnsignedInt);
    mPackId.write(stream);
    stream.writeBool(mIsDefaultPiece);
    stream.writeString(mProductId);
}

Result<> SerializedSkin::PersonaPiece::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mPieceId));
    _SCULK_READ(stream.readEnum(mPieceType, &ReadOnlyBinaryStream::readUnsignedInt));
    _SCULK_READ(mPackId.read(stream));
    _SCULK_READ(stream.readBool(mIsDefaultPiece));
    return stream.readString(mProductId);
}

void SerializedSkin::PieceTintColors::write(BinaryStream& stream) const {
    stream.writeString(mPieceType);
    for (auto color : mPieceTintColors) {
        stream.writeUnsignedInt(color);
    }
}

Result<> SerializedSkin::PieceTintColors::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mPieceType));
    for (auto& color : mPieceTintColors) {
        _SCULK_READ(stream.readUnsignedInt(color));
    }
    return {};
}

void SerializedSkin::write(BinaryStream& stream) const {
    stream.writeString(mId);
    stream.writeString(mPlayFabId);
    stream.writeString(mResourcePatch);
    stream.writeUnsignedInt(mSkinImageWidth);
    stream.writeUnsignedInt(mSkinImageHeight);
    stream.writeString(mSkinImageBytes);
    stream.writeArray(mAnimations, &Animation::write);
    stream.writeUnsignedInt(mCapeImageWidth);
    stream.writeUnsignedInt(mCapeImageHeight);
    stream.writeString(mCapeImageBytes);
    stream.writeString(mGeometryData);
    stream.writeString(mGeometryDataMinEngineVersion);
    stream.writeString(mAnimationData);
    stream.writeString(mCapeId);
    stream.writeString(mFullId);
    stream.writeEnum(mArmSize, &BinaryStream::writeByte);
    stream.writeUnsignedInt(mSkinColor);
    stream.writeArray(mPersonaPieces, &PersonaPiece::write);
    stream.writeArray(mPieceTintColors, &PieceTintColors::write);
    stream.writeBool(mIsPremiumSkin);
    stream.writeBool(mIsPersonaSkin);
    stream.writeBool(mIsPersonaCapeOnClassicSkin);
    stream.writeBool(mIsPrimaryUser);
    stream.writeBool(mOverridesPlayerAppearance);
    stream.writeString(mTrustedSkinFlag);
    stream.writeString(mProfileHash);
}

Result<> SerializedSkin::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mId));
    _SCULK_READ(stream.readString(mPlayFabId));
    _SCULK_READ(stream.readString(mResourcePatch));
    _SCULK_READ(stream.readUnsignedInt(mSkinImageWidth));
    _SCULK_READ(stream.readUnsignedInt(mSkinImageHeight));
    _SCULK_READ(stream.readString(mSkinImageBytes));
    _SCULK_READ(stream.readArray(mAnimations, &Animation::read));
    _SCULK_READ(stream.readUnsignedInt(mCapeImageWidth));
    _SCULK_READ(stream.readUnsignedInt(mCapeImageHeight));
    _SCULK_READ(stream.readString(mCapeImageBytes));
    _SCULK_READ(stream.readString(mGeometryData));
    _SCULK_READ(stream.readString(mGeometryDataMinEngineVersion));
    _SCULK_READ(stream.readString(mAnimationData));
    _SCULK_READ(stream.readString(mCapeId));
    _SCULK_READ(stream.readString(mFullId));
    _SCULK_READ(stream.readEnum(mArmSize, &ReadOnlyBinaryStream::readByte));
    _SCULK_READ(stream.readUnsignedInt(mSkinColor));
    _SCULK_READ(stream.readArray(mPersonaPieces, &PersonaPiece::read));
    _SCULK_READ(stream.readArray(mPieceTintColors, &PieceTintColors::read));
    _SCULK_READ(stream.readBool(mIsPremiumSkin));
    _SCULK_READ(stream.readBool(mIsPersonaSkin));
    _SCULK_READ(stream.readBool(mIsPersonaCapeOnClassicSkin));
    _SCULK_READ(stream.readBool(mIsPrimaryUser));
    _SCULK_READ(stream.readBool(mOverridesPlayerAppearance));
    _SCULK_READ(stream.readString(mTrustedSkinFlag));
    return stream.readString(mProfileHash);
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
