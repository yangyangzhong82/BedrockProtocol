// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/actor/player/UUID.hpp"
#include "sculk/protocol/utility/BinaryStream.hpp"
#include "sculk/protocol/utility/Enum.hpp"
#include "sculk/protocol/utility/ReadOnlyBinaryStream.hpp"
#include <array>

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

enum class AnimatedTextureType : std::uint8_t {
    None        = 0,
    Face        = 1,
    Body32x32   = 2,
    Body128x128 = 3,
};

enum class AnimationExpression : std::uint8_t {
    Linear   = 0,
    Blinking = 1,
};

class SerializedSkin {
public:
    enum class PieceType : std::uint8_t {
        Skeleton      = 1,
        Body          = 2,
        Skin          = 3,
        Bottom        = 4,
        Feet          = 5,
        Dress         = 6,
        Top           = 7,
        HighPants     = 8,
        Hands         = 9,
        Outerwear     = 10,
        FacialHair    = 11,
        Mouth         = 12,
        Eyes          = 13,
        Hair          = 14,
        Hood          = 15,
        Back          = 16,
        FaceAccessory = 17,
        Head          = 18,
        Legs          = 19,
        LeftLeg       = 20,
        RightLeg      = 21,
        Arms          = 22,
        LeftArm       = 23,
        RightArm      = 24,
        Capes         = 25,
        ClassicSkin   = 26,
        Emote         = 27,
    };

    enum class ArmSizeType : std::uint8_t {
        Slim = 0,
        Wide = 1,
    };

public:
    struct Animation {
        std::uint32_t       mWidth{};
        std::uint32_t       mHeight{};
        std::string         mSkinImageBytes{};
        AnimatedTextureType mAnimationType{};
        float               mFrameCount{};
        AnimationExpression mAnimationExpression{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

    struct PersonaPiece {
        std::string mPieceId{};
        PieceType   mPieceType{};
        UUID        mPackId{};
        bool        mIsDefaultPiece{};
        std::string mProductId{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

    struct PieceTintColors {
        std::string                  mPieceType{};
        std::array<std::uint32_t, 4> mPieceTintColors{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

public:
    std::string                  mId{};
    std::string                  mPlayFabId{};
    std::string                  mResourcePatch{};
    std::uint32_t                mSkinImageWidth{};
    std::uint32_t                mSkinImageHeight{};
    std::string                  mSkinImageBytes{};
    std::vector<Animation>       mAnimations{};
    std::uint32_t                mCapeImageWidth{};
    std::uint32_t                mCapeImageHeight{};
    std::string                  mCapeImageBytes{};
    std::string                  mGeometryData{};
    std::string                  mGeometryDataMinEngineVersion{};
    std::string                  mAnimationData{};
    std::string                  mCapeId{};
    std::string                  mFullId{};
    ArmSizeType                  mArmSize{};
    std::uint32_t                mSkinColor{};
    std::vector<PersonaPiece>    mPersonaPieces{};
    std::vector<PieceTintColors> mPieceTintColors{};
    bool                         mIsPremiumSkin{};
    bool                         mIsPersonaSkin{};
    bool                         mIsPersonaCapeOnClassicSkin{};
    bool                         mIsPrimaryUser{};
    bool                         mOverridesPlayerAppearance{};
    // "unset", "false", or "true", as used by the TrustedSkinFlag wire enum.
    std::string mTrustedSkinFlag{"unset"};
    std::string mProfileHash{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(AnimatedTextureType, 0, 3)
SCULK_PROTOCOL_ENUM_RANGE(AnimationExpression, 0, 1)
SCULK_PROTOCOL_ENUM_RANGE(SerializedSkin::ArmSizeType, 0, 1)
SCULK_PROTOCOL_ENUM_RANGE(SerializedSkin::PieceType, 1, 27)
