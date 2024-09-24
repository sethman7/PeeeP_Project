// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPPunchTrap.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APPPunchTrap::APPPunchTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(SkeletalMesh);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	TriggerBox->SetupAttachment(SkeletalMesh, TEXT("Bone010Socket"));
	TriggerBox->bDynamicObstacle = true;
	TriggerBox->GetAttachParent();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPPunchTrap::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APPPunchTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPPunchTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPPunchTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Triggered!"));
	APPCharacterPlayer* PlayerCharacter = Cast<APPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Cast Completed!"));
		PlayerCharacter->GetCharacterMovement()->AddImpulse(this->GetActorRightVector() * -500.0f, true);
		PlayerCharacter->GetCharacterMovement()->AddImpulse(this->GetActorUpVector() * 300.0f, true);
	}
}

