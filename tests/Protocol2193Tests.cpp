#include <sculk/protocol/Version.hpp>
#include <sculk/protocol/codec/MinecraftPackets.hpp>
#include <sculk/protocol/codec/actor/MetaData.hpp>
#include <sculk/protocol/codec/actor/player/DisconnectFailReason.hpp>
#include <sculk/protocol/codec/command/CurrentCmdVersion.hpp>
#include <sculk/protocol/codec/inventory/item/ItemStackRequest.hpp>
#include <sculk/protocol/codec/inventory/item/ItemStackResponse.hpp>
#include <sculk/protocol/codec/inventory/item/NetworkItemStackDescriptor.hpp>
#include <sculk/protocol/codec/level/StructureEditorData.hpp>
#include <sculk/protocol/codec/packet/ClientboundAttributeLayerSyncPacket.hpp>
#include <sculk/protocol/codec/packet/ClientboundUpdateSoundDataPacket.hpp>
#include <sculk/protocol/codec/packet/DimensionDataPacket.hpp>
#include <sculk/protocol/codec/packet/MoveActorDeltaPacket.hpp>
#include <sculk/protocol/codec/packet/PlayerAuthInputPacket.hpp>
#include <sculk/protocol/codec/packet/PlayerListPacket.hpp>
#include <sculk/protocol/codec/packet/RecordStartedPacket.hpp>
#include <sculk/protocol/codec/packet/ServerboundDiagnosticsPacket.hpp>
#include <sculk/protocol/codec/packet/ServerboundPackSettingChangePacket.hpp>
#include <sculk/protocol/codec/packet/SetPlayerFurnaceOptionsPacket.hpp>
#include <sculk/protocol/codec/packet/SetScorePacket.hpp>
#include <sculk/protocol/codec/packet/StartGamePacket.hpp>
#include <sculk/protocol/codec/packet/SubChunkPacket.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace sculk::protocol;

