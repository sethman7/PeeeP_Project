// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APPLobbyPlayerController();

	virtual void BeginPlay() override;
	
private:

	TSubclassOf<class UUserWidget> LobbyUIClass;

	TObjectPtr<class UUserWidget> LobbyUI;
};
