// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/CommandRequestPacket.hpp"
#include "../utility/EnumName.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds CommandRequestPacket::getId() const noexcept { return MinecraftPacketIds::CommandRequest; }

std::string_view CommandRequestPacket::getName() const noexcept { return "CommandRequestPacket"; }

void CommandRequestPacket::write(BinaryStream& stream) const {
    stream.writeString(mCommand);
    mOriginData.write(stream);
    stream.writeBool(mIsInternal);
    utils::writeEnumName(stream, mVersion);
}

Result<> CommandRequestPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mCommand));
    _SCULK_READ(mOriginData.read(stream));
    _SCULK_READ(stream.readBool(mIsInternal));
    std::string version{};
    _SCULK_READ(stream.readString(version));
    for (char& c : version) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    // These engine enum names alias another numeric value and are omitted by magic_enum.
    if (version == "latest") {
        mVersion = CurrentCmdVersion::Latest;
    } else if (version == "agentsweepingblocktest") {
        mVersion = CurrentCmdVersion::AgentSweepingBlockTest;
    } else if (version == "commandpositionfix") {
        mVersion = CurrentCmdVersion::CommandPositionFix;
    } else if (auto value = magic_enum::enum_cast<CurrentCmdVersion>(version, magic_enum::case_insensitive)) {
        mVersion = *value;
    } else {
        return error_utils::makeError("Invalid command version name");
    }
    return {};
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string CommandRequestPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mCommand),
        SCULK_FORMAT_FIELD(mOriginData),
        SCULK_FORMAT_FIELD(mIsInternal)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
