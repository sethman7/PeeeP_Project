// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPIntroWidget.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPIntroWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void StartGame();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMediaPlayer> IntroMedia;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMediaSource> IntroMediaSource;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMediaSource> IntroMediaLoopSource;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMediaTexture> IntroMediaTexture;


public:

	void PlayIntro();

	UFUNCTION()
	void PlayLoop();

};
