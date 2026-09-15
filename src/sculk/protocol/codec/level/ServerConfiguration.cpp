// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/level/ServerConfiguration.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void GatheringsConfigurationJoinInfo::write(BinaryStream& stream) const {
    mExperienceId.write(stream);
    stream.writeString(mExperienceName);
    stream.writeOptional(mExperienceWorldId, &UUID::write);
    stream.writeOptional(mExperienceWorldName, &BinaryStream::writeString);
    stream.writeString(mCreatorId);
    stream.writeOptional(mTargetId, &UUID::write);
    stream.writeOptional(mScenarioId, &BinaryStream::writeString);
    stream.writeOptional(mServerId, &BinaryStream::writeString);
}

Result<> GatheringsConfigurationJoinInfo::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mExperienceId.read(stream));
    _SCULK_READ(stream.readString(mExperienceName));
    _SCULK_READ(stream.readOptional(mExperienceWorldId, &UUID::read));
    _SCULK_READ(stream.readOptional(mExperienceWorldName, &ReadOnlyBinaryStream::readString));
    _SCULK_READ(stream.readString(mCreatorId));
    _SCULK_READ(stream.readOptional(mTargetId, &UUID::read));
    _SCULK_READ(stream.readOptional(mScenarioId, &ReadOnlyBinaryStream::readString));
    return stream.readOptional(mServerId, &ReadOnlyBinaryStream::readString);
}

void GatheringsConfigurationClientStoreEntryPointInfo::write(BinaryStream& stream) const {
    stream.writeString(mStoreId);
    stream.writeString(mStoreName);
}

Result<> GatheringsConfigurationClientStoreEntryPointInfo::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mStoreId));
    return stream.readString(mStoreName);
}

void ServerConfigurationJoinInfo::write(BinaryStream& stream) const {
    stream.writeOptional(mGatheringsConfiguration, &GatheringsConfigurationJoinInfo::write);
    stream.writeOptional(mStoreEntryPointInfo, &GatheringsConfigurationClientStoreEntryPointInfo::write);
    stream.writeOptional(mPresenceInfo, &PresenceConfiguration::write);
}

Result<> ServerConfigurationJoinInfo::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readOptional(mGatheringsConfiguration, &GatheringsConfigurationJoinInfo::read));
    _SCULK_READ(stream.readOptional(mStoreEntryPointInfo, &GatheringsConfigurationClientStoreEntryPointInfo::read));
    return stream.readOptional(mPresenceInfo, &PresenceConfiguration::read);
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
