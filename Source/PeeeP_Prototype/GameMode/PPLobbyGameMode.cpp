// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPLobbyGameMode.h"

APPLobbyGameMode::APPLobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Game/PeeeP_Level/MainLobby/BP_LobbyPlayerController.BP_LobbyPlayerController_C"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
}
