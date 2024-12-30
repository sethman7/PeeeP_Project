// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/PPQuickSlotWidget.h"
#include "Inventory/PPInventoryComponent.h"
#include "Blueprint/WidgetTree.h"

void UPPQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TXT_Equipment = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Equipment")));
	TXT_Equipment->SetVisibility(ESlateVisibility::Hidden);
}

void UPPQuickSlotWidget::Init()
{
	// 기본 타입 설정(파츠)
	SetType(ESlotType::ST_InventoryParts);

	// 슬롯을 저장하기 위한 배열 초기화(일단 6칸)
	Slots.Init(nullptr, 6);

	TArray<UWidget*> Widgets;
	// 모든 위젯들을 가져와 Widgets 배열에 저장
	WidgetTree->GetAllWidgets(Widgets);

	// 배열을 순회하여 커스텀 Slot을 찾음
	// 궁금해진 점: 이렇게 찾아오면 UMG에서 순서를 지정한대로 저장할 수 있을까?
	// 일단은 순서대로 저장됨.
	for (UWidget* Widget : Widgets)
	{
		UPPSlot* InvSlot = Cast<UPPSlot>(Widget);
		if (InvSlot)
		{
			// 슬롯의 소유주를 현재 소유주로 지정
			InvSlot->SetOwningActor(OwningActor);
			// 슬롯의 타입을 현재 인벤토리의 타입으로 지정
			InvSlot->SetType(InventorySlotType);
			// 슬롯 초기화
			InvSlot->Init();
			// 해당 슬롯의 인덱스 위치에 슬롯을 저장
			Slots[InvSlot->SlotIndex] = InvSlot;
		}
	}
	UPPInventoryComponent* InventoryComponent = Cast<UPPInventoryComponent>(OwningActor->GetComponentByClass(UPPInventoryComponent::StaticClass()));
	if (InventoryComponent != nullptr)
	{
		InventoryComponent->SetQuickSlotWidget(this);
	}

	for (auto& InvSlot : Slots)
	{
		InvSlot->SetVisibility(ESlateVisibility::Hidden);
	}
	Slots[0]->SetVisibility(ESlateVisibility::Visible);
}

void UPPQuickSlotWidget::UpdateQuickSlot()
{
	// 슬롯을 현재 인벤토리 타입으로 업데이트
	for (const auto& InvSlot : Slots)
	{
		InvSlot->SetType(InventorySlotType);
		InvSlot->UpdateSlot();
	}
}

void UPPQuickSlotWidget::SetQuickSlotWidget(UPPQuickSlotWidget* source)
{
	source = this;
}

void UPPQuickSlotWidget::SetEquipmentTextVisible(ESlateVisibility VisibleMode)
{
	TXT_Equipment->SetVisibility(VisibleMode);
}

TArray<TObjectPtr<class UPPSlot>> UPPQuickSlotWidget::GetSlots()
{
	return Slots;
}

void UPPQuickSlotWidget::SetType(ESlotType Type)
{
	// 현재 인벤토리 타입 변경
	InventorySlotType = Type;
	UpdateQuickSlot();
}


