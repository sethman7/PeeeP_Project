// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPPauseMenyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPPauseMenyHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPPPauseMenyHUD(const FObjectInitializer& ObjectInitializer);
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_ResomeButton;

	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_PauseSettingButton;

	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_TitleButton;

	UPROPERTY(VisibleAnywhere, Category = Button)
	TObjectPtr<class APPLobbyActor> LobbyActor;

	TSubclassOf<class APPLobbyActor> LobbyActorClass;

protected:

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void ContinueButtonClick();

	UFUNCTION()
	void SettingButtonClick();

	UFUNCTION()
	void ExitButtonClick();

	UFUNCTION()
	void ContinueButtonHovered();

	UFUNCTION()
	void SettingButtonHovered();

	UFUNCTION()
	void ExitButtonHovered();
};
