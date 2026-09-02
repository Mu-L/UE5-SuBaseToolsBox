// Copyright 2026 SuBase. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Right-ClickOperationTool/ActorAction.h"

#include "DrawDebugHelpers.h"
#include "Editor.h"
#include "ScopedTransaction.h"
#include "Selection.h"
#include "Misc/MessageDialog.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"


void UActorAction::AlignSelectedActorsToGround(float GroundOffset)
{
    if (!GEditor) return;
    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return;
 
    TArray<AActor*> SelectedActors;
    GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);
 
    if (SelectedActors.Num() == 0) return;
 
   
 
    FScopedTransaction Transaction(FText::FromString("Align Actors to Ground"));
 
    for (AActor* Actor : SelectedActors)
    {
        if (!Actor) continue;
 
        // ... 获取几何中心逻辑 ...
        FBox SphereBounds = Actor->GetComponentsBoundingBox(true);
        FVector GeometryCenter = SphereBounds.GetCenter();
        
        FHitResult Hit;
        if (World->LineTraceSingleByChannel(Hit, GeometryCenter, GeometryCenter - (FVector::UpVector * 10000.f), ECC_WorldStatic))
        {
            Actor->Modify();
 
            // 1. 处理绕几何中心旋转补偿
            FVector TargetNormal = Hit.Normal;
            FQuat DeltaSurfRot = FQuat::FindBetweenVectors(FVector::UpVector, TargetNormal);
            FRotator CurrentRot = Actor->GetActorRotation();
            FQuat TargetRotation = DeltaSurfRot * FQuat(FRotator(0, CurrentRot.Yaw, 0));
 
            FVector PivotToCenter = GeometryCenter - Actor->GetActorLocation();
            FVector RotatedPivotToCenter = TargetRotation.RotateVector(Actor->GetActorQuat().UnrotateVector(PivotToCenter));
            
            Actor->SetActorRotation(TargetRotation);
            Actor->SetActorLocation(GeometryCenter - RotatedPivotToCenter);
 
            // 2. 重新计算旋转后的底部位置并加上用户自定义偏移
            Actor->UpdateComponentTransforms();
            FBox NewBounds = Actor->GetComponentsBoundingBox(true);
            
            float OffsetToBottom = Actor->GetActorLocation().Z - NewBounds.Min.Z;
            
            FVector FinalLocation = Actor->GetActorLocation();
            // 最终高度 = 击中点高度 + 枢轴到底部的垂直距离 + 用户偏移量
            FinalLocation.Z = Hit.Location.Z + OffsetToBottom + GroundOffset;
 
            Actor->SetActorLocation(FinalLocation);
            Actor->PostEditMove(true);
        }
    }
    GEditor->RedrawLevelEditingViewports();
}

