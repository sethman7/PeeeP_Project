// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPKeyManualUI.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API UPPKeyManualUI : public UUserWidget
{
	GENERATED_BODY()
	
	void NativeConstruct() override;

protected:
	uint32 KeyManualImage;

	UPROPERTY()
	TObjectPtr <class UImage> KeyManualImage_WASD;
	UPROPERTY()
	TObjectPtr <class UImage> KeyManualImage_Jump;
	UPROPERTY()
	TObjectPtr <class UImage> KeyManualImage_Charge;
	UPROPERTY()
	TObjectPtr <class UImage> KeyManualImage_Discharge;
	UPROPERTY()
	TObjectPtr <class UImage> KeyManualImage_Equip;
	UPROPERTY()
	TObjectPtr <class UImage> KeyManualImage_Unequip;
	UPROPERTY()
	TObjectPtr <class UImage> KeyManualImage_Grab01;
	UPROPERTY()
	TObjectPtr <class UImage> KeyManualImage_Grab02;

public:
	void SetAllKeyManualImageHidden();
	void SetKeyManualImageVisible(uint32 index);
	
};
