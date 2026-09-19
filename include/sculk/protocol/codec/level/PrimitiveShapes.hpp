// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/math/Vec2.hpp"
#include "sculk/protocol/codec/math/Vec3.hpp"
#include "sculk/protocol/utility/BinaryStream.hpp"
#include "sculk/protocol/utility/Enum.hpp"
#include "sculk/protocol/utility/ReadOnlyBinaryStream.hpp"
#include <cstdint>
#include <optional>
#include <string>
#include <variant>

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

enum class PrimitiveShapesType : std::uint8_t {
    Line          = 0,
    Box           = 1,
    Sphere        = 2,
    Circle        = 3,
    Text          = 4,
    Arrow         = 5,
    Cylinder      = 6,
    Pyramid       = 7,
    Ellipsoid     = 8,
    Cone          = 9,
    NumShapeTypes = 10,
};

struct ArrowDataPayload {
    std::optional<Vec3>         mArrowEndLocation{};
    std::optional<float>        mArrowHeadLength{};
    std::optional<float>        mArrowHeadRadius{};
    std::optional<std::uint8_t> mArrowSegments{};
};

struct TextDataPayload {
    std::string        mText{};
    bool               mUseRotation{};
    std::optional<int> mBackgroundColor{};
    float              mLineGapHeight{};
    bool               mDepthTest{};
    bool               mShowBackface{};
    bool               mShowTextBackface{};
};

struct BoxDataPayload {
    Vec3 mBoxBound{};
};

struct LineDataPayload {
    Vec3 mLineEndLocation{};
};

struct SphereDataPayload {
    std::uint8_t mSegments{};
};

struct CylinderDataPayload {
    Vec2         mRadiusX{};
    Vec2         mRadiusZ{};
    float        mHeight{};
    std::uint8_t mNumSegments{};
};

struct PyramidDataPayload {
    float                mWidth{};
    std::optional<float> mDepth{};
    float                mHeight{};
};

struct EllipsoidDataPayload {
    Vec3         mRadii{};
    std::uint8_t mSegmentsPerAxis{};
};

struct ConeDataPayload {
    Vec2         mRadii{};
    float        mHeight{};
    std::uint8_t mNumSegments{};
};

using PrimitiveShapeDataVariant = std::variant<
    std::monostate,
    ArrowDataPayload,
    TextDataPayload,
    BoxDataPayload,
    LineDataPayload,
    SphereDataPayload,
    CylinderDataPayload,
    PyramidDataPayload,
    EllipsoidDataPayload,
    ConeDataPayload>;

struct PrimitiveShapes {
    std::uint64_t                      mNetworkId{};
    std::optional<Vec3>                mLocation{};
    std::optional<PrimitiveShapesType> mType{};
    std::optional<std::int32_t>        mDimensionId{};
    std::optional<float>               mScale{};
    std::optional<Vec3>                mRotation{};
    std::optional<float>               mTotalTimeLeft{};
    std::optional<float>               mMaxRenderDistance{};
    std::optional<std::int32_t>        mColor{};
    std::optional<std::int64_t>        mAttachedToEntityId{};
    PrimitiveShapeDataVariant          mShape{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(PrimitiveShapesType, 0, 10)
