// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/RecordStartedPacket.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds RecordStartedPacket::getId() const noexcept { return MinecraftPacketIds::RecordStarted; }

std::string_view RecordStartedPacket::getName() const noexcept { return "RecordStartedPacket"; }

void RecordStartedPacket::write(BinaryStream& stream) const {
    mBlockPos.write(stream);
    stream.writeUnsignedInt64(mServerSoundHandle);
}

Result<> RecordStartedPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mBlockPos.read(stream));
    return stream.readUnsignedInt64(mServerSoundHandle);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string RecordStartedPacket::toString() const {
    return SCULK_FORMAT_PACKET(SCULK_FORMAT_FIELD(mBlockPos), SCULK_FORMAT_FIELD(mServerSoundHandle));
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
