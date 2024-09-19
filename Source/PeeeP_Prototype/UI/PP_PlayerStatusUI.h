// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP_PlayerStatusUI.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPP_PlayerStatusUI : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr <class UProgressBar> BatteryProgressBar;

public:
	void SetBatteryProgressBarPercent(float rate);
};
