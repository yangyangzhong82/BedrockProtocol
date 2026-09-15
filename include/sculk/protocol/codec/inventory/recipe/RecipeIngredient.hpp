// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/utility/BinaryStream.hpp"
#include "sculk/protocol/utility/ReadOnlyBinaryStream.hpp"
#include <map>
#include <string>

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

// CraftingData uses the descriptor's string map, followed by aux and count.
struct RecipeIngredient {
    std::map<std::string, std::string> mDescriptor{};
    std::int32_t                       mAux{32767};
    std::int32_t                       mStackSize{};

    void                   write(BinaryStream& stream) const;
    [[nodiscard]] Result<> read(ReadOnlyBinaryStream& stream);
};

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
