// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCustomWidget.h"
#include "PPSlot.h"
#include "../../Inventory/ESlotType.h"
#include "PPQuickSlotWidget.generated.h"


UCLASS()
class PEEEP_PROTOTYPE_API UPPQuickSlotWidget : public UPPCustomWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct	() override;

public:
	// 퀵슬롯 초기화 함수
	void Init();

	// 퀵슬롯 업데이트 함수
	void UpdateQuickSlot();

	UFUNCTION()
	void SetQuickSlotWidget	(UPPQuickSlotWidget* source);

private:
	void SetType(ESlotType Type);

	// 슬롯들을 저장하기 위한 배열
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UPPSlot>> Slots;

	// 현재 인벤토리의 타입(파츠)
	ESlotType InventorySlotType;

private:
};
