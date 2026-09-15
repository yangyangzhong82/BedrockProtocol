// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/codec/actor/player/UUID.hpp"
#include "sculk/protocol/codec/inventory/item/NetworkItemInstanceDescriptor.hpp"
#include "sculk/protocol/codec/inventory/recipe/RecipeIngredient.hpp"
#include "sculk/protocol/utility/Enum.hpp"
#include <string>
#include <variant>
#include <vector>

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

struct RecipeUnlockingRequirement {
    enum class UnlockingContext : std::uint8_t {
        None               = 0,
        AlwaysUnlocked     = 1,
        PlayerInWater      = 2,
        PlayerHasManyItems = 3,
    };

    UnlockingContext                             mUnlockingContext{};
    std::optional<std::vector<RecipeIngredient>> mUnlockingIngredients{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct ShapelessRecipe {
    std::string                                mRecipeUniqueId{};
    std::vector<RecipeIngredient>              mIngredientList{};
    std::vector<NetworkItemInstanceDescriptor> mProductionList{};
    UUID                                       mRecipeId{};
    std::string                                mRecipeTag{};
    std::int32_t                               mPriority{};
    std::optional<RecipeUnlockingRequirement>  mUnlockingRequirement{};
    std::uint32_t                              mNetId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct ShapedRecipe {
    std::string                                mRecipeUniqueId{};
    std::int32_t                               mGridWidth{};
    std::int32_t                               mGridHeight{};
    std::vector<RecipeIngredient>              mIngredientList{};
    std::vector<NetworkItemInstanceDescriptor> mProductionList{};
    UUID                                       mRecipeId{};
    std::string                                mRecipeTag{};
    std::int32_t                               mPriority{};
    bool                                       mAssumeSymmetry{};
    std::optional<RecipeUnlockingRequirement>  mUnlockingRequirement{};
    std::uint32_t                              mNetId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct MultiRecipe {
    UUID          mMultiRecipe{};
    std::uint32_t mNetId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct UserDataShapelessRecipe {
    std::string                                mRecipeUniqueId{};
    std::vector<RecipeIngredient>              mIngredientList{};
    std::vector<NetworkItemInstanceDescriptor> mProductionList{};
    UUID                                       mRecipeId{};
    std::string                                mRecipeTag{};
    std::int32_t                               mPriority{};
    std::optional<RecipeUnlockingRequirement>  mUnlockingRequirement{};
    std::uint32_t                              mNetId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct ShapelessChemistryRecipe {
    std::string                                mRecipeUniqueId{};
    std::vector<RecipeIngredient>              mIngredientList{};
    std::vector<NetworkItemInstanceDescriptor> mProductionList{};
    UUID                                       mRecipeId{};
    std::string                                mRecipeTag{};
    std::int32_t                               mPriority{};
    std::optional<RecipeUnlockingRequirement>  mUnlockingRequirement{};
    std::uint32_t                              mNetId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct ShapedChemistryRecipe {
    ShapedRecipe mChemistryRecipe{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct SmithingTransformRecipe {
    std::string                   mRecipeUniqueId{};
    RecipeIngredient              mTemplate{};
    RecipeIngredient              mBase{};
    RecipeIngredient              mAddition{};
    NetworkItemInstanceDescriptor mResult{};
    std::string                   mRecipeTag{};
    std::uint32_t                 mNetId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct SmithingTrimRecipe {
    std::string      mRecipeUniqueId{};
    RecipeIngredient mTemplate{};
    RecipeIngredient mBase{};
    RecipeIngredient mAddition{};
    std::string      mRecipeTag{};
    std::uint32_t    mNetId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct PotionMixDataEntry {
    std::int32_t mFromPotionId{};
    std::int32_t mFromPotionAux{};
    std::int32_t mReagentItemId{};
    std::int32_t mReagentItemAux{};
    std::int32_t mToPotionId{};
    std::int32_t mToPotionAux{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct ContainerMixDataEntry {
    std::int32_t mInputItemId{};
    std::int32_t mReagentItemId{};
    std::int32_t mToItemId{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

struct MaterialReducerDataEntry {
    struct ItemInfo {
        std::int32_t mItemId{};
        std::int32_t mItemCount{};

        void write(BinaryStream& stream) const;

        [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
    };

    std::int32_t          mInput{};
    std::vector<ItemInfo> mItems{};

    void write(BinaryStream& stream) const;

    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE

SCULK_PROTOCOL_ENUM_RANGE(RecipeUnlockingRequirement::UnlockingContext, 0, 3)
