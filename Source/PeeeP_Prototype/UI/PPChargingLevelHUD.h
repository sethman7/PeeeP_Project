// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/PPCustomWidget.h"
#include "PPChargingLevelHUD.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPChargingLevelHUD : public UPPCustomWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	// Progress Bar
	UPROPERTY()
	TObjectPtr<class UProgressBar> Level1Bar;
	UPROPERTY()
	TObjectPtr<class UProgressBar> Level2Bar;
	UPROPERTY()
	TObjectPtr<class UProgressBar> Level3Bar;


	// Glow
	UPROPERTY()
	TObjectPtr<class UImage> Level1Glow;
	UPROPERTY()
	TObjectPtr<class UImage> Level2Glow;
	UPROPERTY()
	TObjectPtr<class UImage> Level3Glow;

public:
	void SetChargingCapacityAmount(int LevelOfBar, float Rate);
	void SetGaugeGlowEffectVisible(bool bFlag);
	void SetWidgetVisible(bool Flag);
};
