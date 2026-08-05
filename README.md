[🇨🇳 中文](README.md) · [🇺🇸 English](README.en.md) · [🇯🇵 日本語](README.ja.md)

---

# ToolsBox

> 一个面向 **Unreal Engine 5.8** 的编辑器工具箱插件，为美术与资产工作流提供一组实用工具：批量导入与材质连接、批量生成材质、材质属性迁移、蓝图变量复印、自动前缀、物理摆放，以及右键菜单操作脚本等。

- **引擎版本**：Unreal Engine 5.8
- **作者**：SuBase
- **B 站**：https://space.bilibili.com/391627131/

---

## ✨ 功能一览

工具箱在编辑器关卡工具条上提供一个 **「ToolsBox」** 按钮，点击打开工具箱面板，里面包含以下工具：

| 工具 | 说明 |
| --- | --- |
| **批量导入模型和材质** | 批量导入模型与材质；同文件下的模型及其附属纹理贴图会自动连接材质球并赋予模型。 |
| **右键菜单操作脚本** | 一组扩展「内容浏览器 / 视口」右键菜单的资产 / Actor 操作脚本（如批量添加动画修饰符等）。 |
| **蓝图变量批量复制** | 多选两个蓝图的变量（函数、宏、事件分发器等），复制 / 剪切后一次性粘贴到另一个蓝图。 |
| **批量材质球生成** | 选中若干纹理，批量生成材质球或材质实例并赋予模型。 |
| **材质属性转移** | 选中若干材质球 / 材质实例，按变量命名映射，将参数值与引用转移到以母材质为父类（或独立材质类）的新资产中。 |
| **自动前缀** | 新建常见蓝图类时，自动为其添加该类对应的命名前缀。 |
| **物理摆放** | 选中多个场景物体，启动物理自由掉落摆放，并支持保存 / 回溯摆位。 |

> 提示：以上工具标题与描述均支持本地化（中文 / English / 日本語），可在插件设置中切换。

---

## 📦 安装

### 方式一：作为项目插件
将本仓库中的 `ToolsBox` 插件文件夹放入你的项目：
```
你的项目/Plugins/ToolsBox/      # 需包含 ToolsBox.uplugin
```
然后重新生成项目文件（右键 `.uproject` → Generate Visual Studio project files）并编译。

### 方式二：作为引擎插件
放入引擎目录：
```
UE_5.8/Engine/Plugins/Marketplace/ToolsBox/
```

### 启用插件
打开项目后，在 **编辑 → 插件** 中搜索 `ToolsBox` 并勾选启用，重启编辑器。

---

## 🚀 使用方式

1. 启用插件并重启编辑器。
2. 在 **关卡编辑器（Level Editor）顶部工具条**（靠近 Play 的位置）会出现一个 **「ToolsBox」** 按钮。
3. 点击按钮打开工具箱面板，面板中列出上述工具，点击对应工具块即可展开使用。

---

## 🌐 多语言

插件内置中文（zh-Hans，原生）、English、日本語 三套本地化。切换方式：

- 编辑 `ToolsBox/ToolsBox.uplugin` 中的 `"Language"` 字段为 `zh-Hans` / `en` / `ja`，保存后重启编辑器；
- 或在编辑器内通过工具箱提供的语言下拉菜单实时切换（需先在 UE 本地化面板对 en / ja 执行 **Compile Text** 生成 `.locres`）。

---

## 📁 目录结构

```
ToolsBox/
├─ ToolsBox.uplugin
├─ Source/ToolsBox/
│  ├─ Private/
│  │  ├─ ToolsBox.cpp                                # 模块入口、工具条注册、语言切换
│  │  ├─ Tools.cpp                                   # 工具清单 Get_ToolsData()
│  │  ├─ Tools/                                       # 各工具实现
│  │  │  ├─ IntelligentImportOfModelsAndMaterials/    # 批量导入模型和材质
│  │  │  ├─ Right-ClickOperationTool/                 # 右键菜单操作脚本
│  │  │  ├─ VariableCopier/                           # 蓝图变量批量复制
│  │  │  ├─ SpawnMaterial/                            # 批量材质球生成
│  │  │  ├─ MaterialAttributeTransfer/                # 材质属性转移
│  │  │  ├─ AutoPrefix/                               # 自动前缀
│  │  │  ├─ PhysicsPlacer/                            # 物理摆放
│  │  │  └─ BlankTemplateTool/                        # 空白模版（预留）
│  │  └─ Slate_Assist/                                # Slate 辅助（图标、工具块构造、本地化）
│  └─ Public/Tools/...                                # 对应头文件
├─ Content/AssetActionUtility/                        # 右键工具所用资产
├─ Content/Localization/ToolsBox/{en,ja}              # 翻译 PO 文件
├─ Resources/                                         # 图标 / 图片资源
└─ (运行期生成) ToolUserDataSave/                      # 用户保存的 JSON 配置
```

---

## 📝 备注

- 用户配置（保存的 JSON）在运行期生成于插件 `ToolUserDataSave/` 目录（已在 `.gitignore` 中忽略，不会进入版本库）。
- 本插件仍处于 Beta 阶段（uplugin 中 `IsBetaVersion = true`），接口与功能可能随版本调整。

---

## 📄 License

完全开源且免费，大家可以修改和扩展插件，并不定时更新新工具。如果你有插件工具的需求或建议，欢迎在评论区提出。
