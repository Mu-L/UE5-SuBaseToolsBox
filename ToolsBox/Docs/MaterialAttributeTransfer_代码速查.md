# MaterialAttributeTransfer.cpp 涉及的引擎类速查

> 阅读方式：每条都写成「**从哪拿 → 拿到什么 → 调什么**」的链路。

---

## 一、编辑器模块访问（最核心的套路）

UE 的编辑器功能大多封装在**模块**里，统一用 `FModuleManager` 按模块名拿到模块对象，
再从模块里拿**接口单例**，最后调接口上的函数。三步走。

```cpp
FModuleManager::LoadModuleChecked<模块类>("模块名")  →  模块.Get()  →  接口单例  →  调函数
```

| 目的 | 链路 | 头文件 |
|---|---|---|
| 拿内容浏览器里**选中的资产** | `FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser")` → `.Get()` 得 `IContentBrowserSingleton&` → `GetSelectedAssets(TArray<FAssetData>&)` | `ContentBrowserModule.h` / `IContentBrowserSingleton.h` |
| 拿内容浏览器里**选中的文件夹路径** | 同上 → `GetSelectedPathViewFolders(TArray<FString>&)` | 同上 |
| **创建资产** | `FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools")` → `.Get()` 得 `IAssetTools&` → `CreateAsset(名字, 包路径, 类, Factory)` | `AssetToolsModule.h` / `IAssetTools.h` |
| **通知资产注册表"我新建了个资产"** | `FAssetRegistryModule::AssetCreated(UObject*)`（**静态函数**，不用先拿模块） | `AssetRegistry/AssetRegistryModule.h` |

> 记忆点：`LoadModuleChecked` 找不到模块会直接崩（Checked = 断言）；
> 对应的还有 `GetModulePtr<T>()`，找不到返回 nullptr。
> 用哪个模块，就要在 `Build.cs` 的 `PrivateDependencyModuleNames` 里加对应模块名。

---

## 二、资产数据 / 创建 / 保存

| 类 / API | 作用 | 说明 |
|---|---|---|
| `FAssetData` | **资产的"名片"** | 只含路径、类名等元数据，**不加载对象**。要真正拿到 UObject 得调 `AssetData.GetAsset()`（会触发加载） |
| `UMaterialInstanceConstantFactoryNew` | 材质实例的**工厂类** | `IAssetTools::CreateAsset` 需要一个 `UFactory*` 来告诉它"怎么造"。每种资产类型都有对应 `XxxFactoryNew` |
| `CreatePackage(*包路径)` → `UPackage*` | 手动建一个"包"（= 磁盘上的 .uasset 文件容器） | 走 `DuplicateObject` 这种手动路线时必须先建包 |
| `DuplicateObject<T>(源, Outer包, 新名字)` | 深拷贝一个 UObject 到指定包 | 本代码用它来"复制母材质材质图"生成新材质类 |
| `NewMat->SetFlags(RF_Public \| RF_Standalone)` | 标记为**可被外部引用 + 独立存在** | 不加这两个 Flag，资产存盘后会被 GC 当垃圾清掉 |
| `MarkPackageDirty()` | 标脏，告诉编辑器"这个包要保存" | |
| `FindObject<UPackage>(nullptr, *全路径)` | 查**内存中**是否已有同名包 | 判重名第一步 |
| `FPackageName::DoesPackageExist(路径)` | 查**磁盘上**是否已有该包 | 判重名第二步，两者要一起用 |
| `StaticLoadObject(类, nullptr, *路径)` | 按路径字符串**同步加载**一个资产 | 加载配置时用它把保存的母材质路径还原成对象 |
| `UEditorLoadingAndSavingUtils::SaveDirtyPackages(bSaveMaps, bSaveContent)` | 一键保存所有脏包 | 头文件 `FileHelpers.h`，名字和类名对不上，容易找不到 |

> 路径拼接用 `FString / FString`（重载了 `operator/`），会自动补斜杠：`FinalPath / NewAssetName`。

---

## 三、材质体系

### 继承关系（先记这个，后面全通）

```
UMaterialInterface          ← 抽象基类，"任何能当材质用的东西"
 ├── UMaterial              ← 材质类（有材质图节点，没有"父材质"概念）
 └── UMaterialInstance      ← 材质实例（只存参数覆盖值，有 Parent）
      ├── UMaterialInstanceConstant   ← MIC，编辑器里能存成资产的那种
      └── UMaterialInstanceDynamic    ← MID，运行时动态创建，不落盘
```

### 读参数：在**基类** `UMaterialInterface` 上，实例和材质类通用

```cpp
UTexture* Tex = nullptr;
Source->GetTextureParameterValue(FName("BaseColor"), Tex);
Source->GetScalarParameterValue(FName("Rough"), FloatVal);
Source->GetVectorParameterValue(FName("Tint"), LinearColorVal);
```

