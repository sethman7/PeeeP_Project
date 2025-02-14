// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionObject/Electric/PPElectricCharageStation.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "Component/PPElectricDischargeComponent.h"
#include "GameMode/PPPlayerState.h"
#include "Components/AudioComponent.h"


// Sets default values
APPElectricCharageStation::APPElectricCharageStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootSceneComponent);

	TriggerFloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElectricChager"));
	TriggerFloorMesh->SetupAttachment(RootSceneComponent);
	TriggerFloorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(TriggerFloorMesh);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPElectricCharageStation::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APPElectricCharageStation::OnOverlapEnd);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->VolumeMultiplier = 0.5f;

	bIsActivate = false;
}

// Called when the game starts or when spawned
void APPElectricCharageStation::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != SaveSound)
	{
		AudioComponent->Sound = SaveSound;
	}
}

// Called every frame
void APPElectricCharageStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActivate)
	{
		ElectricDischargeComponent->AddCurrentCapacity(5.0f * DeltaTime);
	}
}

void APPElectricCharageStation::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this) && !bIsActivate)
	{
		if (APPCharacterPlayer* player = Cast<APPCharacterPlayer>(OtherActor))
		{
			UE_LOG(LogTemp, Log, TEXT("Player Overlap Begin"));
			ElectricDischargeComponent = player->GetElectricDischargeComponent();

			SaveGame(player);
		}

	}
}

void APPElectricCharageStation::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && bIsActivate)
	{
		if (APPCharacterPlayer* player = Cast<APPCharacterPlayer>(OtherActor))
		{
			UE_LOG(LogTemp, Log, TEXT("Player Overlap End"));
			bIsActivate = false;
		}
	}
}

void APPElectricCharageStation::SaveGame(APPCharacterPlayer* InPlayer)
{
	APPPlayerState* PlayerState = InPlayer->GetPlayerState<APPPlayerState>();
	if (IsValid(PlayerState))
	{
		//현재 액터의 위치에 스폰되면 땅에 박혀서 임의로 조정, 임의값 사용이 문제되면 추후 SceneComponent를 사용하여 위치 조정
		//FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 10.0f);
		PlayerState->SetSpawnActorLocation(this);
		//PlayerState->SetSpawnLocation(SpawnLocation);

		if (nullptr != AudioComponent)
		{
			AudioComponent->Play();
		}
		if (nullptr != ActiveMaterial)
		{
			TriggerFloorMesh->SetMaterial(1, ActiveMaterial);
		}
	}
	bIsActivate = true;

}

