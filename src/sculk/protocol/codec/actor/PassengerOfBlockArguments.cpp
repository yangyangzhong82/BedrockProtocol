// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/actor/PassengerOfBlockArguments.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void PassengerOfBlockArguments::write(BinaryStream& stream) const {
    mBlockPos.write(stream);
    mOffset.write(stream);
    stream.writeFloat(mRotation);
    stream.writeFloat(mRotationLimit);
    stream.writeEnum(mEmoteType, &BinaryStream::writeByte);
}

Result<> PassengerOfBlockArguments::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(mBlockPos.read(stream));
    _SCULK_READ(mOffset.read(stream));
    _SCULK_READ(stream.readFloat(mRotation));
    _SCULK_READ(stream.readFloat(mRotationLimit));
    return stream.readEnum(mEmoteType, &ReadOnlyBinaryStream::readByte);
}


} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
