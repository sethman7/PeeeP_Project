// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionObject/Electric/PPElectricCharageStation.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "Component/PPElectricDischargeComponent.h"


// Sets default values
APPElectricCharageStation::APPElectricCharageStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerFloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElectricChager"));
	SetRootComponent(TriggerFloorMesh);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(TriggerFloorMesh);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPElectricCharageStation::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APPElectricCharageStation::OnOverlapEnd);

	bIsActivate = false;
}

// Called when the game starts or when spawned
void APPElectricCharageStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPElectricCharageStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActivate)
	{
		ElectricDischargeComponent->ChargeElectric(1.0f * DeltaTime);
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
			bIsActivate = true;
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

