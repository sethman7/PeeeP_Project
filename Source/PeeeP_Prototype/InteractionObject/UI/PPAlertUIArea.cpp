// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/UI/PPAlertUIArea.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "TimerManager.h"
#include "Interface/UI/PPElectricHUDInterface.h"

// Sets default values
APPAlertUIArea::APPAlertUIArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPAlertUIArea::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APPAlertUIArea::OnOverlapEnd);

	KeyManualIndex = 0;
}

// Called when the game starts or when spawned
void APPAlertUIArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPAlertUIArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPAlertUIArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (APPCharacterPlayer* player = Cast<APPCharacterPlayer>(OtherActor))
		{
			UE_LOG(LogTemp, Log, TEXT("[APPAlertUIArea] Player Overlap Begin."));
			// UI 이미지 변경 테스트
			IPPElectricHUDInterface* ElectircHUDInterface = Cast<IPPElectricHUDInterface>(player);
			if (ElectircHUDInterface)
			{
				UE_LOG(LogTemp, Log, TEXT("Succeessed Cast to IPPElectricHUDInterface."));
				if (ElectircHUDInterface->ShowKeyManualDelegate.IsBound())
				{
					UE_LOG(LogTemp, Log, TEXT("Succeessed to Bound ShowKeyManualDelegate."));
					ElectircHUDInterface->ShowKeyManualDelegate.Broadcast(KeyManualIndex);

					FTimerDelegate TimerDelegate;
					TimerDelegate.BindUObject(this, &APPAlertUIArea::SetKeyMaunalWidgetHiddenByTimer, ElectircHUDInterface);
					GetWorld()->GetTimerManager().SetTimer(AutoHiddenTimer, TimerDelegate, 3.0f, false);
					UE_LOG(LogTemp, Warning, TEXT("Timer"));
				}
			}
		}
	}
}

void APPAlertUIArea::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		if (APPCharacterPlayer* player = Cast<APPCharacterPlayer>(OtherActor))
		{
			UE_LOG(LogTemp, Log, TEXT("[APPAlertUIArea] Player Overlap End."));
		}
	}
}

void APPAlertUIArea::SetKeyMaunalWidgetHiddenByTimer(IPPElectricHUDInterface* ElectircHUDInterface)
{
	UE_LOG(LogTemp, Warning, TEXT("Hidden"));
	ElectircHUDInterface->ShowKeyManualDelegate.Broadcast(0);
	// GetWorld()->GetTimerManager().ClearTimer(AutoHiddenTimer);
}

