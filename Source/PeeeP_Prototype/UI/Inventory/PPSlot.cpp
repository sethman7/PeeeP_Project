// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/PPSlot.h"
#include "Inventory/PPInventoryPartsItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Interface/UI/PPInventoryInterface.h"
#include "Inventory/PPInventoryComponent.h"

void UPPSlot::NativeConstruct()
{
	TXT_Quantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Quantity")));
}

void UPPSlot::Init()
{
	// 개별 함수를 연동하여 맵에 저장(현재는 파츠 슬롯만)
	SlotUpdateActions.Add(ESlotType::ST_None, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UPPSlot::UpdatePartsSlot)));
	SlotUpdateActions.Add(ESlotType::ST_InventoryParts, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UPPSlot::UpdatePartsSlot)));

	for (auto& item : SlotUpdateActions)
	{
		item.Value.SlotDelegate.ExecuteIfBound();
	}
}

void UPPSlot::SetType(ESlotType Type)
{
}

void UPPSlot::UpdateSlot()
{
	// 슬롯 타입에 따라 실행되는 함수 호출
	SlotUpdateActions[SlotType].SlotDelegate.ExecuteIfBound();
}

void UPPSlot::UpdatePartsSlot()
{
	IPPInventoryInterface* InvPlayer = Cast<IPPInventoryInterface>(OwningActor);

	if (InvPlayer)
	{
		// 인벤토리(Parts)를 가져온다.
		TArray<UPPInventoryPartsItem*> InventoryItems = InvPlayer->GetInventoryComponent()->GetPartsItems();	// ?

		// 현재 Slot의 인덱스가 유효한지 체크한다.
		if (InventoryItems.IsValidIndex(SlotIndex))
		{
			// 해당 슬롯에 아이템이 존재하는지 확인
			if (IsValid(InventoryItems[SlotIndex]))
			{
				// 존재하는 경우 아이템의 텍스쳐와 수량을 반영(파츠는 수량 표시 안함)
				IMG_Item->SetBrushFromTexture(InventoryItems[SlotIndex]->PartsData->ItemTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));
			}
			else
			{
				// 존재하지 않는 경우 빈 칸
				IMG_Item->SetBrushFromTexture(DefaultTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));
			}
		}
	}
} 
