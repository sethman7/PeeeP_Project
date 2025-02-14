// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsItemBox/PPPartsItemBox.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APPPartsItemBox::APPPartsItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemBoxMesh"));
	SetRootComponent(ItemBoxMesh);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(ItemBoxMesh);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPPartsItemBox::OnOverlapBegin);

	if (ItemBoxMaterial)
	{
		ItemBoxMesh->SetMaterial(0, ItemBoxMaterial);
	}
}

// Called when the game starts or when spawned
void APPPartsItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPPartsItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the item box
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += DeltaTime * 50.0f;
	SetActorRotation(NewRotation);

}

void APPPartsItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (APPCharacterPlayer* player = Cast<APPCharacterPlayer>(OtherActor))
		{
			UE_LOG(LogTemp, Log, TEXT("Player Overlap Begin"));
			InventoryComponent = player->GetInventoryComponent();
			int32 TempOutItemQuntity = 0;
			InventoryComponent->AddItem(PartsClassName, 1, TempOutItemQuntity);

			if (nullptr != EarnSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), EarnSound, 0.75f);
			}

			Destroy();
		}
	}
}

