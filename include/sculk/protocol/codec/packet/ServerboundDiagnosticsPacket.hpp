// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
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
        Persona_Textures                        = 58,
        Persona_Characters                      = 59,
        Persona_SkinPacks                       = 60,
        Persona_Repo                            = 61,
        Player                                  = 62,
        RenderChunk                             = 63,
        RenderChunk_IndexBuffer                 = 64,
        RenderChunk_VertexBuffer                = 65,
        Rendering                               = 66,
        Rendering_BgfxInit                      = 67,
        Rendering_BgfxStartFrame                = 68,
        Rendering_BlockTessellator              = 69,
        Rendering_EndFrame                      = 70,
        Rendering_GraphicsTasksInit             = 71,
        Rendering_Library                       = 72,
        Rendering_PolygonOperatorPool           = 73,
        Rendering_PBRTextureData                = 74,
        Rendering_RenderRegistry                = 75,
        Rendering_Setup                         = 76,
        Rendering_Vertices                      = 77,
        RequestLog                              = 78,
        ResourcePacks                           = 79,
        Sound                                   = 80,
        SubChunk_BiomeData                      = 81,
        SubChunk_BlockData                      = 82,
        SubChunk_LightData                      = 83,
        Textures                                = 84,
        WeatherRenderer                         = 85,
        World_Generator                         = 86,
        Tasks                                   = 87,
        Test                                    = 88,
        Test_LoadTestTags                       = 89,
        Scripting                               = 90,
        Scripting_Runtime                       = 91,
        Scripting_Context                       = 92,
        Scripting_Context_Bindings_MC           = 93,
        Scripting_Context_Bindings_GT           = 94,
        Scripting_Context_Run                   = 95,
        DataDrivenUI                            = 96,
        DataDrivenUI_Defs                       = 97,
        Gameface                                = 98,
        Gameface_System                         = 99,
        Gameface_DOM                            = 100,
        Gameface_CSS                            = 101,
        Gameface_Display                        = 102,
        Gameface_TempAllocator                  = 103,
        Gameface_PoolAllocator                  = 104,
        Gameface_Dump                           = 105,
        Gameface_Media                          = 106,
        Gameface_JSON                           = 107,
        Gameface_ScriptEngine                   = 108,
        Gameface_Script                         = 109,
        Gameface_Layout                         = 110,
    };

    struct MemoryCategoryCounter {
        MemoryCategory mType{};
        std::uint64_t  mCurrentBytes{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

    struct EntityDiagnosticTimingInfo {
        std::string   mDisplayName{};
        std::string   mEntity{};
        std::uint64_t mTimeInNanoseconds{};
        std::uint8_t  mPercentOfTotal{};

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
    float                                   mAvgFps{};
    float                                   mAvgServerSimTickTimeMS{};
    float                                   mAvgClientSimTickTimeMS{};
    float                                   mAvgBeginFrameTimeMS{};
    float                                   mAvgInputTimeMS{};
    float                                   mAvgRenderTimeMS{};
    float                                   mAvgEndFrameTimeMS{};
    float                                   mAvgRemainderTimePercent{};
    float                                   mAvgUnaccountedTimePercent{};
    std::vector<MemoryCategoryCounter>      mMemoryCategoryValues{};
    std::vector<EntityDiagnosticTimingInfo> mEntityDiagnostics{};
    std::vector<SystemDiagnosticTimingInfo> mSystemDiagnostics{};
    std::vector<SystemCategory>             mSystemCategories{};
    std::vector<ScopeDataSummary>           mScopeDataSummaries{};

public:
    [[nodiscard]] MinecraftPacketIds getId() const noexcept override;

    [[nodiscard]] std::string_view getName() const noexcept override;

    void write(BinaryStream& stream) const override;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream) override;

    SCULK_PROTOCOL_PACKET_TO_STRING()
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(ServerboundDiagnosticsPacket::MemoryCategory, 0, 110)

SCULK_PROTOCOL_PACKET_FORMATTER(ServerboundDiagnosticsPacket)