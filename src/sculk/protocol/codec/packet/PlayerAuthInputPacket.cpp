// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/packet/PlayerAuthInputPacket.hpp"
#include "sculk/protocol/codec/utility/Cereal.hpp"
#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
#include "../utility/Format.hpp"
#endif

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

MinecraftPacketIds PlayerAuthInputPacket::getId() const noexcept { return MinecraftPacketIds::PlayerAuthInput; }

std::string_view PlayerAuthInputPacket::getName() const noexcept { return "PlayerAuthInputPacket"; }

void PlayerAuthInputPacket::write(BinaryStream& stream) const {
    mPlayerRotation.write(stream);
    mPosition.write(stream);
    mMoveVector.write(stream);
    stream.writeFloat(mPlayerHeadRotation);
    stream.writeOptional(mInputData, [](BinaryStream& stream, const std::bitset<66>& flags) {
        stream.writeUnsignedVarInt(static_cast<std::uint32_t>(flags.count()));
        for (std::size_t i = 0; i < flags.size(); ++i) {
            if (flags.test(i)) {
                stream.writeVarInt(static_cast<std::int32_t>(i));
            }
        }
    });
    stream.writeUnsignedVarInt(mInputType);
    stream.writeUnsignedVarInt(mPlayMode);
    stream.writeVarInt(mNewInteractionModel);
    mInteractRotation.write(stream);
    stream.writeUnsignedVarInt64(mClientTick);
    mPosDelta.write(stream);
    writeDoubleOptional(stream, mItemUseTransaction, &PackedItemUseLegacyInventoryTransaction::write);
    writeDoubleOptional(stream, mItemStackRequestData, &ItemStackRequestData::write);
    writeDoubleOptional(stream, mPlayerBlockActions, &PlayerBlockActions::write);
    writeDoubleOptional(stream, mVehicleRotation, &Vec2::write);
    writeDoubleOptional(stream, mClientPredictedVihicle, &BinaryStream::writeVarInt64);
    mAnologMoveVector.write(stream);
    mCameraOrientation.write(stream);
    mRawMoveVector.write(stream);
}

Result<> PlayerAuthInputPacket::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mPlayerRotation.read(stream));
    _SCULK_READ(mPosition.read(stream));
    _SCULK_READ(mMoveVector.read(stream));
    _SCULK_READ(stream.readFloat(mPlayerHeadRotation));
    _SCULK_READ(stream.readOptional(mInputData, [](ReadOnlyBinaryStream& stream, std::bitset<66>& flags) -> Result<> {
        flags.reset();
        std::uint32_t count{};
        _SCULK_READ(stream.readUnsignedVarInt(count));
        if (count > flags.size()) {
            return error_utils::makeError("Too many player input flags");
        }
        for (std::uint32_t i = 0; i < count; ++i) {
            std::int32_t flag{};
            _SCULK_READ(stream.readVarInt(flag));
            if (flag < 0 || static_cast<std::size_t>(flag) >= flags.size() || flags.test(flag)) {
                return error_utils::makeError("Invalid or duplicate player input flag");
            }
            flags.set(flag);
        }
        return {};
    }));
    _SCULK_READ(stream.readUnsignedVarInt(mInputType));
    _SCULK_READ(stream.readUnsignedVarInt(mPlayMode));
    _SCULK_READ(stream.readVarInt(mNewInteractionModel));
    _SCULK_READ(mInteractRotation.read(stream));
    _SCULK_READ(stream.readUnsignedVarInt64(mClientTick));
    _SCULK_READ(mPosDelta.read(stream));
    _SCULK_READ(readDoubleOptional(stream, mItemUseTransaction, &PackedItemUseLegacyInventoryTransaction::read));
    _SCULK_READ(readDoubleOptional(stream, mItemStackRequestData, &ItemStackRequestData::read));
    _SCULK_READ(readDoubleOptional(stream, mPlayerBlockActions, &PlayerBlockActions::read));
    _SCULK_READ(readDoubleOptional(stream, mVehicleRotation, &Vec2::read));
    _SCULK_READ(readDoubleOptional(stream, mClientPredictedVihicle, &ReadOnlyBinaryStream::readVarInt64));
    _SCULK_READ(mAnologMoveVector.read(stream));
    _SCULK_READ(mCameraOrientation.read(stream));
    return mRawMoveVector.read(stream);
}

#ifdef SCULK_PROTOCOL_ENABLE_FORMATTING
std::string PlayerAuthInputPacket::toString() const {
    return SCULK_FORMAT_PACKET(
        SCULK_FORMAT_FIELD(mPlayerRotation),
        SCULK_FORMAT_FIELD(mPosition),
        SCULK_FORMAT_FIELD(mMoveVector),
        SCULK_FORMAT_FIELD(mPlayerHeadRotation),
        SCULK_FORMAT_FIELD(mInputData),
        SCULK_FORMAT_FIELD(mInputType),
        SCULK_FORMAT_FIELD(mPlayMode),
        SCULK_FORMAT_FIELD(mNewInteractionModel),
        SCULK_FORMAT_FIELD(mInteractRotation),
        SCULK_FORMAT_FIELD(mClientTick),
        SCULK_FORMAT_FIELD(mPosDelta),
        SCULK_FORMAT_FIELD(mItemUseTransaction),
        SCULK_FORMAT_FIELD(mItemStackRequestData),
        SCULK_FORMAT_FIELD(mPlayerBlockActions),
        SCULK_FORMAT_FIELD(mVehicleRotation),
        SCULK_FORMAT_FIELD(mClientPredictedVihicle),
        SCULK_FORMAT_FIELD(mAnologMoveVector),
        SCULK_FORMAT_FIELD(mCameraOrientation),
        SCULK_FORMAT_FIELD(mRawMoveVector)
    );
}
#endif

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
