// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PPInventoryPartsItem.h"
#include "Inventory/ESlotType.h"
#include "../UI/Inventory/PPSlot.h"
#include "../UI/Inventory/PPQuickSlotWidget.h"
#include "PPInventoryComponent.generated.h"

// 인벤토리 내용 변경 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnChangedInventoryDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEEEP_PROTOTYPE_API UPPInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPPInventoryComponent();

	virtual void InitializeComponent() override;

public:
	FOnChangedInventoryDelegate OnChangeInven;

	void ClearUsingItem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 파츠
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UPPInventoryPartsItem>> PartsItems;
	// 소비
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UPPInventoryPartsItem>> ConsumableItems;
	// 기타
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UPPInventoryPartsItem>> OtherItems;

	// 최대 인벤토리 슬롯 수
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxInventorySlotNum;

	// 슬롯 한 칸당 저장 가능한 아이템의 수(파츠는 1 고정)
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxItemNum;

public:
	// Getter
	FORCEINLINE TArray<TObjectPtr<class UPPInventoryPartsItem>> GetPartsItems() { return PartsItems; }
	FORCEINLINE TArray<TObjectPtr<class UPPInventoryPartsItem>> GetConsumableItems() { return ConsumableItems; }
	FORCEINLINE TArray<TObjectPtr<class UPPInventoryPartsItem>> GetOtherItems() { return OtherItems; }

	// 아이템 추가
	bool AddItem(FName InItemName, int32 InItemQuantity, int32& OutItemQuantity);
	// 아이템 사용
	void UseItem(int32 InSlotIndex, ESlotType InventoryType);
	// 현재 인덱스 기반 아이템 사용
	void UseItemCurrentIndex(ESlotType InventoryType);
	// 아이템 교체
	void SwapItem(int32 InprevIndex, int32 InCurrentIndex);


	// 인벤토리 정렬
	void SortItem();

protected:
	// 인벤토리 초기화
	void InitInventory();
	// 아이템 삭제
	void RemoveItem(int32 InSlotIndex, ESlotType InventoryType);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 현재 선택된 슬롯 인덱스
	int32 CurrentSlotIndex;
	// 현재 장착된 파츠 슬롯 인덱스
	int32 UsingSlotIndex;

public:
	void ModifyCurrentSlotIndex(int32 Value);

private:
	// 퀵슬롯 위젯
	UPROPERTY()
	TObjectPtr <class UPPQuickSlotWidget> QuickSlotWidget;

	// 슬롯들을 저장하기 위한 배열
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UPPSlot>> Slots;

public:
	// 인벤토리 컴포넌트 내 퀵슬롯 위젯 설정을 위한 함수
	UFUNCTION()
	void SetQuickSlotWidget(UPPQuickSlotWidget* widget);
};
