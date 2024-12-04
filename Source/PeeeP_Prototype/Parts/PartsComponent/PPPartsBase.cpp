// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPPartsBase.h"
#include "Parts/PartsData/PPGrabPartsData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UPPPartsBase::UPPPartsBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//Controller을 얻어오기 위해 APawn으로 캐스팅.
	Owner = Cast<APawn>(GetOwner()); 
}



// Called when the game starts
void UPPPartsBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPPPartsBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPPPartsBase::SetPartsActive(bool flag)
{
	PrimaryComponentTick.bCanEverTick = flag;
	SetComponentTickEnabled(flag);
	SetActive(flag);

	if (Owner == nullptr) return;

	if (flag == false) 
	{
		APlayerController* PlayerController = CastChecked<APlayerController>(Owner->GetController());
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (PartsData)
			{
				Subsystem->RemoveMappingContext(PartsData->PartsMappingContext);
			}
		}
	}
	else
	{
		SetUp();
	}

}
