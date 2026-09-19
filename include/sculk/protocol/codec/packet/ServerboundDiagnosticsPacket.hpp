// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/math/Vec3.hpp"
#include "sculk/protocol/codec/packet/IPacket.hpp"
#include "sculk/protocol/utility/Enum.hpp"
#include <vector>

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

class ServerboundDiagnosticsPacket : public IPacket {
public:
public:
    enum class MemoryCategory : std::uint8_t {
        Unknown                                 = 0,
        Invalid_SizeUnknown                     = 1,
        Actor                                   = 2,
        ActorAnimation                          = 3,
        ActorRendering                          = 4,
        BlockTickingQueues                      = 5,
        Biome_Storage                           = 6,
        Blobs                                   = 7,
        Cereal                                  = 8,
        CircuitSystem                           = 9,
        Client                                  = 10,
        Commands                                = 11,
        DBStorage                               = 12,
        Debug                                   = 13,
        Documentation                           = 14,
        ECSSystems                              = 15,
        FMOD                                    = 16,
        Fonts                                   = 17,
        ImGui                                   = 18,
        Input                                   = 19,
        JsonUI                                  = 20,
        JsonUI_ControlFactory_Json              = 21,
        JsonUI_ControlTree                      = 22,
        JsonUI_ControlTree_ControlElement       = 23,
        JsonUI_ControlTree_PopulateDataBinding  = 24,
        JsonUI_ControlTree_PopulateFocus        = 25,
        JsonUI_ControlTree_PopulateLayout       = 26,
        JsonUI_ControlTree_PopulateOther        = 27,
        JsonUI_ControlTree_PopulateSprite       = 28,
        JsonUI_ControlTree_PopulateText         = 29,
        JsonUI_ControlTree_PopulateTTS          = 30,
        JsonUI_ControlTree_Visibility           = 31,
        JsonUI_CreateUI                         = 32,
        JsonUI_Defs                             = 33,
        JsonUI_LayoutManager                    = 34,
        JsonUI_LayoutManager_RemoveDependencies = 35,
        JsonUI_LayoutManager_InitVariable       = 36,
        Languages                               = 37,
        Level                                   = 38,
        LevelStructures                         = 39,
        LevelChunk                              = 40,
        LevelChunkGen                           = 41,
        LevelChunkGenThreadLocal                = 42,
        LightVolumeManager                      = 43,
        Network                                 = 44,
        Marketplace                             = 45,
        Material_DragonCompiledDefinition       = 46,
        Material_DragonMaterial                 = 47,
        Material_DragonResource                 = 48,
        Material_DragonUniformMap               = 49,
        Material_RenderMaterial                 = 50,
        Material_RenderMaterialGroup            = 51,
        Material_VariationManager               = 52,
        Molang                                  = 53,
        OreUI                                   = 54,
        OreUI_Client                            = 55,
        Persona_Pieces                          = 56,
        Persona_Animations                      = 57,
        Persona_Characters                      = 58,
        Persona_SkinPacks                       = 59,
        Persona_Repo                            = 60,
        Player                                  = 61,
        RenderChunk                             = 62,
        RenderChunk_IndexBuffer                 = 63,
        RenderChunk_VertexBuffer                = 64,
        Rendering                               = 65,
        Rendering_BgfxInit                      = 66,
        Rendering_BgfxStartFrame                = 67,
        Rendering_BlockTessellator              = 68,
        Rendering_EndFrame                      = 69,
        Rendering_GraphicsTasksInit             = 70,
        Rendering_Library                       = 71,
        Rendering_PolygonOperatorPool           = 72,
        Rendering_PBRTextureData                = 73,
        Rendering_RenderRegistry                = 74,
        Rendering_Setup                         = 75,
        Rendering_Vertices                      = 76,
        RequestLog                              = 77,
        ResourcePacks                           = 78,
        Sound                                   = 79,
        SubChunk_BiomeData                      = 80,
        SubChunk_BlockData                      = 81,
        SubChunk_LightData                      = 82,
        Textures                                = 83,
        WeatherRenderer                         = 84,
        World_Generator                         = 85,
        Tasks                                   = 86,
        Test                                    = 87,
        Test_LoadTestTags                       = 88,
        Scripting                               = 89,
        Scripting_Runtime                       = 90,
        Scripting_Context                       = 91,
        Scripting_Context_Bindings_MC           = 92,
        Scripting_Context_Bindings_GT           = 93,
        Scripting_Context_Run                   = 94,
        DataDrivenUI                            = 95,
        DataDrivenUI_Defs                       = 96,
        Gameface                                = 97,
        Gameface_System                         = 98,
        Gameface_DOM                            = 99,
        Gameface_CSS                            = 100,
        Gameface_Display                        = 101,
        Gameface_TempAllocator                  = 102,
        Gameface_PoolAllocator                  = 103,
        Gameface_Dump                           = 104,
        Gameface_Media                          = 105,
        Gameface_JSON                           = 106,
        Gameface_ScriptEngine                   = 107,
        Gameface_Script                         = 108,
        Gameface_Layout                         = 109,
    };

    struct MemoryCategoryCounter {
        MemoryCategory mType{};
        std::uint64_t  mCurrentBytes{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

    struct EntityDiagnosticTimingInfo {
        std::string                mDisplayName{};
        std::string                mEntity{};
        std::optional<Vec3>        mPosition{};
        std::optional<std::string> mDimension{};
        std::uint64_t              mTimeInNanoseconds{};
        std::uint8_t               mPercentOfTotal{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

    struct SystemDiagnosticTimingInfo {
        std::string   mDisplayName{};
        std::uint64_t mSystemIndex{};
        std::uint64_t mTimeInNanoseconds{};
        std::uint8_t  mPercentOfTotal{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

    struct SystemCategory {
        std::string            mCategoryName{};
        std::uint64_t          mSystemIndex{};
        void                   write(BinaryStream& stream) const;
        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

    struct ScopeDataSummary {
        std::string   mLabel{};
        std::string   mIndentation{};
        std::uint64_t mTotalHighCostNS{};
        std::uint64_t mTotalMidCostNS{};
        std::uint64_t mTotalLowCostNS{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

public:
    float                                      mAvgFps{};
    float                                      mAvgServerSimTickTimeMS{};
    float                                      mAvgClientSimTickTimeMS{};
    float                                      mAvgBeginFrameTimeMS{};
    float                                      mAvgInputTimeMS{};
    float                                      mAvgRenderTimeMS{};
    float                                      mAvgEndFrameTimeMS{};
    float                                      mAvgRemainderTimePercent{};
    float                                      mAvgUnaccountedTimePercent{};
    std::vector<MemoryCategoryCounter>         mMemoryCategoryValues{};
    std::vector<EntityDiagnosticTimingInfo>    mEntityDiagnostics{};
    std::vector<SystemDiagnosticTimingInfo>    mSystemDiagnostics{};
    std::optional<std::vector<SystemCategory>> mSystemCategories{};
    std::vector<ScopeDataSummary>              mScopeDataSummaries{};

public:
    [[nodiscard]] MinecraftPacketIds getId() const noexcept override;

    [[nodiscard]] std::string_view getName() const noexcept override;

    void write(BinaryStream& stream) const override;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream) override;

    SCULK_PROTOCOL_PACKET_TO_STRING()
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(ServerboundDiagnosticsPacket::MemoryCategory, 0, 109)

SCULK_PROTOCOL_PACKET_FORMATTER(ServerboundDiagnosticsPacket)
