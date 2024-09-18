// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPLobbyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/Menu/PPGameMenuHUD.h"

APPLobbyPlayerController::APPLobbyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyUIClassRef(TEXT("/Game/UI/MenuHUD/WBP_GameMenu.WBP_GameMenu_C"));
	if (LobbyUIClassRef.Class)
	{
		LobbyUIClass = LobbyUIClassRef.Class;
	}
}


void APPLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly LobbyInputMode;
	SetInputMode(LobbyInputMode);
	bShowMouseCursor = true;

	LobbyUI = CreateWidget<UUserWidget>(this, LobbyUIClass);
	UPPGameMenuHUD* LobbyMenuWidget = Cast<UPPGameMenuHUD>(LobbyUI);
	if (LobbyMenuWidget)
	{
		LobbyUI->AddToViewport();
	}
}
