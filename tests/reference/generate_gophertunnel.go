// Generate the interoperability corpus using gophertunnel commit
// 7a556a07335b663744b50d38062636ad8283f314 (protocol 2193, Minecraft 26.50).
// Run from that checkout: go run /path/to/this/file.go /path/to/gophertunnel-2193.txt
package main

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"image/color"
	"os"

	"github.com/go-gl/mathgl/mgl32"
	"github.com/google/uuid"
	p "github.com/sandertv/gophertunnel/minecraft/protocol"
	pk "github.com/sandertv/gophertunnel/minecraft/protocol/packet"
)

func main() {
	if p.CurrentProtocol != 2193 || p.CurrentVersion != "1.26.50" {
		panic("wrong reference version")
	}
	out, err := os.Create(os.Args[1])
	if err != nil {
		panic(err)
	}
	defer out.Close()
	fmt.Fprintln(out, "# gophertunnel 7a556a07335b663744b50d38062636ad8283f314; name packet-id body-hex")
	emit := func(name string, packet pk.Packet) {
		var buf bytes.Buffer
		packet.Marshal(p.NewWriter(&buf, 0))
		fmt.Fprintf(out, "%s %d %s\n", name, packet.ID(), hex.EncodeToString(buf.Bytes()))
	}
	emit("start_game", &pk.StartGame{EntityUniqueID: -123, EntityRuntimeID: 456, EducationEditionOffer: 2,
		PlayerPermissions: 2, ForceExperimentalGameplay: p.Option(false), GameVersion: "1.26.50",
		GameRules: []p.GameRule{{Name: "randomtickspeed", Value: uint32(300)}}, PropertyData: map[string]any{}})
	emit("boss", &pk.BossEvent{BossEntityUniqueID: -12, EventType: 5, BossBarTitle: "boss", FilteredBossBarTitle: "filtered", HealthPercentage: 0.5, Colour: 3, Overlay: 2})
	emit("furnace_options", &pk.SetPlayerFurnaceOptions{FurnaceType: pk.FurnaceTypeSmoker,
		FurnaceOptions: p.FurnaceOptions{LeftFurnaceTab: p.FurnaceLeftTabRecipeSearch, Filtering: true, Layout: p.FurnaceLayoutDefault}})
	emit("record_started", &pk.RecordStarted{Position: p.BlockPos{-123, -64, 300}, Handle: 0x1122334455667788})
	emit("anvil", &pk.AnvilDamage{AnvilPosition: p.BlockPos{-12, -64, 31}})
	emit("creative_empty", &pk.CreativeContent{})
	emit("crafting_empty", &pk.CraftingData{})
	emit("dimension_empty", &pk.DimensionData{})
	emit("map_empty", &pk.ClientBoundMapItemData{})
	emit("map_texture", &pk.ClientBoundMapItemData{MapID: -900, Width: p.Option(int32(2)), Height: p.Option(int32(1)),
		XOffset: p.Option(int32(3)), YOffset: p.Option(int32(4)), Scale: p.Option(byte(2)),
		Pixels: p.Option([]color.RGBA{{R: 1, G: 2, B: 3, A: 255}, {R: 50, G: 60, B: 70, A: 80}})})
	emit("chunk_uncached", &pk.LevelChunk{Position: p.ChunkPos{-4, 300}, SubChunkCount: 2, RawPayload: []byte{1, 2, 3}})
	emit("chunk_request", &pk.LevelChunk{Position: p.ChunkPos{2, -3}, SubChunkLimit: p.Option(int32(-1)), CacheEnabled: true, BlobHashes: []uint64{0x0123456789abcdef}})
	emit("subchunk_empty", &pk.SubChunk{Position: p.SubChunkPos{300, -4, -700}})
	var heights p.HeightMap
	for z := range heights {
		for x := range heights[z] {
			heights[z][x] = int8(z*16 + x)
		}
	}
	emit("subchunk_full", &pk.SubChunk{CacheEnabled: true, Position: p.SubChunkPos{-16, 9, 40}, SubChunkEntries: []p.SubChunkEntry{
		{Offset: p.SubChunkOffset{-1, 2, 3}, Result: p.SubChunkResultSuccess, RawPayload: p.Option([]byte{0xab, 0xcd}),
			HeightMapType: p.HeightMapDataHasData, HeightMapData: p.Option(heights),
			RenderHeightMapType: p.HeightMapDataHasData, RenderHeightMapData: p.Option(heights), BlobHash: p.Option(uint64(0x8877665544332211))},
		{Result: p.SubChunkResultSuccessAllAir, HeightMapType: p.HeightMapDataTooHigh}}})
	emit("subchunk_request", &pk.SubChunkRequest{})
	emit("move_normal", &pk.MovePlayer{EntityRuntimeID: 99, Position: mgl32.Vec3{1, 2, 3}, Tick: 456})
	emit("move_teleport", &pk.MovePlayer{EntityRuntimeID: 99, Mode: 2, TeleportData: p.Option(p.TeleportData{}), Tick: 999})
	emit("delta_empty", &pk.MoveActorDelta{EntityRuntimeID: 17})
	emit("delta_full", &pk.MoveActorDelta{EntityRuntimeID: 17, PositionX: p.Option(float32(4)), PositionY: p.Option(float32(-9)),
		PositionZ: p.Option(float32(30)), RotationX: p.Option(float32(90)), RotationY: p.Option(float32(180)),
		RotationYHead: p.Option(float32(270)), OnGround: true, ForceMove: true, ForceMoveLocalEntity: true, ForceCompletion: true, Ticks: 300})
	emit("input_empty", &pk.PlayerAuthInput{})
	emit("input_flags", &pk.PlayerAuthInput{InputData: p.NewInputFlagsFromIDs(pk.InputFlagCount, []int32{0, 32, 64, 65}),
		InteractionModel: 2, Tick: 1000, VehicleRotation: p.Option(mgl32.Vec2{1, 2}), ClientPredictedVehicle: p.Option(int64(-12))})
	emit("input_transaction", &pk.PlayerAuthInput{InputData: p.NewInputFlagsFromIDs(pk.InputFlagCount, []int32{34}),
		ItemInteractionData: p.Option(p.UseItemTransactionData{LegacyRequestID: -2, LegacySetItemSlots: p.Option([]p.LegacySetItemSlot{}),
			Actions: []p.InventoryAction{}, Hand: p.HandSlotOffHand, ActionType: 1, BlockPosition: p.BlockPos{1, 2, 3}})})
	emit("input_transaction_no_actions", &pk.PlayerAuthInput{ItemInteractionData: p.Option(p.UseItemTransactionData{ActionType: 1})})
	id := uuid.MustParse("00112233-4455-6677-8899-aabbccddeeff")
	for i, value := range []any{float32(1.25), true, "quality", []string{"low", "high"}} {
		emit(fmt.Sprintf("pack_setting_%d", i), &pk.ServerBoundPackSettingChange{PackID: id, PackSetting: p.PackSetting{Name: "setting", Value: value}})
	}
	emit("camera_presets", &pk.CameraPresets{Presets: []p.CameraPreset{
		{Name: "custom:camera", Parent: "minecraft:free", ApplyInheritedStartingRotation: true, StartingRotation: p.Option(mgl32.Vec2{25, -90})},
		{Name: "custom:empty"}}})
	// Attribute operations and diagnostics differ from r26_u5 in this reference.
	// Those are tested using independent schema-derived bytes in Protocol2193Tests.cpp.
	emit("play_sound_default", &pk.PlaySound{})
	emit("sound_update_default", &pk.ClientboundUpdateSoundData{})
	emit("input_block_actions", &pk.PlayerAuthInput{BlockActions: p.Option([]p.PlayerBlockAction{
		{Action: p.PlayerActionInteractWithBlock, BlockPos: p.BlockPos{-12, -64, 300}, Face: 5}}),
		ItemStackRequest: p.Option(p.ItemStackRequest{RequestID: -7})})
	emit("shape_text", &pk.PrimitiveShapes{Shapes: []p.PrimitiveShape{{NetworkID: 300, ExtraShapeData: &p.TextShape{
		Text: "first\nsecond", UseRotation: true, LineGapHeight: 1.25, DepthTest: true, ShowBackface: true}}}})
	for i, source := range []p.InventoryAction{
		{SourceType: 0, WindowID: p.Option(int8(-1)), InventorySlot: 2},
		{SourceType: 2, SourceFlags: p.Option(uint32(1)), InventorySlot: 3},
	} {
		emit(fmt.Sprintf("transaction_%d", i), &pk.InventoryTransaction{Actions: []p.InventoryAction{source},
			TransactionData: &p.UseItemTransactionData{ActionType: 1, Hand: p.HandSlotOffHand, BlockPosition: p.BlockPos{-1, -64, 300}}})
	}
	emit("item_response_slots", &pk.ItemStackResponse{Responses: []p.ItemStackResponse{{RequestID: -7,
		ContainerInfo: []p.StackResponseContainerInfo{{SlotInfo: []p.StackResponseSlotInfo{
			{Slot: 1, HotbarSlot: 2, Count: 3, StackNetworkID: 123, CustomName: "raw", FilteredCustomName: p.Option("filtered"), DurabilityCorrection: -1},
			{Slot: 4, CustomName: "plain"}}}}}}})
	emit("dimension_custom", &pk.DimensionData{Definitions: []p.DimensionDefinition{{Name: "custom:test", MinimumY: -64, HeightRange: 384, DefaultBiome: "minecraft:plains", Generator: 1, DimensionType: 1000, PackID: id}}})
	emit("transfer_gathering", &pk.Transfer{Address: "localhost", Port: 19132, GatheringJoinInfo: p.Option(p.GatheringJoinInfo{
		ExperienceID: id, ExperienceName: "experience", ExperienceWorldID: p.Option(id), ExperienceWorldName: p.Option("world"),
		CreatorID: "creator", TargetID: p.Option(id), ScenarioID: p.Option("scenario"), ServerID: p.Option("server")})})
	emit("command_2193", &pk.CommandRequest{CommandLine: "say test", Version: "timeofcommandoverloads"})
	skin := p.Skin{SkinID: "skin", ArmSize: 1, SkinColour: color.RGBA{R: 1, G: 2, B: 3, A: 4}, Trusted: true, ProfileHash: "hash",
		Animations:       []p.SkinAnimation{{AnimationType: 3, ExpressionType: 1, FrameCount: 2}},
		PersonaPieces:    []p.PersonaPiece{{PieceID: "piece", PieceType: 3, PackID: id}},
		PieceTintColours: []p.PersonaPieceTintColour{{PieceType: "persona_hand", Colours: [4]color.RGBA{{R: 1, A: 255}, {G: 2, A: 255}, {B: 3, A: 255}, {A: 4}}}}}
	emit("skin", &pk.PlayerSkin{UUID: id, Skin: skin, NewSkinName: "new", OldSkinName: "old"})
	emit("player_list_mixed", &pk.PlayerList{Entries: []p.PlayerListEntry{
		{ActionType: p.PlayerListActionAdd, UUID: id, EntityUniqueID: -1, Username: "tester", Skin: skin},
		{ActionType: p.PlayerListActionRemove, UUID: id}}})
	emit("player_location_hidden", &pk.PlayerLocation{EntityUniqueID: -1, Type: 1})
	emit("entity_override_int", &pk.PlayerUpdateEntityOverrides{EntityUniqueID: -1, PropertyIndex: 2, Type: 2, IntValue: 0x12345678})
	emit("play_sound", &pk.PlaySound{SoundName: "note.harp", LoopCount: -1, BypassListenerRangeCheck: true, PlaybackPositionSeconds: p.Option(float32(1.5)), Handle: p.Option(uint64(0x1122334455667788))})
	emit("sound_update", &pk.ClientboundUpdateSoundData{ServerSoundHandle: 19,
		Fade:  p.SoundDataUpdate{Type: p.SoundDataUpdateFade, Duration: 1.25, TargetVolume: 0.5},
		Pause: p.SoundDataUpdate{Type: p.SoundDataUpdatePause}})
	emit("resource_response_cancel", &pk.ResourcePackClientResponse{Response: 1})
	emit("resource_response_download", &pk.ResourcePackClientResponse{Response: 2, PacksToDownload: []string{"pack_1", "pack_2"}})
	emit("resource_info", &pk.ResourcePacksInfo{})
	emit("score_mixed", &pk.SetScore{Entries: []p.ScoreboardEntry{
		{IdentityType: p.ScoreboardIdentityRemove, EntryID: 12, ObjectiveName: "objective"},
		{IdentityType: p.ScoreboardIdentityFakePlayer, EntryID: 13, ObjectiveName: "objective", DisplayName: "line", Score: 12345}}})
	emit("score_identity", &pk.SetScoreboardIdentity{Entries: []p.ScoreboardIdentityEntry{{EntryID: -3, EntityUniqueID: p.Option(int64(-9))}, {EntryID: 42}}})
	emit("transfer", &pk.Transfer{Address: "localhost", Port: 19132})
	emit("item_request_empty", &pk.ItemStackRequest{})
	slot := p.StackRequestSlotInfo{Slot: 3, StackNetworkID: -3}
	take := &p.TakeStackRequestAction{}
	take.Count = 2
	take.Source = slot
	take.Destination = slot
	place := &p.PlaceStackRequestAction{}
	place.Count = 3
	place.Source = slot
	place.Destination = slot
	emit("item_request_actions", &pk.ItemStackRequest{Requests: []p.ItemStackRequest{{RequestID: -7, FilterCause: 2,
		Actions: []p.StackRequestAction{take, place, &p.SwapStackRequestAction{Source: slot, Destination: slot},
			&p.DropStackRequestAction{Count: 1, Source: slot, Randomly: true}, &p.DestroyStackRequestAction{Count: 2, Source: slot},
			&p.ConsumeStackRequestAction{}, &p.CreateStackRequestAction{ResultsSlot: 3}, &p.LabTableCombineStackRequestAction{},
			&p.BeaconPaymentStackRequestAction{PrimaryEffect: 1, SecondaryEffect: 5},
			&p.MineBlockStackRequestAction{HotbarSlot: 2, PredictedDurability: 300, StackNetworkID: -9},
			&p.CraftRecipeStackRequestAction{RecipeNetworkID: 300, NumberOfCrafts: 5},
			&p.AutoCraftRecipeStackRequestAction{RecipeNetworkID: 301, NumberOfCrafts: 3, Ingredients: []p.ItemDescriptorCount{
				{Descriptor: &p.DefaultItemDescriptor{Name: "minecraft:stone", MetadataValue: 1}, Count: 2},
				{Descriptor: &p.MoLangItemDescriptor{Expression: "q.any_tag('wood')", Version: 12}, Count: 1},
				{Descriptor: &p.ItemTagItemDescriptor{Tag: "minecraft:logs"}, Count: 4}}},
			&p.CraftCreativeStackRequestAction{CreativeItemNetworkID: 400, NumberOfCrafts: 7},
			&p.CraftRecipeOptionalStackRequestAction{RecipeNetworkID: 500, FilterStringIndex: -1},
			&p.CraftGrindstoneRecipeStackRequestAction{RecipeNetworkID: 999, NumberOfCrafts: 2, Cost: -2},
			&p.CraftLoomRecipeStackRequestAction{Pattern: "stripe", TimesCrafted: 3}, &p.CraftNonImplementedStackRequestAction{},
			&p.CraftResultsDeprecatedStackRequestAction{ResultItems: []p.StackRequestItem{{Identifier: "minecraft:stone", Count: 1}}, TimesCrafted: 1}},
		FilterStrings: []string{"rename"}}}})
	ing := p.ItemDescriptorCount{Descriptor: &p.DefaultItemDescriptor{Name: "minecraft:stone", MetadataValue: 0}, Count: 1}
	output := p.ItemStack{ItemType: p.ItemType{NetworkID: 1}, Count: 2}
	emit("creative_group", &pk.CreativeContent{Groups: []p.CreativeGroup{{Category: 3, Name: "items", Icon: output}}, Items: []p.CreativeItem{{CreativeItemNetworkID: 1, Item: output, GroupIndex: 0}}})
	shaped := p.ShapedRecipe{RecipeID: "shaped", Width: 1, Height: 1, Input: []p.ItemDescriptorCount{ing}, Output: []p.ItemStack{output},
		Block: "crafting_table", RecipeNetworkID: 1, UnlockRequirement: p.Option(p.RecipeUnlockRequirement{Context: 0, Ingredients: []p.ItemDescriptorCount{ing}})}
	shapeless := p.ShapelessRecipe{RecipeID: "shapeless", Input: []p.ItemDescriptorCount{ing}, Output: []p.ItemStack{output}, Block: "crafting_table", RecipeNetworkID: 2}
	emit("crafting_recipes", &pk.CraftingData{ShapedRecipes: []p.ShapedRecipe{shaped}, ShapelessRecipes: []p.ShapelessRecipe{shapeless},
		MultiRecipes: []p.MultiRecipe{{UUID: id, RecipeNetworkID: 3}}, UserDataShapelessRecipes: []p.UserDataShapelessRecipe{{ShapelessRecipe: shapeless}},
		ShapelessChemistryRecipes: []p.ShapelessChemistryRecipe{{ShapelessRecipe: shapeless}}, ShapedChemistryRecipes: []p.ShapedChemistryRecipe{{ShapedRecipe: shaped}}, ClearRecipes: true})
	emit("item_response_empty", &pk.ItemStackResponse{})
	emit("inventory_slot_air", &pk.InventorySlot{})
	emit("mob_equipment_air", &pk.MobEquipment{})
	emit("mob_armour_air", &pk.MobArmourEquipment{})
	emit("add_player", &pk.AddPlayer{})
	emit("add_actor", &pk.AddActor{EntityMetadata: p.EntityMetadata{0: int64(123), 4: "actor"}})
	emit("add_item_actor", &pk.AddItemActor{})
	emit("set_actor_data", &pk.SetActorData{EntityMetadata: p.EntityMetadata{1: int16(1234), 2: int32(-123), 3: byte(42)}})
}