void UActorAction::SetStaticMeshPivot()
{
    if (!GEditor) return;


    TSharedPtr<SSpinBox<float>> ZBox;   // 上下 -> Z 轴 (0=底部 1=顶部)
    TSharedPtr<SSpinBox<float>> YBox;   // 左右 -> Y 轴 (0=左   1=右)
    TSharedPtr<SSpinBox<float>> XBox;   // 前后 -> X 轴 (0=前   1=后)
    TSharedPtr<SCheckBox> WorldChk;     // 世界坐标系 / 本地坐标系
    TSharedPtr<SCheckBox> UnifiedChk;   // 统一位置 / 各自
    TSharedPtr<SCheckBox> PermChk;      // 永久修改 / 临时

    TSharedRef<SWindow> Window = SNew(SWindow)
        .Title(NSLOCTEXT("ActorAction", "PivotTitle", "设置静态网格体枢轴"))
        .ClientSize(FVector2D(460, 420))
        .SupportsMinimize(false)
        .SupportsMaximize(false)
        .IsTopmostWindow(true);

    auto BuildRow = [](const FText& Label, TSharedPtr<SSpinBox<float>>& OutBox) -> TSharedRef<SHorizontalBox>
    {
        return SNew(SHorizontalBox)
            + SHorizontalBox::Slot().FillWidth(0.45f).VAlign(VAlign_Center).Padding(8, 4)
            [
                SNew(STextBlock).Text(Label).AutoWrapText(true)
            ]
            + SHorizontalBox::Slot().FillWidth(0.55f).Padding(8, 4)
            [
                SAssignNew(OutBox, SSpinBox<float>).MinValue(0.f).MaxValue(1.f).Value(0.5f).Delta(0.01f).MinDesiredWidth(120.f)
            ];
    };

    Window->SetContent(
        SNew(SVerticalBox)
        + SVerticalBox::Slot().AutoHeight().Padding(12, 10, 12, 4)
        [ SNew(STextBlock)
            .AutoWrapText(true)
            .Text(NSLOCTEXT("ActorAction", "PivotHint",
                "输入 0-1 的数值，基于网格体实际占用的包围盒（立方体积）计算枢轴位置（默认 0.5 为几何中心）。\n0=最小边（底/左/前），1=最大边（顶/右/后）。")) ]
        + SVerticalBox::Slot().AutoHeight().Padding(4, 2)
        [ BuildRow(NSLOCTEXT("ActorAction", "AxisZ", "上下 (Z 轴)：0=底部  1=顶部"), ZBox) ]
        + SVerticalBox::Slot().AutoHeight().Padding(4, 2)
        [ BuildRow(NSLOCTEXT("ActorAction", "AxisY", "左右 (Y 轴)：0=左  1=右"), YBox) ]
        + SVerticalBox::Slot().AutoHeight().Padding(4, 2)
        [ BuildRow(NSLOCTEXT("ActorAction", "AxisX", "前后 (X 轴)：0=前  1=后"), XBox) ]
        + SVerticalBox::Slot().AutoHeight().Padding(10, 8, 10, 2)
        [ SAssignNew(WorldChk, SCheckBox).IsChecked(ECheckBoxState::Unchecked)
            [
                SNew(STextBlock)
                .Text(NSLOCTEXT("ActorAction", "ChkWorld", "使用世界坐标"))
                .ToolTipText(NSLOCTEXT("ActorAction", "ChkWorld", "按世界坐标系设置枢轴（默认：本地坐标系）"))
            ] ]
        + SVerticalBox::Slot().AutoHeight().Padding(10, 2)
        [ SAssignNew(UnifiedChk, SCheckBox).IsChecked(ECheckBoxState::Unchecked)
            [
                SNew(STextBlock)
                .Text(NSLOCTEXT("ActorAction", "ChkUnified", "统一位置"))
                .ToolTipText(NSLOCTEXT("ActorAction", "ChkUnified", "多个模型按总体位置设置相同枢轴（默认：各自设置）"))
            ] ]
        + SVerticalBox::Slot().AutoHeight().Padding(10, 2)
        [ SAssignNew(PermChk, SCheckBox).IsChecked(ECheckBoxState::Unchecked)
            [
                SNew(STextBlock)
                .Text(NSLOCTEXT("ActorAction", "ChkPerm", "永久修改"))
                .ToolTipText(NSLOCTEXT("ActorAction", "ChkPerm", "永久修改：保存到关卡（默认：临时，撤销/重载即还原）"))
            ] ]
        + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Right).Padding(10, 12)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().AutoWidth().Padding(4)
            [ SNew(SButton)
                .Text(NSLOCTEXT("ActorAction", "Apply", "应用"))
                .OnClicked_Lambda([=]() -> FReply
                {
                    const float Z = FMath::Clamp(ZBox->GetValue(), 0.f, 1.f);
                    const float Y = FMath::Clamp(YBox->GetValue(), 0.f, 1.f);
                    const float X = FMath::Clamp(XBox->GetValue(), 0.f, 1.f);
                    const bool bWorld    = WorldChk->IsChecked();
                    const bool bUnified  = UnifiedChk->IsChecked();
                    const bool bPerm     = PermChk->IsChecked();

                    // 重新获取当前选中的静态网格体 Actor
                    TArray<AActor*> Selected;
                    GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(Selected);

                    struct FTarget { AActor* Actor; UStaticMeshComponent* Comp; };
                    TArray<FTarget> Targets;
                    for (AActor* A : Selected)
                    {
                        if (!A) continue;
                        TArray<UStaticMeshComponent*> Comps;
                        A->GetComponents<UStaticMeshComponent>(Comps);
                        for (UStaticMeshComponent* C : Comps)
                        {
                            if (C && C->GetStaticMesh())
                            {
                                Targets.Add({ A, C });
                                break;
                            }
                        }
                    }

                    if (Targets.Num() == 0)
                    {
                        FMessageDialog::Open(EAppMsgType::Ok,
                            NSLOCTEXT("ActorAction", "NoTarget", "未选中任何带静态网格体的 Actor。请先选中场景中的 StaticMesh Actor。"));
                        return FReply::Handled();
                    }

                    // 统一模式：先算出所有选中网格体实际占用的世界包围盒合并体
                    FBox CombinedWorldBox(ForceInit);
                    bool bHasBox = false;
                    if (bUnified)
                    {
                        for (const FTarget& T : Targets)
                        {
                            FBox B = T.Comp->Bounds.GetBox();
                            if (B.IsValid)
                            {
                                CombinedWorldBox += B;
                                bHasBox = true;
                            }
                        }
                    }

                    auto LerpBox = [](const FBox& Box, float Xv, float Yv, float Zv)
                    {
                        return FVector(
                            FMath::Lerp(Box.Min.X, Box.Max.X, Xv),
                            FMath::Lerp(Box.Min.Y, Box.Max.Y, Yv),
                            FMath::Lerp(Box.Min.Z, Box.Max.Z, Zv));
                    };

                    FScopedTransaction Transaction(NSLOCTEXT("ActorAction", "Txn", "设置静态网格体枢轴"));
                    for (const FTarget& T : Targets)
                    {
                        // 1) 先算出该静态网格体实际占用的世界包围盒（立方体积大小与位置）
                        FBox MeshWorldBox = T.Comp->Bounds.GetBox();
                        if (!MeshWorldBox.IsValid)
                        {
                            // 兜底：用网格本地包围盒经组件变换得到世界盒
                            MeshWorldBox = T.Comp->GetStaticMesh()->GetBoundingBox()
                                .TransformBy(T.Comp->GetComponentTransform());
                        }

                        // 2) 在包围盒上按 0-1 参数插值得到目标枢轴世界点
                        //    Z: 0=底部 1=顶部 ; Y: 0=左 1=右 ; X: 0=前 1=后
                        FVector WorldPivot = LerpBox(MeshWorldBox, X, Y, Z);

                        FVector PivotLocal;
                        if (bUnified && bHasBox)
                        {
                            // 统一位置：所有目标共用同一个世界枢轴点
                            WorldPivot = LerpBox(CombinedWorldBox, X, Y, Z);
                            PivotLocal = T.Actor->GetActorTransform().InverseTransformPosition(WorldPivot);
                        }
                        else if (bWorld)
                        {
                            // 世界坐标系：枢轴落在世界轴向上的参数位置
                            PivotLocal = T.Actor->GetActorTransform().InverseTransformPosition(WorldPivot);
                        }
                        else
                        {
                            // 本地坐标系：枢轴落在网格自身（组件）本地坐标框架的参数位置
                            PivotLocal = T.Comp->GetComponentTransform().InverseTransformPosition(WorldPivot);
                        }

                        T.Actor->Modify();
                        T.Actor->SetPivotOffset(PivotLocal);
                        if (bPerm)
                        {
                            T.Actor->MarkPackageDirty();
                        }
                    }

                    GEditor->RedrawLevelEditingViewports();
                    Window->RequestDestroyWindow();
                    return FReply::Handled();
                }) ]
            + SHorizontalBox::Slot().AutoWidth().Padding(4)
            [ SNew(SButton)
                .Text(NSLOCTEXT("ActorAction", "Cancel", "取消"))
                .OnClicked_Lambda([Window]() -> FReply
                {
                    Window->RequestDestroyWindow();
                    return FReply::Handled();
                }) ]
        ]
    );

    FSlateApplication::Get().AddWindow(Window);
}
