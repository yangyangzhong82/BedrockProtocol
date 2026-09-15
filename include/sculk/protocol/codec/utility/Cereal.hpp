// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/protocol/utility/BinaryStream.hpp"
#include "sculk/protocol/utility/ReadOnlyBinaryStream.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

// Protocol 2168 wraps some optional projections in an additional optional.
// Writers emit the outer value; readers also accept an absent outer value.
template <typename T, typename F>
void writeDoubleOptional(BinaryStream& stream, const std::optional<T>& value, F&& write) {
    stream.writeBool(true);
    stream.writeOptional(value, std::forward<F>(write));
}

template <typename T, typename F>
Result<> readDoubleOptional(ReadOnlyBinaryStream& stream, std::optional<T>& value, F&& read) {
    bool present{};
    _SCULK_READ(stream.readBool(present));
    if (!present) {
        value.reset();
        return {};
    }
    return stream.readOptional(value, std::forward<F>(read));
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
