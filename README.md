# ToolsBox

> 一个面向 **Unreal Engine 5.8** 的编辑器工具箱插件，提供了批量导入、批量生成材质、材质属性迁移、右键操作脚本等一组实用的美术 / 资产工作流工具。

- **引擎版本**：Unreal Engine 5.8
- **作者**：SuBase

---

## ✨ 功能一览

工具箱在编辑器关卡工具条上提供一个 **「ToolsBox」** 按钮，点击打开工具箱面板，里面包含以下工具：

| 工具 | 说明 |
| --- | --- |
| **批量导入模型和材质** | 批量导入模型与材质，同文件下的模型及其附属纹理贴图会自动连接材质球并赋予模型。 |
| **批量材质球生成** | 选中若干纹理，批量生成材质球或材质实例并赋予模型。 |
| **材质属性转移** | 选中若干材质球 / 材质实例，按变量命名映射，把参数值转移到以母材质为父类（或独立材质类）的新资产中。 |
| **右键菜单操作脚本** | 一组扩展「内容浏览器 / 视口」右键菜单的资产 / Actor 操作脚本（。 |

> 另有「空白模版」模块，可在 `Tools.cpp` 的 `Get_ToolsData()` 中新建工具。

---

## 📦 安装

### 方式一：作为项目插件
将本仓库（即 `ToolsBox` 插件目录）放入你的项目：
```
你的项目/Plugins/ToolsBox/
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
2. 在 **关卡编辑器（Level Editor）顶部工具条**（靠近 Play 工具条的位置）会出现一个 **「ToolsBox」** 按钮。
3. 点击按钮打开工具箱面板，面板中列出上述工具，点击对应工具块即可展开使用。
4. 每个工具块右上角有一个链接图标，可打开该工具的说明页面（当前默认指向作者 B 站主页占位，可在 `Tools.cpp` 的 `FTool` 中替换为实际地址）。

---

## 📁 目录结构

```
ToolsBox/
├─ ToolsBox.uplugin
├─ Source/ToolsBox/
│  ├─ Private/
│  │  ├─ ToolsBox.cpp                      # 模块入口、工具条注册
│  │  ├─ Tools.cpp                         # 工具清单 Get_ToolsData()
│  │  ├─ Tools/                            # 各工具实现
│  │  │  ├─ IntelligentImportOfModelsAndMaterials/   # 批量导入
│  │  │  ├─ MaterialAttributeTransfer/              # 材质属性转移
│  │  │  ├─ SpawnMaterial/                        # 批量材质球生成
│  │  │  ├─ Right-ClickOperationTool/             # 右键操作脚本
│  │  │  └─ ...
│  │  └─ Slate_Assist/                     # Slate 辅助（图标、工具块构造）
│  └─ Public/Tools/...                     # 对应头文件
├─ Content/AssetActionUtility/             # 右键工具所用资产
├─ Resources/                              # 图标 / 图片资源
└─ (用户配置) Public/Tools/ToolUserDataSave/   # 运行期生成的 JSON 配置
```

---

## 📝 备注

- 用户配置（保存的 JSON）默认位于插件 `Public/Tools/ToolUserDataSave/` 目录。

- 本插件仍处于 Beta 阶段，接口与功能可能随版本调整。

  

---

## 📄 License

完全开源且免费，大家可以修改和扩展插件，并且不定时更新新工具，如果有什么插件工具的需求，可以发布到评论区。
