# Bedrock 26.50 / 协议 2193

当前目标为 **Minecraft Bedrock 26.50 / 协议 2193**，ABI 命名空间为 `sculk::protocol::abi_v2193`。
这是从 2168 的单版本升级，使用方需要重新编译；不提供 2168 或预览协议 2192 的协商兼容层。

## 参考版本

| 来源 | 固定提交与用途 |
| --- | --- |
| [EndstoneMC/protocol-docs](https://github.com/EndstoneMC/protocol-docs/tree/0db6185921974b5aba4a30af33a5eed842dd2cb9) | `0db6185921974b5aba4a30af33a5eed842dd2cb9`，`r26_u5`，26.51.1 / 2193；与 `r26_u4` 的字段、类型、枚举转储逐项对照。同属 26.50 的 2193 协议。 |
| [EndstoneMC/bedrock-protocol](https://github.com/EndstoneMC/bedrock-protocol/tree/e15ec630d3d1fb983e8601571e54b59a886896c4) | `e15ec630d3d1fb983e8601571e54b59a886896c4`；按 `since/until` 过滤出 2193，并参考正式字段名和命令版本。未引入 2211 的后续布局或包。 |
| [Sandertv/gophertunnel](https://github.com/Sandertv/gophertunnel/tree/7a556a07335b663744b50d38062636ad8283f314) | `7a556a07335b663744b50d38062636ad8283f314`，26.50 / 2193；实际生成互操作参考字节。 |
| [CloudburstMC/Protocol](https://github.com/CloudburstMC/Protocol/tree/97eae1a3a89bcfa6d0a8958f02f1808ea1bcb807/bedrock-codec/src/main/java/org/cloudburstmc/protocol/bedrock/codec/v2193) | `97eae1a3a89bcfa6d0a8958f02f1808ea1bcb807`；交叉检查序列化器，包含维度高度范围修正。 |

参考检出位于被忽略的 `build/protocol-references/`；正常构建和测试不依赖它们。

## 编码变化

- 新增 `SetPlayerFurnaceOptionsPacket`（351，双向）和 `RecordStartedPacket`（352，服务端到客户端），注册进数据包工厂。
- 玩家输入标志列表始终存在，去掉外层存在标记。交互、物品请求、方块动作和载具字段各使用一层 optional。
- 库存交易去掉类型和动作列表前的固定 true 标记；交易来源的容器 ID / flags、物品响应的容器列表 / net ID 去掉多余外层 optional。交互交易在槽位后增加单字节手部枚举。
- 子区块高度图仍为 16×16；每一行现在先写 VarUInt 长度 16，解码器拒绝其他行长度。
- 维度定义依次写入最低 Y 和高度范围，末尾新增默认群系字符串。最大边界为 `mMinY + mHeightRange`，不再传旧的最大/最小高度对。
- BossEvent 移除玩家 ID，事件、颜色和覆盖样式使用单字节；MoveActorDelta 追加 VarUInt64 插值 ticks。
- 相机预设追加继承起始旋转开关和可选起始旋转。调试文字形状追加 Float32 行距。
- PlaySound 增加范围检查绕过开关和可选播放起点；声音更新包写入七个必填声音操作变体。
- 资源包设置使用 `UUID + 名称 + VarUInt 变体下标 + 对应值`，支持 float、bool、string、string 列表。
- 环境属性增加噪声对齐类型和 VarUInt 值；同时修正缓动名称、浮点约束 optional、颜色四个 Int32 分量。
- 诊断实体增加可选位置和维度；系统类别列表使用 optional。移除内存类别 `Persona_Textures` 并重排后续编号。
- 新增断开原因 148/149、容器类型 37、命令版本 51/52，命令 `Latest=52`、`Count=53`。地图装饰仍用原有数值字段，可承载新增编号 25–29。
- SetScore 移除记录使用单层 optional；删除旧的 `mUse26_44Format` 开关。

## API 迁移

| 2168 API | 2193 API |
| --- | --- |
| `mInputData.emplace().set(...)` | `mInputData.set(...)`，字段为 `std::bitset<66>`。 |
| `PackedItemUseLegacyInventoryTransaction::mHasActions` | 移除；`mItemUseTransaction.mTransaction.mActions` 始终序列化，可为空列表。 |
| 交互没有手部字段 | `ItemUseInventoryTransaction::mHand`，默认为 `HandSlot::MainHand`，支持 `OffHand`。 |
| 维度的 `mHeightMax` / `mHeightMin` | `mMinY` / `mHeightRange`，另设 `mDefaultBiome`。 |
| `BossEventPacket::mPlayerID` | 移除。 |
| 资源包设置的类型字节及三个独立值字段 | `mPackSettingValue` 为 `std::variant<float, bool, std::string, std::vector<std::string>>`。 |
| 声音更新的七个 optional | 七个普通 `SoundUpdate`；每个字段都写入变体下标及其负载。默认值为 Stop。 |
| `FloatAttributeData::mConstraintMin/Max` 是 float | 改为 `optional<float>`。 |
| `ColorAttributeData::mValue` 是 string | 改为 `array<int32_t, 4>`，按 R、G、B、A 顺序。 |
| 诊断系统类别为 vector | 改为 `optional<vector<SystemCategory>>`；实体诊断另有 `mPosition` / `mDimension` 两个 optional。 |
| `SetScorePacket::mUse26_44Format` | 移除，2193 固定使用单层 optional。 |

## 上游差异的处理

- **声音更新**：Endstone 的高级模型将七个展示字段合并成一个事件；本实现采用其 `protocol-docs` 转储和 gophertunnel 一致的七个必填变体，以 gophertunnel 实际字节验证。
- **诊断**：转储与 Endstone 模型将新增位置、维度及系统类别列表标为 optional。Cloudburst 的新增位置/维度缺少存在标记，gophertunnel 尚缺新增实体字段，并将系统类别写成普通数组。因此使用转储推导的固定字节验证，不将该参考库的诊断输出当作基准。
- **环境属性**：操作使用名称字符串，浮点约束为 optional，颜色为四个 Int32，缓动使用 `in_out_quad` 等名称。gophertunnel 的属性叶类型在此提交仍不同，因此这些类型和带噪声对齐的完整环境属性包使用独立固定字节测试。
- **交互手部**：转储和 gophertunnel 都在 hotbar slot 后写入 Hand 字节；同时应用于普通库存交易和 PlayerAuthInput 内的交易。

## 验证

`tests/reference/gophertunnel-2193.txt` 包含固定提交实际生成的 **62 份包体**。
测试检查数据包工厂、完整解码、逐字节重编码、截断拒绝，以及关键字段语义。
独立用例还覆盖诊断可选字段、环境属性、熔炉字节布局、非法高度图长度、非法变体、重复/越界输入标志和既有库存/结构名称回归。

```sh
cmake -S . -B build/protocol2193-cmake -DPROTOCOL_BUILD_TESTS=ON -DPROTOCOL_ENABLE_AUTHENTICATION=OFF -DPROTOCOL_ENABLE_CONNECTION=OFF
cmake --build build/protocol2193-cmake --config Release --parallel
ctest --test-dir build/protocol2193-cmake -C Release --output-on-failure
```

可加 `-DPROTOCOL_ENABLE_FORMATTING=ON -DPROTOCOL_ENABLE_DETAIL_ERRORS=ON` 检查可选功能。
参考字节已提交，运行 C++ 测试无需 Go 或网络。重新生成时，在上述固定 gophertunnel 检出目录执行：

```sh
go run /absolute/path/to/BedrockProtocol/tests/reference/generate_gophertunnel.go /absolute/path/to/BedrockProtocol/tests/reference/gophertunnel-2193.txt
```

格式化选项现在显式引入 Boost.PFR 2.2.0，CMake / xmake 均支持在关闭认证模块时独立启用它。

已通过本地 MSVC Release 编解码模块编译及协议字节测试，覆盖默认配置和同时开启格式化、详细错误的配置；两种配置均禁用 RTTI。
尚未进行真实 26.50 客户端登录、进世界及游戏过程联调。
服务端仍需提供目标版本匹配的物品/方块注册表、世界数据和认证上下文。
