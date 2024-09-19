// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPMenuButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPPMenuButtonWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	//UFUNCTION()
	//void HoveredEvent();

	class UButton* GetButton();

	TObjectPtr<class UButton> Button;

	TObjectPtr<class UImage> ButtonImage;

protected:


};
