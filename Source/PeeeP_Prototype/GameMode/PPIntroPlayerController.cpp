// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPIntroPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/Intro/PPIntroWidget.h"

APPIntroPlayerController::APPIntroPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> IntroClassRef(TEXT("/Game/UI/Intro/WBP_Intro.WBP_Intro_C"));
	if (IntroClassRef.Class)
	{
		Intro = IntroClassRef.Class;
	}
}

void APPIntroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly LobbyInputMode;
	SetInputMode(LobbyInputMode);

	IntroUI = CreateWidget<UUserWidget>(this, Intro);
	UPPIntroWidget* LobbyUI = Cast<UPPIntroWidget>(IntroUI);
	if (LobbyUI)
	{

		UE_LOG(LogTemp, Log, TEXT("Widget"));
		LobbyUI->AddToViewport();
		LobbyUI->PlayIntro();
	}
}
