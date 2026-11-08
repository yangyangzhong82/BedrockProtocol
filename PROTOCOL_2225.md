# Bedrock 1.26.60 / 协议 2225

基于 `2193` 分支的 26.50 实现升级，ABI 为 `sculk::protocol::abi_v2225`。
本分支只编解码 2225；使用方需要重新编译，并使用 26.60 的物品、方块和世界数据。

## 新数据包

| ID | 数据包 |
| --- | --- |
| 353 | ClientboundMatchmakingStatePacket |
| 354 | ServerboundStonecutterSetRecipePacket |
| 355 | ClientboundStonecutterSetRecipePacket |
| 356 | ServerboundMatchmakingCancelPacket |
| 357 | SetPassengerOfBlockPacket |
| 358 | ServerboundCursorItemDragPacket |
| 359 | ClientboundPlayAudioContentPacket |
| 360 | ServerboundRegisterAudioContentPacket |

以上数据包均注册进 `MinecraftPackets` 工厂，提供读写与可选格式化支持。
音频内容中的签名数据按 compact JWT 字符串传输；播放类型使用 `Music` / `Sound` 字符串，
嵌套 `mPlaySound` 仅编码包体。

## 编码和 API 变化

- `AddActorPacket`、`AddPlayerPacket` 增加 `mPassengerBlockData`，与新包共用 `PassengerOfBlockArguments`。
- `AnimatePacket`、`ItemUseOnActorInventoryTransaction`、`ItemReleaseInventoryTransaction` 增加 `mHand`。
  共用 `HandSlot`；原有 `ItemUseInventoryTransaction::HandSlot` 名称仍作为别名保留。
- `LevelSettings` 在 `mIsExportedFromEditor` 后增加 `mEditorLevelMigrationVersion`。
- `DimensionDefinition` 追加 `mCloudHeight`、`mRenderClouds`；`LevelChunkPacket` 追加 `mIsClientBiomeUpdate`。
- `SerializedSkin::mPlayFabId` 移到 `PlayerListEntry::mPlayFabId`，位于 XUID 后。
  登录 JWT 的 PlayFab 字段不受影响。
- 物品栈请求动作 18 为 `CraftReservedAction`，负载使用 `CraftDiscPress`（保留 ID 字符串和次数）。
  `CraftNonImplemented`、`CraftResults` 顺延为 19、20，外层变体编号同步更新。
- `ClientboundUpdateSoundDataPacket` 的七个事件字段替换为单个 `mEvent`。
- `EnvironmentAttributeData` 的旧字段替换为 `mPayload`：
  `ConstantAttributeData`、`TransitionAttributeData` 或 `NoiseTransitionAttributeData`。
  过渡时长、当前时刻、缓动编号改为 VarUInt；新增时钟名称。
  噪声名称从 `AttributeLayerData` 移入 `NoiseTransitionSettingsData`。
  层设置的 `mWeight` 改为直接编码的 `float`，移除旧的 `AttributeLayerWeight` 变体包装。
- 更新实体、断开原因、容器、皮肤部件、声音、成就、诊断内存类别和命令版本枚举。
  命令 `Latest=55`、`Count=56`；尚无正式名称的 53–55 保留为 `Unknown53`–`Unknown55`。

## 参考与编码差异

- [Endstone protocol-docs r26_u6](https://github.com/EndstoneMC/protocol-docs/tree/a4672659d5d607cae9285d0b49aeee6a89f04565)：
  1.26.63.1 / 2225，与 1.26.60 同协议；相对 `r26_u5` 对照包、类型和枚举。
- [Endstone bedrock-protocol](https://github.com/EndstoneMC/bedrock-protocol/tree/ec394c9a437386448f60f33a7610ca3fb90fa089)：交叉核对 `since=2225` 的类型定义。
- [gophertunnel](https://github.com/Sandertv/gophertunnel/tree/0e179fc4bb082c13e946c7d81ab1cf9f623d942c)：1.26.60 / 2225 的读写实现。

声音事件采用 Endstone 转储的 `VarUInt 变体编号 + UInt8 事件类型 + 负载`，
解码检查两种标记一致；gophertunnel 此时仅编码 UInt8 类型。
环境过渡的缓动编号采用转储定义的 VarUInt；gophertunnel 此时使用有符号 VarInt。
环境属性叶类型沿用已有的字符串操作名、可选浮点约束及 RGBA 四个 Int32 分量，
与 Endstone 转储一致。

## 验证范围

现有测试迁移为 `Protocol2225Tests`，使用固定 gophertunnel 提交生成的 78 份 2225 包体，
覆盖新包、方块乘坐可选数据、手部字段、PlayFab ID、云和群系设置、动作编号变化。
空包使用 `-` 表示包体；非空包继续检查截断拒绝。
声音事件的七种操作及环境属性的三种负载使用 Endstone 转储推导的固定字节，
不采用参考库中存在差异的这些编码。其他既有回归用例保留。
Windows AMD64 CI 继续启用测试，并添加 `60` 分支触发。

```sh
cmake -S . -B build/protocol2225-cmake -DPROTOCOL_BUILD_TESTS=ON -DPROTOCOL_ENABLE_AUTHENTICATION=OFF -DPROTOCOL_ENABLE_CONNECTION=OFF -DPROTOCOL_ENABLE_FORMATTING=ON -DPROTOCOL_ENABLE_DETAIL_ERRORS=ON
cmake --build build/protocol2225-cmake --config Release --parallel
ctest --test-dir build/protocol2225-cmake -C Release --output-on-failure
```

参考字节已纳入源码；运行 C++ 测试无需 Go 或网络。重新生成时，从上述固定 gophertunnel 检出目录执行：

```sh
go run /absolute/path/to/BedrockProtocol/tests/reference/generate_gophertunnel.go /absolute/path/to/BedrockProtocol/tests/reference/gophertunnel-2225.txt
```

本地 MSVC Release 编解码模块构建通过，已开启格式化和详细错误选项；
`ctest` 的 `Protocol2225` 测试通过，包含 78 份参考包体及上述独立用例。
尚未进行真实 1.26.60 客户端联调。
