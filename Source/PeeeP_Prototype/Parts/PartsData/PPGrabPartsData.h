// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/PartsData/PPPartsDataBase.h"
#include "../PartsComponent/PPPartsBase.h"
#include "PPGrabPartsData.generated.h"

/**
 개별 파츠에 대한 데이터를 보관하는 클래스 
 이 클래스에서 해야하는 일
 1. 필요한 인풋 액션
 2. 이펙트 관련 NyagaraSystem
  
  
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPGrabPartsData : public UPPPartsDataBase
{
	GENERATED_BODY()
	
public:
	UPPGrabPartsData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PPPartsData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> GrabAction;
};
