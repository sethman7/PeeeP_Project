// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameModeBase.h"
#include "PPPlayerController.h"
#include "PPPlayerState.h"
#include "Character/PPCharacterPlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"

APPGameModeBase::APPGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Characters/BP_PPCharacterPlayer.BP_PPCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/PeeeP_Prototype.PPPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APPPlayerState> PlayerStateClassRef(TEXT("/Script/PeeeP_Prototype.PPPlayerState"));
	if (PlayerStateClassRef.Class)
	{
		PlayerStateClass = PlayerStateClassRef.Class;
	}
}

//void APPGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
//{
//	if (CurrentWidget != nullptr)
//	{
//		CurrentWidget->RemoveFromParent();
//		CurrentWidget = nullptr;
//	}
//
//	if (NewWidgetClass != nullptr)
//	{
//		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
//		if (CurrentWidget != nullptr)
//		{
//			CurrentWidget->AddToViewport();
//		}
//	}
//}

void APPGameModeBase::MoveCharacterToSpawnLocation(APPCharacterPlayer* Character)
{
	if (IsValid(Character))
	{
		APPPlayerState* PlayerState = Cast<APPPlayerState>(Character->GetPlayerState());
		if (IsValid(PlayerState))
		{
			Character->SetActorLocation(PlayerState->GetSpawnLocation());
		}
	}
}

void APPGameModeBase::SetInitialSpawnLocation(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		APPPlayerState* PlayerState = Cast<APPPlayerState>(PlayerController->PlayerState);
		if (IsValid(PlayerState))
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(FindPlayerStart(PlayerController));
			if (IsValid(PlayerStart))
			{
				PlayerState->SetSpawnLocation(PlayerStart->GetActorLocation());
			}
		}
	}
}

void APPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//ChangeMenuWidget(StartingWidgetClass);


}
