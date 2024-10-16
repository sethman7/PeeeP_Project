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
	
	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_StartButton;

	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_LoadButton;
	
	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_SettingButton;
	
	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_ExitButton;

	UPROPERTY(VisibleAnywhere, Category = Button)
	TObjectPtr<class APPLobbyActor> LobbyActor;

	TSubclassOf<class APPLobbyActor> LobbyActorClass;

protected:

	UFUNCTION()
	void StartButtonClick();

	UFUNCTION()
	void LoadButtonClick();

	UFUNCTION()
	void SettingButtonClick();

	UFUNCTION()
	void ExitButtonClick();

	UFUNCTION()
	void StartButtonHover();

	UFUNCTION()
	void LoadButtonHover();

	UFUNCTION()
	void SettingButtonHover();

	UFUNCTION()
	void ExitButtonHover();
};
