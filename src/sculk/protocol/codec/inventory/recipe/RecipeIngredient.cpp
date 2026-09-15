// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/inventory/recipe/RecipeIngredient.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void RecipeIngredient::write(BinaryStream& stream) const {
    stream.writeMap(mDescriptor, [](BinaryStream& stream, const std::string& key, const std::string& value) {
        stream.writeString(key);
        stream.writeString(value);
    });
    stream.writeVarInt(mAux);
    stream.writeVarInt(mStackSize);
}

Result<> RecipeIngredient::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readMap(mDescriptor, [](ReadOnlyBinaryStream& stream, std::string& key, std::string& value) {
        _SCULK_READ(stream.readString(key));
        return stream.readString(value);
    }));
    _SCULK_READ(stream.readVarInt(mAux));
    _SCULK_READ(stream.readVarInt(mStackSize));
    if (mAux < -32768 || mAux > 32767 || mStackSize < 0 || mStackSize > 65535) {
        return error_utils::makeError("Recipe ingredient value out of range");
    }
    return {};
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
