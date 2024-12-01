// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/PPCustomWidget.h"
#include "PPSlot.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPSlot : public UPPCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	// 슬롯 초기화
	void Init();
	// 슬롯 업데이트
	void UpdateSlot();

public:
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
};
