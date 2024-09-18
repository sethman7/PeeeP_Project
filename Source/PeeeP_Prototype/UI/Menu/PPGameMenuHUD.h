// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPGameMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPGameMenuHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPPGameMenuHUD(const FObjectInitializer& ObjectInitializer);
	
protected:

	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere, Category = Button)
	TObjectPtr<class UPPMenuButtonWidget> StartButton;

	UPROPERTY(VisibleAnywhere, Category = Button)
	TObjectPtr<class UPPMenuButtonWidget> LoadButton;
	
	UPROPERTY(VisibleAnywhere, Category = Button)
	TObjectPtr<class UPPMenuButtonWidget> SettingButton;
	
	UPROPERTY(VisibleAnywhere, Category = Button)
	TObjectPtr<class UPPMenuButtonWidget> ExitButton;

protected:

	UFUNCTION()
	void StartButtonClick();

	UFUNCTION()
	void LoadButtonClick();

	UFUNCTION()
	void SettingButtonClick();

	UFUNCTION()
	void ExitButtonClick();
};
