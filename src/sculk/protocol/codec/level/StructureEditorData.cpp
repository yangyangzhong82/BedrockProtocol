// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "sculk/protocol/codec/level/StructureEditorData.hpp"

namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE {

void StructureEditorData::write(BinaryStream& stream) const {
    stream.writeString(mStructureName);
    stream.writeOptional(mFilteredStructureName, &BinaryStream::writeString);
    stream.writeString(mDataField);
    stream.writeBool(mIncludePlayer);
    stream.writeBool(mShowBoundingBox);
    stream.writeVarInt(mStructureBlockType);
    mStructureSettings.write(stream);
    stream.writeByte(mRedstoneSaveMode);
}

Result<> StructureEditorData::read(ReadOnlyBinaryStream& stream) {
    _SCULK_READ(stream.readString(mStructureName));
    _SCULK_READ(stream.readOptional(mFilteredStructureName, &ReadOnlyBinaryStream::readString));
    _SCULK_READ(stream.readString(mDataField));
    _SCULK_READ(stream.readBool(mIncludePlayer));
    _SCULK_READ(stream.readBool(mShowBoundingBox));
    _SCULK_READ(stream.readVarInt(mStructureBlockType));
    _SCULK_READ(mStructureSettings.read(stream));
    return stream.readByte(mRedstoneSaveMode);
}

} // namespace sculk::protocol::SCULK_ABI_INLINE_NAMESPACE
