#include <sculk/protocol/Version.hpp>
#include <sculk/protocol/codec/MinecraftPackets.hpp>
#include <sculk/protocol/codec/actor/MetaData.hpp>
#include <sculk/protocol/codec/inventory/item/ItemStackRequest.hpp>
#include <sculk/protocol/codec/inventory/item/ItemStackResponse.hpp>
#include <sculk/protocol/codec/inventory/item/NetworkItemStackDescriptor.hpp>
#include <sculk/protocol/codec/level/StructureEditorData.hpp>
#include <sculk/protocol/codec/packet/PlayerAuthInputPacket.hpp>
#include <sculk/protocol/codec/packet/PlayerListPacket.hpp>
#include <sculk/protocol/codec/packet/SetScorePacket.hpp>
#include <sculk/protocol/codec/packet/StartGamePacket.hpp>

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
        auto& packet = **created;
        if (name.ends_with("hotfix4")) {
            require(packet.getId() == MinecraftPacketIds::SetScore, "Invalid hotfix fixture");
            static_cast<SetScorePacket&>(packet).mUse26_44Format = true;
        }
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
            require(auth.mInputData && auth.mInputData->test(65), "Input flag 65");
            require(auth.mClientPredictedVihicle == -12, "Input vehicle ID");
        }
        // A fresh decoder must reject a truncated mandatory suffix.
        auto truncated = MinecraftPackets::createPacket(static_cast<MinecraftPacketIds>(id));
        if (name.ends_with("hotfix4")) {
            static_cast<SetScorePacket&>(**truncated).mUse26_44Format = true;
        }
        ReadOnlyBinaryStream shortReader(std::span<const std::byte>(bytes).first(bytes.size() - 1));
        require(!(*truncated)->read(shortReader), name + ": accepted truncated payload");
        ++count;
    }
    for (const auto& failure : failures) {
        std::cerr << failure << '\n';
    }
    require(failures.empty(), std::to_string(failures.size()) + " interoperability failures");
    require(count >= 40, "Incomplete reference corpus");
    std::cout << count << " reference payloads matched and rejected truncation\n";
}

void checkIndependentCases() {
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

    // The base 26.40 removal has one optional; 26.44 has two despite the same protocol ID.
    SetScorePacket            scores;
    SetScorePacket::ScoreInfo removal;
    removal.mScoreboardId  = 7;
    removal.mObjectiveName = "a";
    scores.mScoresInfo.push_back(removal);
    require(encode(scores) == unhex("01000672656d6f76650e010161"), "26.40 score removal");
    scores.mUse26_44Format = true;
    require(encode(scores) == unhex("01000672656d6f76650e01010161"), "26.44 score removal");

    // Endstone's RedactableString and Mojang's 2168 schema have an optional redacted string.
    ItemStackResponseSlotInfo responseSlot;
    responseSlot.mRequestedSlot      = 1;
    responseSlot.mSlot               = 2;
    responseSlot.mAmount             = 3;
    responseSlot.mNetId              = 4;
    responseSlot.mCustomName         = "a";
    responseSlot.mFilteredCustomName = "b";
    responseSlot.mDurationCorrection = -1;
    const auto slotBytes             = unhex("010203010108016101016201");
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
    const auto             duplicate = unhex("01020000");
    invalid.insert(invalid.end(), duplicate.begin(), duplicate.end());
    PlayerAuthInputPacket auth;
    ReadOnlyBinaryStream  duplicateReader(invalid);
    auto                  duplicateResult = auth.read(duplicateReader);
    require(
        !duplicateResult && duplicateResult.error().mMessage == "Invalid or duplicate player input flag",
        "Duplicate input flags"
    );
    invalid.resize(32);
    const auto outOfRange = unhex("01018401"); // flag 66, signed VarInt
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
    static_assert(getProtocolVersion() == 2168);
    static_assert(getMinecraftVersion() == "26.40");
    try {
        require(argc == 2, "Usage: Protocol2168Tests <reference-corpus>");
        checkCorpus(argv[1]);
        checkIndependentCases();
        std::cout << "Protocol 2168 checks passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
