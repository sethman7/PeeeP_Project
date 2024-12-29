// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/PPInventoryWidget.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "UI/Inventory/PPSlot.h"

void UPPInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPPInventoryWidget::Init()
{
	// 기본 타입 설정(파츠)
	SetType(ESlotType::ST_InventoryParts);

	// 버튼 클릭 이벤트와 함수 연동
	if (BTN_Parts)
	{
		BTN_Parts->OnClicked.AddDynamic(this, &UPPInventoryWidget::SetPartsType);
	}

	// 슬롯을 저장하기 위한 배열 초기화(일단 5칸)
	Slots.Init(nullptr, 5);

	TArray<UWidget*> Widgets;
	// 모든 위젯들을 가져와 Widgets 배열에 저장
	WidgetTree->GetAllWidgets(Widgets);

	// 배열을 순회하여 커스텀 Slot을 찾음
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
}

void UPPInventoryWidget::UpdateInventorySlot()
{
	// 슬롯을 현재 인벤토리 타입으로 업데이트
	for (const auto& InvSlot : Slots)
	{
		InvSlot->SetType(InventorySlotType);
		InvSlot->UpdateSlot();
	}
}

void UPPInventoryWidget::SetPartsType()
{
	SetType(ESlotType::ST_InventoryParts);
}

void UPPInventoryWidget::SetType(ESlotType Type)
{
	// 현재 인벤토리 타입 변경
	InventorySlotType = Type;
	UpdateInventorySlot();
}
