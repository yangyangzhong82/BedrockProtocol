// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/actor/player/UUID.hpp"
#include "sculk/protocol/codec/packet/IPacket.hpp"
#include "sculk/protocol/utility/Enum.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

class DimensionDataPacket : public IPacket {
public:
    struct DimensionDefinition {
        enum class GeneratorType : std::uint8_t {
            Legacy    = 0,
            Overworld = 1,
            Flat      = 2,
            Nether    = 3,
            TheEnd    = 4,
            Void      = 5,
            Undefined = 6,
        };

        std::string   mName{};
        std::int32_t  mMinY{};
        std::int32_t  mHeightRange{};
        GeneratorType mGeneratorType{};
        std::int32_t  mDimensionType{};
        UUID          mPackId{};
        std::string   mDefaultBiome{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

public:
    std::vector<DimensionDefinition> mDefinitionGroup{};

public:
    [[nodiscard]] MinecraftPacketIds getId() const noexcept override;

    [[nodiscard]] std::string_view getName() const noexcept override;

    void write(BinaryStream& stream) const override;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream) override;

    SCULK_PROTOCOL_PACKET_TO_STRING()
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(DimensionDataPacket::DimensionDefinition::GeneratorType, 0, 6)

SCULK_PROTOCOL_PACKET_FORMATTER(DimensionDataPacket)
