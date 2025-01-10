// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Parts/PartsData/PPPartsDataBase.h"
#include "PPInventoryPartsItem.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPInventoryPartsItem : public UObject
{
	GENERATED_BODY()
	
public:
	UPPInventoryPartsItem();

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPPPartsDataBase> PartsData;

	// 파츠당 보유 가능한 개수는 1개이므로 큰 상관 안써도 됨.
	UPROPERTY(VisibleAnywhere)
	int32 ItemQuantity;
};
 