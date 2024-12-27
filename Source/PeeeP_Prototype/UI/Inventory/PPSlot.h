// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/PPCustomWidget.h"
#include "../../Inventory/ESlotType.h"
#include "PPSlot.generated.h"

// 업데이트 델리게이트 선언
DECLARE_DELEGATE(FOnUpdateSlotDelegate);

// 래퍼 구조체를 선언
USTRUCT(BlueprintType)
struct FUpdateSlotDelegateWrapper
{
	GENERATED_BODY()
	
	FUpdateSlotDelegateWrapper() {}
	FUpdateSlotDelegateWrapper(const FOnUpdateSlotDelegate& InSlotDelegate) : SlotDelegate(InSlotDelegate) { }

	FOnUpdateSlotDelegate SlotDelegate;
};

UCLASS()
class PEEEP_PROTOTYPE_API UPPSlot : public UPPCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	// 슬롯 초기화
	void Init();
	// 슬롯 타입 지정
	void SetType(ESlotType Type);
	// 슬롯 업데이트
	void UpdateSlot();

public:
	// 현재 슬롯의 타입
	UPROPERTY(EditAnywhere, Category = "Slot")
	ESlotType SlotType;

	// 슬롯에 지정될 이미지
	UPROPERTY(VisibleAnywhere, Category = "Slot")
	TObjectPtr<class UImage> IMG_Item;

	// 슬롯에 지정될 아이템의 수량
	UPROPERTY(VisibleAnywhere, Category = "Slot")
	TObjectPtr<class UTextBlock> TXT_Quantity;

	// 현재 슬롯의 인덱스
	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 SlotIndex;

protected:
	// 개별 슬롯 업데이트 함수
	// 이 프로젝트에서는 쓸 일이 그렇게 많지 않을 것 같다.
	void UpdatePartsSlot();

	// 빈 칸에 적용하기 위한 투명 텍스쳐
	UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UTexture2D> DefaultTexture;

protected:
	// <슬롯 타입, 래퍼 구조체> 형식의 Map 선언
	UPROPERTY()
	TMap<ESlotType, FUpdateSlotDelegateWrapper> SlotUpdateActions;
	// 이후 https://velog.io/@apth1023/UE5-%EC%95%84%EC%9D%B4%ED%85%9C-%EC%9D%B8%EB%B2%A4%ED%86%A0%EB%A6%AC-%EC%8B%9C%EC%8A%A4%ED%85%9C-3 계속 이어가면됨
};
