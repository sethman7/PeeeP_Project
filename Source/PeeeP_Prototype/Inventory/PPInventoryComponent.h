// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PPInventoryPartsItem.h"
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 파츠
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UPPInventoryPartsItem>> PartsItems;

	// 최대 인벤토리 슬롯 수
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxInventorySlotNum;

	// 슬롯 한 칸당 저장 가능한 아이템의 수(파츠는 1 고정)
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxItemNum;

public:
	// Getter
	FORCEINLINE TArray<TObjectPtr<class UPPInventoryPartsItem>> GetPartsItems() { return PartsItems; }

	// 아이템(파츠) 추가
	bool AddItem(FName InItemName, int32 InItemQuantity, int32& OutItemQuantity);
	// 아이템(파츠) 사용
	void UseItem(int32 InSlotIndex);
	// 아이템(파츠) 교체
	void SwapItem(int32 InprevIndex, int32 InCurrentIndex);

	// 인벤토리 정렬
	void SortItem();

protected:
	// 인벤토리 초기화
	void InitInventory();
	// 아이템 삭제
	void RemoveItem(int32 InSlotIndex);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