### 写参数：**没有**统一接口，必须先 Cast 再分别调

| 目标类型 | 写法 |
|---|---|
| `UMaterialInstanceConstant` | `MIC->SetTextureParameterValueEditorOnly(Name, Tex)` / `SetScalar...` / `SetVector...` |
| `UMaterial` | `Mat->SetTextureParameterValueEditorOnly(Name, Tex)` / 同上 |

> 后缀 `EditorOnly` = 只在编辑器构建里存在，改的是**资产上保存的默认值**（区别于运行时 MID 的 `SetScalarParameterValue`）。
> 这也是本代码里 `ApplyParameterValues()` 要写一堆 `Cast` 分支的原因。

### 其它材质 API

- `MIC->SetParentEditorOnly(UMaterialInterface*)` —— 给材质实例指定父材质。
- `UMaterialInstance::Parent` —— 公有属性（`TObjectPtr<UMaterialInterface>`），可以**向上追溯**父链，本代码 `FindBaseMaterialTemplate()` 就是循环 Cast + 取 Parent，一直找到最底层的 `UMaterial`。
- `PostEditChange()` —— 属性改完后通知引擎重新编译/刷新，**不调的话改动可能不生效**。

---

## 四、Slate UI

### 声明式语法的骨架

| 元素 | 含义 |
|---|---|
| `SCompoundWidget` | 自定义控件的基类（"我是一个由别的控件组合成的控件"） |
| `SLATE_BEGIN_ARGS(类名) {} SLATE_END_ARGS()` | 宏，生成 `FArguments` 结构体，用来接构造参数 |
| `void Construct(const FArguments& InArgs)` | 相当于 Slate 的构造函数，UI 树在这里搭 |
| `ChildSlot[ ... ]` | 把搭好的 UI 塞进本控件的插槽 |
| `SNew(SButton)` | 新建控件，返回 `TSharedRef` |
| `SAssignNew(成员变量, SScrollBox)` | 新建的同时把指针存进成员变量（后面要操作它时用） |

### 布局

- `SVerticalBox` / `SHorizontalBox` —— 纵/横排容器，用 `+ SVerticalBox::Slot()` 加槽位。
- 槽位修饰：`AutoHeight()`（按内容高）/ `FillHeight(0.6f)`（按比例分剩余空间）/ `AutoWidth()` / `FillWidth(1.f)` / `Padding()` / `HAlign()` / `VAlign()`。
- `SScrollBox` —— 可滚动容器，`->ScrollToEnd()` 滚到底（日志窗口就靠这个）。
- `SBorder` —— 带背景的框，`.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))` 用引擎内置样式。

### 常用控件

| 控件 | 说明 |
|---|---|
| `STextBlock` | 静态文本，`.AutoWrapText(true)` 自动换行，`.MinDesiredWidth(100)` 对齐用 |
| `SEditableTextBox` | 单行输入框 |
| `SMultiLineEditableText` | 多行文本，`.IsReadOnly(true)` 当日志显示区 |
| `SButton` | 按钮，`.ButtonStyle(FAppStyle::Get(), "PrimaryButton")` 换成蓝色主按钮 |
| `SCheckBox` | 勾选框，状态类型是 `ECheckBoxState`（`Checked` / `Unchecked` / `Undetermined`，**是三态不是 bool**） |
| `SObjectPropertyEntryBox` | **资产选择器**（就是细节面板里那种带缩略图的下拉框）。`.AllowedClass(UMaterialInterface::StaticClass())` 限定类型，`.OnObjectChanged()` 选中回调，`.ObjectPath()` 反向显示当前值。头文件 `PropertyCustomizationHelpers.h`，需要 `PropertyEditor` 模块 |

### 属性绑定的三种写法（重点，Slate 里到处都是）

Slate 属性类型是 `TAttribute<T>`，支持三种赋值：

```cpp
.Text(FText::FromString(Str))                              // ① 静态值：只在构建时取一次，之后不会变
.Text_Lambda([this](){ return FText::FromString(Str); })   // ② Lambda 绑定：每帧调用，值会实时更新
.Text(this, &SMyWidget::GetText)                           // ③ 成员函数绑定，效果同 ②
```

同理还有 `.IsEnabled_Lambda()`、`.IsChecked_Lambda()`、`.OnClicked_Lambda()`。
**踩坑点**：想让 UI 跟着变量变，必须用 ②/③；用 ① 就写死了。

另外注意 `.ToolTip()` 收的是 `TSharedPtr<IToolTip>`，想直接给文字要用 **`.ToolTipText(FText)`**。

### 回调返回值

- 按钮回调必须返回 `FReply`：`FReply::Handled()`（事件我处理了）/ `FReply::Unhandled()`（继续往上传）。

### 文本本地化

