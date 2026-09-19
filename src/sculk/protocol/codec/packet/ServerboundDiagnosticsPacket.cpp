// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ServerboundDiagnosticsPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void ServerboundDiagnosticsPacket::MemoryCategoryCounter::write(BinaryStream& stream) const {
    stream.writeEnum(mType, &BinaryStream::writeByte);
    stream.writeUnsignedInt64(mCurrentBytes);
}

Result<> ServerboundDiagnosticsPacket::MemoryCategoryCounter::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readEnum(mType, &ReadOnlyBinaryStream::readByte));
    return stream.readUnsignedInt64(mCurrentBytes);
}

void ServerboundDiagnosticsPacket::EntityDiagnosticTimingInfo::write(BinaryStream& stream) const {
    stream.writeString(mDisplayName);
    stream.writeString(mEntity);
    stream.writeUnsignedInt64(mTimeInNanoseconds);
    stream.writeByte(mPercentOfTotal);
    stream.writeOptional(mPosition, &Vec3::write);
    stream.writeOptional(mDimension, &BinaryStream::writeString);
}

Result<> ServerboundDiagnosticsPacket::EntityDiagnosticTimingInfo::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mDisplayName));
    _SCULK_READ(stream.readString(mEntity));
    _SCULK_READ(stream.readUnsignedInt64(mTimeInNanoseconds));
    _SCULK_READ(stream.readByte(mPercentOfTotal));
    _SCULK_READ(stream.readOptional(mPosition, &Vec3::read));
    return stream.readOptional(mDimension, &ReadOnlyBinaryStream::readString);
}

void ServerboundDiagnosticsPacket::SystemDiagnosticTimingInfo::write(BinaryStream& stream) const {
    stream.writeString(mDisplayName);
    stream.writeUnsignedInt64(mSystemIndex);
    stream.writeUnsignedInt64(mTimeInNanoseconds);
    stream.writeByte(mPercentOfTotal);
}

Result<> ServerboundDiagnosticsPacket::SystemDiagnosticTimingInfo::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mDisplayName));
    _SCULK_READ(stream.readUnsignedInt64(mSystemIndex));
    _SCULK_READ(stream.readUnsignedInt64(mTimeInNanoseconds));
    return stream.readByte(mPercentOfTotal);
}

void ServerboundDiagnosticsPacket::SystemCategory::write(BinaryStream& stream) const {
    stream.writeString(mCategoryName);
    stream.writeUnsignedInt64(mSystemIndex);
}

Result<> ServerboundDiagnosticsPacket::SystemCategory::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mCategoryName));
    return stream.readUnsignedInt64(mSystemIndex);
}

void ServerboundDiagnosticsPacket::ScopeDataSummary::write(BinaryStream& stream) const {
    stream.writeString(mLabel);
    stream.writeString(mIndentation);
    stream.writeUnsignedInt64(mTotalHighCostNS);
    stream.writeUnsignedInt64(mTotalMidCostNS);
    stream.writeUnsignedInt64(mTotalLowCostNS);
}

Result<> ServerboundDiagnosticsPacket::ScopeDataSummary::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mLabel));
    _SCULK_READ(stream.readString(mIndentation));
    _SCULK_READ(stream.readUnsignedInt64(mTotalHighCostNS));
    _SCULK_READ(stream.readUnsignedInt64(mTotalMidCostNS));
    return stream.readUnsignedInt64(mTotalLowCostNS);
}

MinecraftPacketIds ServerboundDiagnosticsPacket::getId() const noexcept {
    return MinecraftPacketIds::ServerboundDiagnostics;
}
std::string_view ServerboundDiagnosticsPacket::getName() const noexcept { return "ServerboundDiagnosticsPacket"; }

void ServerboundDiagnosticsPacket::write(BinaryStream& stream) const {
    stream.writeFloat(mAvgFps);
    stream.writeFloat(mAvgServerSimTickTimeMS);
    stream.writeFloat(mAvgClientSimTickTimeMS);
    stream.writeFloat(mAvgBeginFrameTimeMS);
    stream.writeFloat(mAvgInputTimeMS);
    stream.writeFloat(mAvgRenderTimeMS);
    stream.writeFloat(mAvgEndFrameTimeMS);
    stream.writeFloat(mAvgRemainderTimePercent);
    stream.writeFloat(mAvgUnaccountedTimePercent);
    stream.writeArray(mMemoryCategoryValues, &MemoryCategoryCounter::write);
    stream.writeArray(mEntityDiagnostics, &EntityDiagnosticTimingInfo::write);
    stream.writeArray(mSystemDiagnostics, &SystemDiagnosticTimingInfo::write);
    stream.writeOptional(mSystemCategories, [](BinaryStream& stream, const auto& categories) {
        stream.writeArray(categories, &SystemCategory::write);
    });
    stream.writeArray(mScopeDataSummaries, &ScopeDataSummary::write);
}

Result<> ServerboundDiagnosticsPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readFloat(mAvgFps));
    _SCULK_READ(stream.readFloat(mAvgServerSimTickTimeMS));
    _SCULK_READ(stream.readFloat(mAvgClientSimTickTimeMS));
    _SCULK_READ(stream.readFloat(mAvgBeginFrameTimeMS));
    _SCULK_READ(stream.readFloat(mAvgInputTimeMS));
    _SCULK_READ(stream.readFloat(mAvgRenderTimeMS));
    _SCULK_READ(stream.readFloat(mAvgEndFrameTimeMS));
    _SCULK_READ(stream.readFloat(mAvgRemainderTimePercent));
    _SCULK_READ(stream.readFloat(mAvgUnaccountedTimePercent));
    _SCULK_READ(stream.readArray(mMemoryCategoryValues, &MemoryCategoryCounter::read));
    _SCULK_READ(stream.readArray(mEntityDiagnostics, &EntityDiagnosticTimingInfo::read));
    _SCULK_READ(stream.readArray(mSystemDiagnostics, &SystemDiagnosticTimingInfo::read));
    _SCULK_READ(stream.readOptional(mSystemCategories, [](ReadOnlyBinaryStream& stream, auto& categories) {
        return stream.readArray(categories, &SystemCategory::read);
    }));
    return stream.readArray(mScopeDataSummaries, &ScopeDataSummary::read);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ServerboundDiagnosticsPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mAvgFps),
        SCULK_FORMAT_FIELD(mAvgServerSimTickTimeMS),
        SCULK_FORMAT_FIELD(mAvgClientSimTickTimeMS),
        SCULK_FORMAT_FIELD(mAvgBeginFrameTimeMS),
        SCULK_FORMAT_FIELD(mAvgInputTimeMS),
        SCULK_FORMAT_FIELD(mAvgRenderTimeMS),
        SCULK_FORMAT_FIELD(mAvgEndFrameTimeMS),
        SCULK_FORMAT_FIELD(mAvgRemainderTimePercent),
        SCULK_FORMAT_FIELD(mAvgUnaccountedTimePercent),
        SCULK_FORMAT_FIELD(mMemoryCategoryValues),
        SCULK_FORMAT_FIELD(mEntityDiagnostics),
        SCULK_FORMAT_FIELD(mSystemDiagnostics),
        SCULK_FORMAT_FIELD(mSystemCategories),
        SCULK_FORMAT_FIELD(mScopeDataSummaries)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
