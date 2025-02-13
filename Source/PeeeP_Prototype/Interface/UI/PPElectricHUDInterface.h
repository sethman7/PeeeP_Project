// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPElectricHUDInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FUsingElectricDelegate, float)
DECLARE_MULTICAST_DELEGATE_TwoParams(FChargingLevelDelegate, int, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FShowKeyManualDelegate, uint32)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPElectricHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PEEEP_PROTOTYPE_API IPPElectricHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 전기량이 변화할 때 사용하는 델리게이트
	FUsingElectricDelegate ElectircCapacityDelegate;

	// 충전된 양에 따라 게이지를 처리할 때 사용하는 델리게이트
	FChargingLevelDelegate ChargingLevelDelegate;

	// UI 이미지 변경
	FShowKeyManualDelegate ShowKeyManualDelegate;
};