- `#define LOCTEXT_NAMESPACE "MaterialTransferTool"` … `#undef LOCTEXT_NAMESPACE` —— 命名空间，配对出现。
- `LOCTEXT("Key", "显示文字")` —— 生成可本地化的 `FText`。
- `FText::FromString(FString)` / `FText::ToString()` —— 两者互转。

### FString / FName / FText 的区别（必须分清）

| 类型 | 用途 | 特点 |
|---|---|---|
| `FString` | 通用字符串操作 | 可变、能拼接查找，开销最大 |
| `FName` | **标识符**（参数名、资产名、Tag） | 存在全局表里，比较是整数比较，极快，**大小写不敏感** |
| `FText` | **给用户看的文本** | 支持本地化，不适合当 key |

---

## 五、文件 / 路径 / 平台

| API | 作用 |
|---|---|
| `FPlatformFileManager::Get().GetPlatformFile()` → `IPlatformFile&` | 跨平台文件系统接口，再调 `DirectoryExists()` / `CreateDirectoryTree()` |
| `FPaths::ProjectPluginsDir()` | 拿到 `工程/Plugins/` 目录 |
| `FPaths::GetPath(路径)` | 去掉最后一段，拿父目录（本代码用它取源材质所在文件夹） |
| `FPaths::ConvertRelativePathToFull()` | 相对路径 → 绝对路径 |
| `FFileHelper::SaveStringToFile()` / `LoadFileToString()` | 一行读写整个文本文件 |
| `FPlatformProcess::ExploreFolder(*路径)` | 在系统资源管理器里打开文件夹。**Windows 上必须传反斜杠路径**，正斜杠会静默失败（所以代码里做了 `.Replace(TEXT("/"), TEXT("\\"))`） |
| `FDateTime::Now().ToString(TEXT("[%H:%M:%S] "))` | 取当前时间并格式化，日志时间戳用 |

---

## 六、JSON 序列化

保存配置那套的完整链路：

```cpp
// 写
TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject());
Root->SetStringField(TEXT("Key"), Value);      // 还有 SetBoolField / SetNumberField / SetArrayField
TArray<TSharedPtr<FJsonValue>> Arr;
Arr.Add(MakeShareable(new FJsonValueObject(子对象)));   // FJsonValue 的子类：FJsonValueString/Number/Boolean/Object/Array

FString Out;
TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);  // 工厂产 Writer
FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);                 // 对象 → 字符串

// 读
TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
FJsonSerializer::Deserialize(Reader, Root);     // 字符串 → 对象
Root->HasField() / GetStringField() / GetBoolField() / TryGetArrayField()
```

> 读的时候优先用 `HasField()` 判断或 `TryGetXxxField()`，直接 `GetXxxField()` 取不存在的字段会报错。

---

## 七、智能指针 / 类型转换

| 写法 | 说明 |
|---|---|
| `TSharedPtr<T>` / `TSharedRef<T>` | Slate 和非 UObject 类型用的引用计数指针。`Ptr` 可为空（`.IsValid()` 判断），`Ref` 不可为空。`Ptr.ToSharedRef()` 互转 |
| `MakeShared<T>()` / `MakeShareable(new T())` | 创建共享指针，`MakeShared` 更高效（一次分配），优先用它 |
| `TWeakObjectPtr<UObject子类>` | **弱引用 UObject**，对象被 GC 后自动失效。用前必须 `.IsValid()`，取原始指针用 `.Get()`。本代码 `TargetMasterMaterial` 就是它 |
| `Cast<T>(Obj)` | UObject 的安全向下转型，失败返回 nullptr（相当于 dynamic_cast） |
| `T::StaticClass()` | 拿到 `UClass*`，反射系统的类型标识 |
| `NewObject<T>()` | 创建 UObject（本代码用它 new 出 Factory） |

---

## 八、一句话串联整个执行流程

1. `FContentBrowserModule` → `GetSelectedAssets` 拿 `TArray<FAssetData>`；
2. `AssetData.GetAsset()` + `Cast<UMaterialInterface>` 过滤出材质；
3. 算目标路径（`FPaths::GetPath` 或用户输入），`MakeUniqueAssetName` 防重名；
4. **走实例分支**：`IAssetTools::CreateAsset` + `UMaterialInstanceConstantFactoryNew` → `SetParentEditorOnly`；
   **走材质类分支**：`FindBaseMaterialTemplate` 追溯基础材质 → `CreatePackage` + `DuplicateObject`；
5. `ApplyParameterValues`：源用 `Get*ParameterValue` 读，目标 Cast 后用 `Set*ParameterValueEditorOnly` 写；
6. `PostEditChange()` + `FAssetRegistryModule::AssetCreated()` 通知引擎；
7. `UEditorLoadingAndSavingUtils::SaveDirtyPackages()` 落盘。
