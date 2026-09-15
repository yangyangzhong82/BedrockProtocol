// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/inventory/item/NetworkItemStackDescriptor.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void NetworkItemStackDescriptor::write(BinaryStream& stream) const { writeCereal(stream); }

[[nodiscard]] Result<> NetworkItemStackDescriptor::read(ReadOnlyBinaryStream& stream) { return readCereal(stream); }

void NetworkItemStackDescriptor::writeCereal(BinaryStream& stream) const {
    stream.writeSignedShort(mId);
    stream.writeUnsignedShort(mStackSize);
    stream.writeUnsignedVarInt(mAux);
    stream.writeOptional(mNetId, [&](BinaryStream& stream, const NetIdVariant& var) {
        std::visit([&stream](const auto& value) { stream.writeVarInt(value.mId); }, var);
    });
    stream.writeUnsignedVarInt(mBlockRuntimeId);
    stream.writeString(mUserData);
}

[[nodiscard]] Result<> NetworkItemStackDescriptor::readCereal(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readSignedShort(mId));
    _SCULK_READ(stream.readUnsignedShort(mStackSize));
    _SCULK_READ(stream.readUnsignedVarInt(mAux));
    _SCULK_READ(stream.readOptional(mNetId, [&](ReadOnlyBinaryStream& stream, NetIdVariant& var) {
        std::int32_t id{};
        _SCULK_READ(stream.readVarInt(id));
        if (id >= 0) {
            var = ItemStackNetId{id};
        } else if ((id & 1) != 0) {
            var = ItemStackRequestId{id};
        } else {
            var = ItemStackLegacyRequestId{id};
        }
        return Result<>{};
    }));
    _SCULK_READ(stream.readUnsignedVarInt(mBlockRuntimeId));
    return stream.readString(mUserData);
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