namespace {
void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

std::vector<std::byte> unhex(std::string_view hex) {
    require(hex.size() % 2 == 0, "Odd hex length");
    std::vector<std::byte> bytes;
    for (std::size_t i = 0; i < hex.size(); i += 2) {
        bytes.push_back(static_cast<std::byte>(std::stoul(std::string(hex.substr(i, 2)), nullptr, 16)));
    }
    return bytes;
}

template <typename T>
std::vector<std::byte> encode(const T& value) {
    std::vector<std::byte> bytes;
    BinaryStream           stream(bytes);
    value.write(stream);
    return bytes;
}

template <typename T>
void rejects(std::string_view hex, const std::string& name) {
    auto                 bytes = unhex(hex);
    ReadOnlyBinaryStream stream(bytes);
    T                    value;
    require(!value.read(stream), name + " accepted malformed data");
}

template <typename T>
void checkGolden(const T& value, std::string_view hex, const std::string& name) {
    const auto bytes = unhex(hex);
    require(encode(value) == bytes, name + ": encoding");
    T                    decoded;
    ReadOnlyBinaryStream reader(bytes);
    require(decoded.read(reader).has_value() && !reader.hasDataLeft(), name + ": decoding");
    require(encode(decoded) == bytes, name + ": re-encoding");
    for (std::size_t size = 0; size < bytes.size(); ++size) {
        T                    truncatedValue;
        ReadOnlyBinaryStream truncated(std::span<const std::byte>(bytes).first(size));
        require(!truncatedValue.read(truncated), name + ": accepted truncation");
    }
}

void checkCorpus(const char* path) {
    std::ifstream input(path);
    require(input.good(), std::string("Cannot open corpus: ") + path);
    std::string              line;
    std::size_t              count{};
    std::vector<std::string> failures;
    while (std::getline(input, line)) {
        if (line.empty() || line.front() == '#') {
            continue;
        }
        std::istringstream fields(line);
        std::string        name, hex;
        unsigned           id{};
        require(static_cast<bool>(fields >> name >> id >> hex), "Invalid corpus line");
        auto bytes   = unhex(hex);
        auto created = MinecraftPackets::createPacket(static_cast<MinecraftPacketIds>(id));
        require(created.has_value(), name + ": missing packet factory entry");
        auto&                packet = **created;
        ReadOnlyBinaryStream reader(bytes);
        auto                 result = packet.read(reader);
        if (!result || reader.hasDataLeft()) {
            failures.push_back(
                name + ": decode stopped at " + std::to_string(reader.getPosition()) + "/"
                + std::to_string(bytes.size())
                + (result ? " (trailing bytes)" : " (" + std::string(result.error().mMessage) + ")")
            );
            continue;
        }
        auto output = encode(packet);
        if (output != bytes) {
            auto mismatch = std::mismatch(output.begin(), output.end(), bytes.begin(), bytes.end());
            failures.push_back(
                name + ": encode differs at byte " + std::to_string(mismatch.first - output.begin()) + " ("
                + std::to_string(output.size()) + "/" + std::to_string(bytes.size()) + ")"
            );
        }
        if (name == "start_game") {
            auto& game = static_cast<StartGamePacket&>(packet);
            require(game.mActorUniqueId == -123 && game.mActorRuntimeId == 456, "StartGame actor IDs");
            require(game.mLevelSettings.mEduOffer == 2, "StartGame unsigned education offer");
            require(game.mLevelSettings.mForceExperimentalGameplay == false, "StartGame optional false");
        }
        if (name == "player_list_mixed") {
            auto& players = static_cast<PlayerListPacket&>(packet).mPlayerEntryList;
            require(
                players.size() == 2 && players[0].mAction == PlayerListEntry::ActionType::Add
                    && players[1].mAction == PlayerListEntry::ActionType::Remove,
                "PlayerList per-entry actions"
            );
            require(players[0].mSerializedSkin.mProfileHash == "hash", "Skin profile hash");
        }
        if (name == "input_flags") {
            auto& auth = static_cast<PlayerAuthInputPacket&>(packet);
            require(auth.mInputData.test(65), "Input flag 65");
            require(auth.mClientPredictedVihicle == -12, "Input vehicle ID");
        }
        if (name == "input_transaction") {
            const auto& use = static_cast<PlayerAuthInputPacket&>(packet).mItemUseTransaction;
            require(
                use && use->mItemUseTransaction.mHand == ItemUseInventoryTransaction::HandSlot::OffHand,
                "Input transaction offhand"
            );
            require(use->mItemUseTransaction.mTransaction.mActions.empty(), "Input empty action list");
        }
        if (name == "dimension_custom") {
            const auto& dimensions = static_cast<DimensionDataPacket&>(packet).mDefinitionGroup;
            require(
                dimensions.size() == 1 && dimensions[0].mMinY == -64 && dimensions[0].mHeightRange == 384,
                "Dimension minimum and height range"
            );
            require(dimensions[0].mDefaultBiome == "minecraft:plains", "Dimension default biome");
        }
        if (name == "delta_full") {
            require(static_cast<MoveActorDeltaPacket&>(packet).mTicks == 300, "Movement interpolation ticks");
        }
        if (name == "record_started") {
            const auto& record = static_cast<RecordStartedPacket&>(packet);
            require(
                record.mBlockPos.mX == -123 && record.mBlockPos.mY == -64 && record.mBlockPos.mZ == 300,
                "Record signed block position"
            );
            require(record.mServerSoundHandle == 0x1122334455667788ULL, "Record sound handle");
        }
        // A fresh decoder must reject a truncated mandatory suffix.
        auto                 truncated = MinecraftPackets::createPacket(static_cast<MinecraftPacketIds>(id));
        ReadOnlyBinaryStream shortReader(std::span<const std::byte>(bytes).first(bytes.size() - 1));
        require(!(*truncated)->read(shortReader), name + ": accepted truncated payload");
        ++count;
    }
    for (const auto& failure : failures) {
        std::cerr << failure << '\n';
    }
    require(failures.empty(), std::to_string(failures.size()) + " interoperability failures");
    require(count >= 60, "Incomplete reference corpus");
    std::cout << count << " reference payloads matched and rejected truncation\n";
}

void checkIndependentCases() {
    static_assert(static_cast<int>(CurrentCmdVersion::Latest) == 52);
    static_assert(static_cast<int>(CurrentCmdVersion::Count) == 53);
    static_assert(static_cast<int>(DisconnectFailReason::UnsupportedTransport) == 149);
    static_assert(static_cast<int>(ServerboundDiagnosticsPacket::MemoryCategory::Persona_Characters) == 58);
    static_assert(static_cast<int>(ServerboundDiagnosticsPacket::MemoryCategory::Gameface_Layout) == 109);

    // r26_u5 uses named attribute operations, optional float constraints, and
    // four Int32 colour channels; the pinned gophertunnel differs on these leaves.
    EnvironmentAttributeData environment;
    environment.mAttributeName          = "a";
    environment.mAttribute.mData        = FloatAttributeData{.mValue = 0.75f};
    environment.mCurrentTransitionTicks = 20;
    environment.mTotalTransitionTicks   = 100;
    environment.mEasing                 = EasingFunction::InOutQuad;
    environment.mLocalTransitionTicks   = 7;
    environment.mNoiseTransition        = true;
    environment.mNoiseAlignment.mValue  = 300;
    checkGolden(
        environment,
        "016100010000403f086f76657272696465000000"
        "14000000640000000b696e5f6f75745f71756164070000000100ac02",
        "Environment noise alignment"
    );
    ClientboundAttributeLayerSyncPacket environmentPacket;
    UpdateEnvironmentAttributesData     update;
    update.mAttributeLayerName      = "w";
    update.mAttributeLayerDimension = -1;
    update.mAttributes.push_back(environment);
    environmentPacket.mData.mData = update;
    checkGolden(
        environmentPacket,
        "0201770101016100010000403f086f76657272696465000000"
        "14000000640000000b696e5f6f75745f71756164070000000100ac02",
        "Environment packet"
    );
    checkGolden(
        FloatAttributeData{
            .mValue         = 1,
            .mOperation     = FloatAttributeOperation::ADD,
            .mConstraintMin = -2,
            .mConstraintMax = 3
        },
        "0000803f0361646401000000c00100004040",
        "Float attribute constraints"
    );
    checkGolden(
        ColorAttributeData{
            .mValue     = {1, 2, 3, 255},
            .mOperation = ColorAttributeOperation::ALPHA_BLEND
    },
        "010000000200000003000000ff0000000b616c7068615f626c656e64",
        "Colour attribute channels"
    );

    // r26_u5 schema: diagnostics adds two independent optionals. Cloudburst omits
    // their presence markers, and the pinned gophertunnel does not implement them.
    using Diagnostic = ServerboundDiagnosticsPacket::EntityDiagnosticTimingInfo;
    Diagnostic diagnostic;
    diagnostic.mDisplayName       = "d";
    diagnostic.mEntity            = "e";
    diagnostic.mTimeInNanoseconds = 0x0102030405060708ULL;
    diagnostic.mPercentOfTotal    = 25;
    diagnostic.mPosition          = Vec3{1, -2, 3};
    diagnostic.mDimension         = "dim";
    const auto diagnosticBytes    = unhex("01640165080706050403020119010000803f000000c000004040010364696d");
    require(encode(diagnostic) == diagnosticBytes, "Diagnostic optional position and dimension");
    Diagnostic           decodedDiagnostic;
    ReadOnlyBinaryStream diagnosticReader(diagnosticBytes);
    require(
        decodedDiagnostic.read(diagnosticReader).has_value() && !diagnosticReader.hasDataLeft(),
        "Diagnostic decoding"
    );
    require(
        decodedDiagnostic.mPosition && decodedDiagnostic.mPosition->mY == -2 && decodedDiagnostic.mDimension == "dim",
        "Diagnostic values"
    );
    for (std::size_t size = 0; size < diagnosticBytes.size(); ++size) {
        ReadOnlyBinaryStream truncated(std::span<const std::byte>(diagnosticBytes).first(size));
        Diagnostic           value;
        require(!value.read(truncated), "Diagnostic accepted truncation");
    }
    diagnostic.mPosition.reset();
    diagnostic.mDimension.reset();
    const auto absentDiagnostic = unhex("016401650807060504030201190000");
    require(encode(diagnostic) == absentDiagnostic, "Absent diagnostic fields");
    ReadOnlyBinaryStream absentReader(absentDiagnostic);
    require(
        decodedDiagnostic.read(absentReader).has_value() && !decodedDiagnostic.mPosition
            && !decodedDiagnostic.mDimension,
        "Reused diagnostic clears absent fields"
    );

    ServerboundDiagnosticsPacket diagnostics;
    require(encode(diagnostics) == std::vector<std::byte>(41), "Absent diagnostic system categories");
    diagnostics.mSystemCategories.emplace().push_back({"x", 0x0102030405060708ULL});
    auto       diagnosticsBytes = std::vector<std::byte>(39);
    const auto categoryTail     = unhex("01010178080706050403020100");
    diagnosticsBytes.insert(diagnosticsBytes.end(), categoryTail.begin(), categoryTail.end());
    require(encode(diagnostics) == diagnosticsBytes, "Present diagnostic system categories");
    ServerboundDiagnosticsPacket decodedDiagnostics;
    ReadOnlyBinaryStream         categoriesReader(diagnosticsBytes);
    require(
        decodedDiagnostics.read(categoriesReader).has_value() && !categoriesReader.hasDataLeft()
            && decodedDiagnostics.mSystemCategories && decodedDiagnostics.mSystemCategories->size() == 1,
        "Diagnostic categories decoding"
    );

    rejects<SubChunkPacket::HeightmapData>("01010f", "Height map row too short");
    rejects<SubChunkPacket::HeightmapData>("010111", "Height map row too long");
    rejects<ClientboundUpdateSoundDataPacket>("000000000000000007", "Unknown sound update variant");
    rejects<ServerboundPackSettingChangePacket>("000000000000000000000000000000000004", "Unknown pack setting variant");
    SetPlayerFurnaceOptionsPacket furnace;
    furnace.mFurnaceType    = SetPlayerFurnaceOptionsPacket::FurnaceType::Smoker;
    furnace.mLeftFurnaceTab = SetPlayerFurnaceOptionsPacket::FurnaceLeftTabIndex::RecipeSearch;
    furnace.mFiltering      = true;
    furnace.mLayout         = SetPlayerFurnaceOptionsPacket::FurnaceLayout::Default;
    require(encode(furnace) == unhex("03080104"), "Furnace uses byte and signed varints");

    // Cereal map {"name":"minecraft:stone"}, aux 0, count 3.
    RecipeIngredient ingredient;
    ingredient.mDescriptor.emplace("name", "minecraft:stone");
    ingredient.mAux       = 0;
    ingredient.mStackSize = 3;
    require(encode(ingredient) == unhex("01046e616d650f6d696e6563726166743a73746f6e650006"), "Crafting ingredient map");

    // Stack request descriptors are a different union (uint tag + byte tag + uint16 count).
    ItemStackRequestIngredient requested;
    requested.mItemDescriptor.mDescriptor = ItemStackRequestItemDescriptor::ItemName{"minecraft:stone", 0};
    requested.mStackSize                  = 3;
    require(encode(requested) == unhex("01010f6d696e6563726166743a73746f6e65000300"), "Request ingredient union");
    rejects<ItemStackRequestItemDescriptor>("0100", "Descriptor discriminator mismatch");
    rejects<ItemStackRequestAction>("0707", "Removed container action");
    rejects<ItemStackRequestAction>("0809", "Action discriminator mismatch");
    rejects<ItemStackRequestAction>("ff01ff", "Unknown action");
    rejects<MetaData>("0100010000", "Actor metadata discriminator mismatch");

    // Signed net IDs retain their semantic alternative without an additional union tag.
    for (const auto [raw, index] : {
             std::pair{300, 0u},
             {-3,  1u},
             {-4,  2u}
    }) {
        NetworkItemStackDescriptor item;
        item.mId                         = 2;
        item.mStackSize                  = 1;
        item.mNetId                      = ItemStackNetId{raw};
        auto                       bytes = encode(item);
        NetworkItemStackDescriptor decoded;
        ReadOnlyBinaryStream       reader(bytes);
        require(decoded.read(reader).has_value() && !reader.hasDataLeft(), "Item net ID decoding");
        require(decoded.mNetId && decoded.mNetId->index() == index, "Item net ID alternative");
        require(encode(decoded) == bytes, "Item net ID encoding");
    }
    NetworkItemStackDescriptor air;
    require(encode(air) == unhex("0000000000000000"), "Air stack includes its fixed fields");

    // 2193 uses one optional for score removal.
    SetScorePacket            scores;
    SetScorePacket::ScoreInfo removal;
    removal.mScoreboardId  = 7;
    removal.mObjectiveName = "a";
    scores.mScoresInfo.push_back(removal);
    require(encode(scores) == unhex("01000672656d6f76650e010161"), "26.50 score removal");

    // Endstone's RedactableString and Mojang's 2193 schema have an optional redacted string.
    ItemStackResponseSlotInfo responseSlot;
    responseSlot.mRequestedSlot      = 1;
    responseSlot.mSlot               = 2;
    responseSlot.mAmount             = 3;
    responseSlot.mNetId              = 4;
    responseSlot.mCustomName         = "a";
    responseSlot.mFilteredCustomName = "b";
    responseSlot.mDurationCorrection = -1;
    const auto slotBytes             = unhex("0102030108016101016201");
    require(encode(responseSlot) == slotBytes, "Response slot optional net ID and redacted name");
    ItemStackResponseSlotInfo decodedSlot;
    ReadOnlyBinaryStream      slotReader(slotBytes);
    require(decodedSlot.read(slotReader).has_value() && !slotReader.hasDataLeft(), "Response slot decoding");
    require(decodedSlot.mFilteredCustomName == "b" && decodedSlot.mDurationCorrection == -1, "Response slot values");

    StructureEditorData structure;
    structure.mStructureName         = "a";
    structure.mFilteredStructureName = "b";
    structure.mDataField             = "c";
    auto       structureBytes        = encode(structure);
    const auto structurePrefix       = unhex("01610101620163");
    require(
        std::equal(structurePrefix.begin(), structurePrefix.end(), structureBytes.begin()),
        "Structure redacted-name prefix"
    );
    StructureEditorData  decodedStructure;
    ReadOnlyBinaryStream structureReader(structureBytes);
    require(
        decodedStructure.read(structureReader).has_value() && !structureReader.hasDataLeft(),
        "Structure data decoding"
    );
    require(
        decodedStructure.mFilteredStructureName == "b" && decodedStructure.mDataField == "c",
        "Structure redacted name values"
    );

    // The header remains a 10-bit packet ID plus two 2-bit sub-client IDs.
    std::vector<std::byte> header;
    BinaryStream           writer(header);
    MinecraftPackets::writePacketHeader(writer, {MinecraftPacketIds::StartGame, 2, 3});
    ReadOnlyBinaryStream reader(header);
    auto                 parsed = MinecraftPackets::readPacketHeader(reader);
    require(
        parsed && parsed->mPacketId == MinecraftPacketIds::StartGame && parsed->mSenderSubClientId == 2
            && parsed->mTargetSubClientId == 3,
        "Packet header with sub-clients"
    );

    // Duplicate and out-of-range input flags must fail before decoding the trailing fields.
    std::vector<std::byte> invalid(32);
    const auto             duplicate = unhex("020000");
    invalid.insert(invalid.end(), duplicate.begin(), duplicate.end());
    PlayerAuthInputPacket auth;
    ReadOnlyBinaryStream  duplicateReader(invalid);
    auto                  duplicateResult = auth.read(duplicateReader);
    require(
        !duplicateResult && duplicateResult.error().mMessage == "Invalid or duplicate player input flag",
        "Duplicate input flags"
    );
    invalid.resize(32);
    const auto outOfRange = unhex("018401"); // flag 66, signed VarInt
    invalid.insert(invalid.end(), outOfRange.begin(), outOfRange.end());
    ReadOnlyBinaryStream invalidReader(invalid);
    auto                 invalidResult = auth.read(invalidReader);
    require(
        !invalidResult && invalidResult.error().mMessage == "Invalid or duplicate player input flag",
        "Out-of-range input flag"
    );
}
} // namespace

int main(int argc, char** argv) {
    static_assert(getProtocolVersion() == 2193);
    static_assert(getMinecraftVersion() == "26.50");
    try {
        require(argc == 2, "Usage: Protocol2193Tests <reference-corpus>");
        checkCorpus(argv[1]);
        checkIndependentCases();
        std::cout << "Protocol 2193 checks passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
