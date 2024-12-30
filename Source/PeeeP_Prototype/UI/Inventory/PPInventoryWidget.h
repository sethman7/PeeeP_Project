// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/PPCustomWidget.h"
#include "../../Inventory/ESlotType.h"
#include "PPInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPInventoryWidget : public UPPCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	// 인벤토리 초기화 함수
	void Init();
	// 인벤토리 아이템 슬롯 업데이트 함수
	void UpdateInventorySlot();
	
private:
	UFUNCTION()
	void SetPartsType	();

// Main
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_MainButton;

// Header
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Parts;

// Tail
public:
	
private:
	// 현재 인벤토리의 타입을 지정하기 위한 함수
	void SetType(ESlotType Type);

	// 슬롯들을 저장하기 위한 배열
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UPPSlot>> Slots;

	// 현재 인벤토리의 타입(파츠)
	ESlotType InventorySlotType;
};
