// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/CraftingDataPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds CraftingDataPacket::getId() const noexcept { return MinecraftPacketIds::CraftingData; }

std::string_view CraftingDataPacket::getName() const noexcept { return "CraftingDataPacket"; }

void CraftingDataPacket::write(BinaryStream& stream) const {
    stream.writeArray(mShapedRecipes, &ShapedRecipe::write);
    stream.writeArray(mShapelessRecipes, &ShapelessRecipe::write);
    stream.writeArray(mMultiRecipes, &MultiRecipe::write);
    stream.writeArray(mUserDataShapelessRecipes, &UserDataShapelessRecipe::write);
    stream.writeArray(mShapelessChemistryRecipes, &ShapelessChemistryRecipe::write);
    stream.writeArray(mShapedChemistryRecipes, &ShapedChemistryRecipe::write);
    stream.writeArray(mSmithingTransformRecipes, &SmithingTransformRecipe::write);
    stream.writeArray(mSmithingTrimRecipes, &SmithingTrimRecipe::write);
    stream.writeArray(mPotionMixDataEntries, &PotionMixDataEntry::write);
    stream.writeArray(mContainerMixDataEntries, &ContainerMixDataEntry::write);
    stream.writeArray(mMaterialReducerDataEntries, &MaterialReducerDataEntry::write);
    stream.writeBool(mClearRecipe);
}

Result<> CraftingDataPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readArray(mShapedRecipes, &ShapedRecipe::read));
    _SCULK_READ(stream.readArray(mShapelessRecipes, &ShapelessRecipe::read));
    _SCULK_READ(stream.readArray(mMultiRecipes, &MultiRecipe::read));
    _SCULK_READ(stream.readArray(mUserDataShapelessRecipes, &UserDataShapelessRecipe::read));
    _SCULK_READ(stream.readArray(mShapelessChemistryRecipes, &ShapelessChemistryRecipe::read));
    _SCULK_READ(stream.readArray(mShapedChemistryRecipes, &ShapedChemistryRecipe::read));
    _SCULK_READ(stream.readArray(mSmithingTransformRecipes, &SmithingTransformRecipe::read));
    _SCULK_READ(stream.readArray(mSmithingTrimRecipes, &SmithingTrimRecipe::read));
    _SCULK_READ(stream.readArray(mPotionMixDataEntries, &PotionMixDataEntry::read));
    _SCULK_READ(stream.readArray(mContainerMixDataEntries, &ContainerMixDataEntry::read));
    _SCULK_READ(stream.readArray(mMaterialReducerDataEntries, &MaterialReducerDataEntry::read));
    return stream.readBool(mClearRecipe);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string CraftingDataPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mShapedRecipes),
        SCULK_FORMAT_FIELD(mShapelessRecipes),
        SCULK_FORMAT_FIELD(mMultiRecipes),
        SCULK_FORMAT_FIELD(mUserDataShapelessRecipes),
        SCULK_FORMAT_FIELD(mShapelessChemistryRecipes),
        SCULK_FORMAT_FIELD(mShapedChemistryRecipes),
        SCULK_FORMAT_FIELD(mSmithingTransformRecipes),
        SCULK_FORMAT_FIELD(mSmithingTrimRecipes),
        SCULK_FORMAT_FIELD(mPotionMixDataEntries),
        SCULK_FORMAT_FIELD(mContainerMixDataEntries),
        SCULK_FORMAT_FIELD(mMaterialReducerDataEntries)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
