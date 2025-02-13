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

			PlayerState->PlayRespawnSequence();
		}
		AController* Controller = Character->GetController();
		if (IsValid(Controller))
		{
			FVector CharacterForwardVector = Character->GetActorForwardVector();
			float Yaw = FMath::Atan2(CharacterForwardVector.Y, CharacterForwardVector.X) * (180.0f/PI);

			Controller->SetControlRotation(FRotator(0.0f, Yaw, 0.0f));
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
				PlayerState->SetSpawnActorLocation(PlayerStart);
			}
		}
	}
}

void APPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//ChangeMenuWidget(StartingWidgetClass);
}
