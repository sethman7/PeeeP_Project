// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"
#include "UI/Menu/PPPauseMenyHUD.h"

APPPlayerController::APPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPPPauseMenyHUD> PauseMenuRef(TEXT("/Game/UI/MenuHUD/WBP_PauseMenu.WBP_PauseMenu_C"));
	if (PauseMenuRef.Class)
	{
		PauseUIClass = PauseMenuRef.Class;
	}
}

void APPPlayerController::OpenMenu()
{
	SetPause(true);

	PauseUI->SetVisibility(ESlateVisibility::Visible);
	
	FInputModeUIOnly InputUIOnly;
	SetInputMode(InputUIOnly);
	bShowMouseCursor = true;
}

void APPPlayerController::CloseMenu()
{
	PauseUI->SetVisibility(ESlateVisibility::Hidden);

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	bShowMouseCursor = false;

	SetPause(false);
}

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	PauseUI = CreateWidget<UUserWidget>(this, PauseUIClass);
	UPPPauseMenyHUD* PauseMenu = Cast<UPPPauseMenyHUD>(PauseUI);
	if (PauseMenu)
	{
		PauseMenu->AddToViewport(1);
		CloseMenu();
	}
}
