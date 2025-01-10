// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPElevatorDoorTrigger.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"

// Sets default values
APPElevatorDoorTrigger::APPElevatorDoorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;
	Trigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	Duration = 3.0f;
	DoorSpeed = 20.0f;
	DoorState = EDoorState::Stay;
}

// Called when the game starts or when spawned
void APPElevatorDoorTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	bool bEndDele = Trigger->OnComponentEndOverlap.IsBound();
	UE_LOG(LogTemp, Warning, TEXT("Set End"));
}

void APPElevatorDoorTrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LeftDoorStartPos = LeftDoor->GetActorLocation();
	RightDoorStartPos = RightDoor->GetActorLocation();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &APPElevatorDoorTrigger::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &APPElevatorDoorTrigger::OnOverlapEnd);
}

// Called every frame
void APPElevatorDoorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (DoorState)
	{
		case EDoorState::Open:
		{
			if (ElapsedTime < Duration)
			{
				LeftDoor->SetActorLocation((LeftDoor->GetActorLocation() - FVector(0.0f, DoorSpeed * DeltaTime, 0.0f)));
				RightDoor->SetActorLocation((RightDoor->GetActorLocation() + FVector(0.0f, DoorSpeed * DeltaTime, 0.0f)));
				ElapsedTime += DeltaTime;
			}
			break;
		}
		case EDoorState::Close:
		{
			if (ElapsedTime > 0.0f)
			{
				LeftDoor->SetActorLocation((LeftDoor->GetActorLocation() + FVector(0.0f, DoorSpeed * DeltaTime, 0.0f)));
				RightDoor->SetActorLocation((RightDoor->GetActorLocation() - FVector(0.0f, DoorSpeed * DeltaTime, 0.0f)));
				ElapsedTime -= DeltaTime;
			}
			break;
		}
		default:
			break;
	}


}

void APPElevatorDoorTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(OtherActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("OverlapBegin"));
		DoorState = EDoorState::Open;
	}
}

void APPElevatorDoorTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapEnd"));
	APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(OtherActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("OverlapEnd"));
		DoorState = EDoorState::Close;
	}
}