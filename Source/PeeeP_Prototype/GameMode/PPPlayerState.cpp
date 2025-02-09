// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerState.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequence.h"
#include "MovieSceneBindingOverrides.h"
#include "Interface/PPSavepointInterface.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"


APPPlayerState::APPPlayerState()
{
	/*static ConstructorHelpers::FObjectFinder<ULevelSequence> RestartSequenceRef(TEXT("/Script/LevelSequence.LevelSequence'/Game/PeeeP_Sequence/LS_Respawn.LS_Respawn'"));
	if(RestartSequenceRef.Object != nullptr)
	{
		RestartSequence = RestartSequenceRef.Object;
	}*/
}

void APPPlayerState::SetSpawnLocation(FVector Location)
{
	SpawnLocation = Location;

	RestartSequenceActor->SetActorLocation(Location);
}

void APPPlayerState::SetSpawnActorLocation(AActor* InActor)
{
	SpawnPointActor = InActor;

	SpawnLocation = InActor->GetActorLocation();

	UDefaultLevelSequenceInstanceData* DefaultInstanceData = NewObject<UDefaultLevelSequenceInstanceData>();
	DefaultInstanceData->TransformOriginActor = InActor;
	if (IsValid(RestartSequenceActor))
	{
		RestartSequenceActor->DefaultInstanceData = DefaultInstanceData;
	}
}

void APPPlayerState::PlayRespawnSequence()
{
	if (IsValid(SpawnPointActor) && SpawnPointActor->Implements<UPPSavepointInterface>())
	{
		UE_LOG(LogTemp, Log, TEXT("Play Sequence"));
		RestartSequencePlayer->Play();
	}
}

void APPPlayerState::BeginPlay()
{
	Super::BeginPlay();

	static const FString RestartSequencePath = TEXT("/Game/PeeeP_Sequence/LS_Respawn.LS_Respawn");
	RestartSequence = LoadObject<ULevelSequence>(nullptr, *RestartSequencePath);

	if (IsValid(RestartSequence))
	{
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bAutoPlay = false;
		Settings.bDisableMovementInput = true;
		Settings.bDisableLookAtInput = true;
		Settings.bHidePlayer = false;
		Settings.bHideHud = true;

		RestartSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), RestartSequence, Settings, RestartSequenceActor);

		RestartSequenceActor->SetBindingByTag(FName("Player"), { GetPawn() });
		RestartSequenceActor->bOverrideInstanceData = true;
		
		UDefaultLevelSequenceInstanceData* DefaultInstanceData = NewObject<UDefaultLevelSequenceInstanceData>();
		DefaultInstanceData->TransformOriginActor = GetPawn();
		RestartSequenceActor->DefaultInstanceData = DefaultInstanceData;	

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundActors);

		RestartSequenceActor->SetBindingByTag(FName("PostProcessVolume"), { FoundActors[0]});

		UE_LOG(LogTemp, Warning, TEXT("LevelSequnceSetting"));
	}
}
