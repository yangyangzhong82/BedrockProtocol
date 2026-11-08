// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/ServerboundCursorItemDragPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds ServerboundCursorItemDragPacket::getId() const noexcept {
    return MinecraftPacketIds::ServerboundCursorItemDrag;
}

std::string_view ServerboundCursorItemDragPacket::getName() const noexcept { return "ServerboundCursorItemDragPacket"; }

void ServerboundCursorItemDragPacket::write(BinaryStream& stream) const {
    stream.writeEnum(mState, &BinaryStream::writeByte);
}

Result<> ServerboundCursorItemDragPacket::read(ReadOnlyBinaryStream& stream) {
    return stream.readEnum(mState, &ReadOnlyBinaryStream::readByte);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string ServerboundCursorItemDragPacket::toString() const {
    return SCULK_FORMAT_PACKET(SCULK_FORMAT_FIELD(mState));
}
#endif


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
