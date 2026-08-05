[🇨🇳 中文](README.md) · [🇺🇸 English](README.en.md) · [🇯🇵 日本語](README.ja.md)

---

# ToolsBox

> An editor toolbox plugin for **Unreal Engine 5.8**, providing a set of practical art / asset workflow tools: batch import with material linking, batch material creation, material attribute transfer, blueprint variable copy, auto prefix, physics placement, and right-click menu operation scripts.

- **Engine**: Unreal Engine 5.8
- **Author**: SuBase
- **Bilibili**: https://space.bilibili.com/391627131/

---

## ✨ Features

The toolbox adds a **"ToolsBox"** button to the level editor's toolbar. Click it to open the toolbox panel, which contains the following tools:

| Tool | Description |
| --- | --- |
| **Batch Import Models and Materials** | Batch-import models and materials; models in the same file and their accompanying textures are automatically connected to a material and assigned to the model. |
| **Right-Click Menu Operation Scripts** | A set of asset / Actor operations added to the Content Browser / viewport right-click menu (e.g. batch-apply animation modifiers). |
| **Blueprint Variable Batch Copy** | Select variables (functions, macros, event dispatchers, etc.) from two blueprints, then copy / cut and paste them all at once into another blueprint. |
| **Batch Material Creation** | Select textures to batch-generate material or material instances and assign them to models. |
| **Material Attribute Transfer** | Select materials / material instances and transfer parameter values and references to new assets (parented to the master material, or standalone) via variable naming. |
| **Auto Prefix** | Automatically add the appropriate naming prefix when creating common new blueprint classes. |
| **Physics Placement** | Select multiple scene objects, start physics-based free-fall placement, and support saving / undoing placements. |

> Note: Tool titles and descriptions are localized (Chinese / English / Japanese) and can be switched in the plugin settings.

---

## 📦 Installation

### Option 1: As a project plugin
Copy the `ToolsBox` plugin folder from this repository into your project:
```
YourProject/Plugins/ToolsBox/      # must contain ToolsBox.uplugin
```
Then regenerate project files (right-click `.uproject` → Generate Visual Studio project files) and build.

### Option 2: As an engine plugin
Place it in the engine directory:
```
UE_5.8/Engine/Plugins/Marketplace/ToolsBox/
```

### Enable the plugin
After opening your project, search for `ToolsBox` under **Edit → Plugins**, check it to enable, then restart the editor.

---

## 🚀 Usage

1. Enable the plugin and restart the editor.
2. A **"ToolsBox"** button appears on the **Level Editor top toolbar** (near the Play button).
3. Click it to open the toolbox panel; the tools above are listed there. Click a tool block to expand and use it.

---

## 🌐 Localization

The plugin ships with three localizations: Chinese (zh-Hans, native), English, and Japanese. To switch:

- Edit the `"Language"` field in `ToolsBox/ToolsBox.uplugin` to `zh-Hans` / `en` / `ja`, save, and restart the editor; or
- Use the language dropdown inside the toolbox to switch at runtime (requires running **Compile Text** for en / ja in the UE localization panel to generate the `.locres` first).

---

## 📁 Directory Structure

```
ToolsBox/
├─ ToolsBox.uplugin
├─ Source/ToolsBox/
│  ├─ Private/
│  │  ├─ ToolsBox.cpp                                # module entry, toolbar registration, language switch
│  │  ├─ Tools.cpp                                   # tool list Get_ToolsData()
│  │  ├─ Tools/                                       # per-tool implementations
│  │  │  ├─ IntelligentImportOfModelsAndMaterials/    # batch import models and materials
│  │  │  ├─ Right-ClickOperationTool/                 # right-click menu operation scripts
│  │  │  ├─ VariableCopier/                           # blueprint variable batch copy
│  │  │  ├─ SpawnMaterial/                            # batch material creation
│  │  │  ├─ MaterialAttributeTransfer/                # material attribute transfer
│  │  │  ├─ AutoPrefix/                               # auto prefix
│  │  │  ├─ PhysicsPlacer/                            # physics placement
│  │  │  └─ BlankTemplateTool/                        # blank template (reserved)
│  │  └─ Slate_Assist/                                # Slate helpers (icons, tool-block builder, localization)
│  └─ Public/Tools/...                                # corresponding headers
├─ Content/AssetActionUtility/                        # assets used by right-click tools
├─ Content/Localization/ToolsBox/{en,ja}              # translation PO files
├─ Resources/                                         # icon / image resources
└─ (generated at runtime) ToolUserDataSave/           # user-saved JSON configs
```

---

## 📝 Notes

- User-saved JSON configs are generated at runtime under the plugin's `ToolUserDataSave/` directory (ignored by `.gitignore`).
- The plugin is still in Beta (`IsBetaVersion = true` in the uplugin); APIs and features may change between versions.

---

## 📄 License

Fully open-source and free. You may modify and extend the plugin, and new tools are added from time to time. If you have requests or suggestions for tools, feel free to leave them in the comments.
