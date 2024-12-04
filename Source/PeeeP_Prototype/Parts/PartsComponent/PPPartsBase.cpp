// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPPartsBase.h"
#include "Parts/PartsData/PPGrabPartsData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UPPPartsBase::UPPPartsBase()
{
	//항상 true로 설정해야 함. false 설정시, TickFunction의 등록 자체를 막아버리기 때문에,절대로 Tick을 사용하지 않을 경우에만 false로 설정함.
	PrimaryComponentTick.bCanEverTick = true; 

	//Tick을 인위적으로 조작 가능함.
	PrimaryComponentTick.bStartWithTickEnabled = false;

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
	SetActive(flag); //물리,충돌 설정. 
	PrimaryComponentTick.bStartWithTickEnabled = flag; //TickComponent 설정.


	if (Owner == nullptr) return; //Owner가 유효하지 않다면.


	if (flag == false) 
	{
		//파츠가 변경될 때, 기존의 파츠에 맵핑 연결 끊기.  
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
		//새 파츠가 장착될 때, 초기화 작업.
		SetUp();
	}

}
