// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/actor/player/UUID.hpp"
#include "sculk/protocol/codec/level/PresenceConfiguration.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

struct GatheringsConfigurationJoinInfo {
    UUID                       mExperienceId{};
    std::string                mExperienceName{};
    std::optional<UUID>        mExperienceWorldId{};
    std::optional<std::string> mExperienceWorldName{};
    std::string                mCreatorId{};
    std::optional<UUID>        mTargetId{};
    std::optional<std::string> mScenarioId{};
    std::optional<std::string> mServerId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct GatheringsConfigurationClientStoreEntryPointInfo {
    std::string mStoreId{};
    std::string mStoreName{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct ServerConfigurationJoinInfo {
    std::optional<GatheringsConfigurationJoinInfo>                  mGatheringsConfiguration{};
    std::optional<GatheringsConfigurationClientStoreEntryPointInfo> mStoreEntryPointInfo{};
    std::optional<PresenceConfiguration>                            mPresenceInfo{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
