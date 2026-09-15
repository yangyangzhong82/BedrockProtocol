# Bedrock 协议 2168 适配

默认目标为 **Minecraft Bedrock 26.40 / 协议 2168**，ABI 命名空间为 `sculk::protocol::abi_v2168`。
这是单版本升级，不提供协议 1001 的兼容编解码器。包 ID 沿用原表，主要变化在包体和共享类型。

## 参考版本

| 仓库 | 固定版本与用途 |
| --- | --- |
| [EndstoneMC/bedrock-protocol](https://github.com/EndstoneMC/bedrock-protocol/tree/1dc4408fd949bdf1eec11daa6373dff1a986d02a/protocol) | `1dc4408fd949bdf1eec11daa6373dff1a986d02a`；按 `since/until` 选择 2168 的类型、字段、枚举 |
| [Sandertv/gophertunnel](https://github.com/Sandertv/gophertunnel/tree/4794743244247a9395360bced1f294d4882c4df0/minecraft/protocol) | `4794743244247a9395360bced1f294d4882c4df0`，26.44 / 2168；独立编解码和参考字节生成 |
| [CloudburstMC/Protocol](https://github.com/CloudburstMC/Protocol/tree/9df9864c2bf79197a30ddff72dc0008dca93a976/bedrock-codec/src/main/java/org/cloudburstmc/protocol/bedrock/codec/v2168) | `9df9864c2bf79197a30ddff72dc0008dca93a976` 的 `v2168`、`v2168_hotfix4`；序列化器与热修复对照 |
| [Mojang/bedrock-protocol-docs](https://github.com/Mojang/bedrock-protocol-docs/tree/80f5254176d199507f83a22495cb54eeb4352cd5/json) | `v1.26.40` / `80f5254176d199507f83a22495cb54eeb4352cd5`；协议 2168 的 JSON schema |

参考检出目录位于被忽略的 `build/protocol-references/`。库的正常构建与测试不依赖这些检出目录。

## 主要编码变化

- 实体元数据增加重复类型字节，并校验它与 VarUInt 变体下标一致。更新实体标志、Cushion、SulfurCube 分类、复用元数据 ID、声音、动作和断开原因枚举。
- 物品栈 ID 使用 Int16；空气物品也有数量、aux、可选 net ID、方块 ID 和用户数据长度。net ID 使用单个有符号 VarInt，按符号及奇偶恢复变体。`write/read` 与 `writeCereal/readCereal` 均使用 2168 布局。
- 物品实例仍用 VarInt 物品 ID，空气物品也序列化其余字段。物品用户数据仍是不透明字节串。
- 玩家输入标志从位集编码改为可选的编号列表，支持 `InternalUpdate=65`。交互、物品请求、方块动作和两个载具字段各自有存在标记；不再根据输入标志猜测字段是否存在。
- 物品请求动作带 VarUInt 变体下标和类型字节。下标跳过原类型 7/8。请求槽位 net ID 和挖掘 net ID 为固定 Int32；自动合成次数不再重复，原料数组长度改为 VarUInt。
- 合成包改为八个分类配方列表。有形配方的原料也有数组长度；解锁条件和解锁原料分别可选。配方原料使用字符串映射、aux、count；请求内的原料使用独立的物品描述变体。
- 皮肤的动画/persona 列表长度改为 VarUInt，手臂类型为字节，persona 类型为 UInt32，pack ID 为 UUID。颜色为小端 `0xAARRGGBB`，每条染色记录有四个固定颜色。可信状态移入共享皮肤，并增加 ProfileHash。
- 玩家列表改为逐条 Add/Remove 变体，支持同包混合操作。地图去掉顶层位掩码，各组数据分别可选，像素和装饰颜色为固定 UInt32。
- LevelChunk 使用普通子区块数量、可选请求上限，以及始终存在的缓存元数据列表。SubChunk 中心为三个固定 Int32，数组长度为 VarUInt，数据/高度图/blob ID 各自可选。
- MoveActorDelta 的六个值各有存在标记，后接四个布尔状态；MovePlayer 的传送数据增加存在标记。
- 计分板记录改为逐条变体，身份更新中的玩家 ID 可选。资源包列表长度改为 VarUInt，客户端响应为变体及名称，仅 Downloading 分支携带包 ID 列表。
- StartGame 的教育版 offer 为无符号 VarInt、权限为字节、游戏规则整数为固定 Int32、强制实验选项为 optional；移除聊天日志字段。
- PlaySound 增加有符号 VarInt 循环次数；声音更新包支持七个可选操作变体。维度定义增加 pack UUID，诊断增加系统类别，Transfer 增加 gatherings 信息，Presence 仅保留可选 rich-presence ID。
- 登录属性增加 ProfileHash 和可选 Nonce。连接 LAN 主机时，由调用方填入主机提供的 nonce。

## API 迁移要点

| 旧用法 | 2168 用法 |
| --- | --- |
| `PlayerAuthInputPacket::mInputData` 是 `bitset<65>` | `optional<bitset<66>>`；先 `emplace()` 再设置标志。交互等字段也改为 optional。 |
| 输入交互中的旧槽位列表始终有值 | `mLegacySetItemSlots` 改为 optional，`mHasActions` 保存动作列表存在状态。 |
| `CraftingDataPacket::mCraftingDataEntries` | `mShapedRecipes`、`mShapelessRecipes`、`mMultiRecipes` 等八个分类列表。移除旧联合体和空的熔炉占位类型。 |
| `RecipeIngredient` 的旧描述联合体 | `mDescriptor` 字符串映射、`mAux`、`mStackSize`。例如 `{{"name", "minecraft:stone"}}`，aux=0，count=1；空描述的 aux 通常为 32767。 |
| 请求复用普通物品/原料类型 | `ItemStackRequestItemDescriptor`、`ItemStackRequestIngredient`、`ItemStackRequestItem`。 |
| 物品响应的容器、net ID、脱敏名称为普通值 | `mContainers`、`mNetId`、`mFilteredCustomName` 为 optional。前两者带额外的外层存在标记。 |
| 包级 `PlayerListPacket::mAction` | 每条 `PlayerListEntry::mAction`。可信状态放在 `mSerializedSkin.mTrustedSkinFlag`，值为 `unset`/`false`/`true`。 |
| 皮肤中的字符串手臂类型和颜色 | `ArmSizeType` 和 UInt32；persona 的 pack ID 为 UUID，染色数组固定四项。 |
| 地图的 `mTypeFlag` | 设置对应字段的 optional。 |
| LevelChunk 的可选 `mCacheBlobs` | 普通列表加 `mCacheEnabled`；请求上限为 Int32。 |
| SetScore 的 `mPacketType` | 每条记录的 `mIdentityType`；`Invalid` 表示移除。`mObjectiveName` 为 optional。 |
| Presence 的 experience/world 名称 | 移除，`mRichPresenceId` 改为 optional。 |
| AnvilDamage 的伤害量、PlayerSkin 的尾部可信布尔、StartGame 的聊天日志布尔 | 移除这些字段。 |

## 26.44 的同协议热修复

26.40 和 26.44 都使用 2168，但 **26.44 的 SetScore 移除记录多了一层 optional**。
默认采用 26.40 布局。面向 26.44 时，需要在读取和写入该包之前设置：

```cpp
SetScorePacket packet;
packet.mUse26_44Format = true;
```

此选项不写入网络，也无法仅从协议号推断。调用方可依据登录属性中的游戏版本选择。

## 参考差异

- PlaySound 的循环次数采用官方、Endstone 和 gophertunnel 一致的有符号 VarInt。
- 结构名称和物品响应的脱敏部分，采用官方 `Bedrock::Safety::RedactableString` 和 Endstone 定义的 optional。锁定的 gophertunnel 仍有写成普通字符串的路径，因此这两项使用独立固定字节用例验证。
- 内存类别采用官方 2168 枚举和 Endstone 版本过滤后的顺序，避免继承 gophertunnel 中已退役类别造成的偏移。
- 26.40 的 TextDataPayload JSON、Endstone 2168 定义、gophertunnel 均未写入 line-gap-height，本次使用该布局。

## 验证

`tests/reference/gophertunnel-2168.txt` 是由锁定的 gophertunnel 实际生成的 **48 份包体**。
测试检查完整解码、逐字节重编码一致、拒绝截断数据；另有固定字节、非法变体、net ID 分类、重复/越界输入标志和热修复用例。

```sh
cmake -S . -B build/tests -DPROTOCOL_BUILD_TESTS=ON -DPROTOCOL_ENABLE_AUTHENTICATION=OFF -DPROTOCOL_ENABLE_CONNECTION=OFF
cmake --build build/tests --config Release --parallel
ctest --test-dir build/tests -C Release --output-on-failure
```

参考字节已提交，测试无需 Go 或联网。重新生成时，在固定提交的 gophertunnel 检出目录运行：

```sh
go run /absolute/path/to/BedrockProtocol/tests/reference/generate_gophertunnel.go /absolute/path/to/BedrockProtocol/tests/reference/gophertunnel-2168.txt
```

本次验证为本地 MSVC 构建和协议字节测试，尚未进行真实 Bedrock 客户端登录、进世界及游戏过程联调。应用仍需提供正确的注册表、世界数据和认证上下文。
