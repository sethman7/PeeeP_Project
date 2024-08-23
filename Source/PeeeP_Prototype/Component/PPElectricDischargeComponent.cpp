// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PPElectricDischargeComponent.h"
#include "CollisionQueryParams.h"
#include "Interface/PPElectricObjectInterface.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PPCharacterPlayer.h"

// Sets default values for this component's properties
UPPElectricDischargeComponent::UPPElectricDischargeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DischargeMode = EDischargeMode::Sphere;
	MaxChargingTime = 3.0f;
	CurrentChargingTime = 0.0f;
	RechargingDelay = 1.0f;
	MoveSpeedReductionRate = 0.5f;
	bRechargingEnable = true;

	bChargeStart = false;
}


// Called when the game starts
void UPPElectricDischargeComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UPPElectricDischargeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPPElectricDischargeComponent::Charging()
{
	if (!bRechargingEnable)
	{
		return;
	}

	if (!bChargeStart)
	{
		APPCharacterPlayer* OwnerCharacter = Cast<APPCharacterPlayer>(GetOwner());
		if (OwnerCharacter)
		{
			OwnerCharacter->ReduationMaxWalkSpeedRatio(MoveSpeedReductionRate);
		}

		bChargeStart = true;
	}

	if (CurrentChargingTime >= MaxChargingTime)
	{
		if (!AutoDischargeTimeHandler.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(AutoDischargeTimeHandler, this, &UPPElectricDischargeComponent::Discharge, 1.0f, false);
			UE_LOG(LogTemp, Warning, TEXT("Timer"));
		}
		return;
	}

	CurrentChargingTime += GetWorld()->GetDeltaSeconds();

	CurrentChargingTime = FMath::Clamp(CurrentChargingTime, 0, MaxChargingTime);

	UE_LOG(LogTemp, Log, TEXT("Charging Time: %f"), CurrentChargingTime);
}

void UPPElectricDischargeComponent::Discharge()
{
	if (!bRechargingEnable)
	{
		return;
	}

	bChargeStart = false;

	AActor* Owner = GetOwner();

	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(ElectricDischarge), false, Owner);

	if (DischargeMode == EDischargeMode::Capsule)
	{
		FHitResult OutHitResult;

		float DefaultEndRange = 300.0f;

		float SphereRadius = 50.0f;

		FVector Start = Owner->GetActorLocation() + Owner->GetActorForwardVector()* SphereRadius;
		FVector End = Start + Owner->GetActorForwardVector() * (DefaultEndRange + CurrentChargingTime * 50.0f);

		bool bIsHit = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel5 /*ÀÓ½Ã·Î ±×·¦ ³Ö¾îµÒ*/,

			FCollisionShape::MakeSphere(SphereRadius), CollisionParam);

		if (bIsHit)
		{
			IPPElectricObjectInterface* HitElectricObject = CastChecked<IPPElectricObjectInterface>(OutHitResult.GetActor());
			if (HitElectricObject)
			{
				UE_LOG(LogTemp, Log, TEXT("Electric Object Hit!"));
				HitElectricObject->Charge();
				UE_LOG(LogTemp, Log, TEXT("IsHit : Capsule"))
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Discharge Capsule %f"), CurrentChargingTime);
	}
	else if (DischargeMode == EDischargeMode::Sphere)
	{
		TArray<FOverlapResult> OutOverlapResults;

		float DefaultSphereRadius = 300.0f;
		float SphereRadius = DefaultSphereRadius + CurrentChargingTime * 50.0f;

		bool bIsHit = GetWorld()->OverlapMultiByChannel(OutOverlapResults, Owner->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel5 /*ÀÓ½Ã·Î ±×·¦ ³Ö¾îµÒ*/,
			FCollisionShape::MakeSphere(CurrentChargingTime), CollisionParam);

		if (bIsHit)
		{
			for (const FOverlapResult OverlapResult : OutOverlapResults)
			{
				IPPElectricObjectInterface* HitElectricObject = CastChecked<IPPElectricObjectInterface>(OverlapResult.GetActor());
				if (HitElectricObject)
				{
					HitElectricObject->Charge();
					UE_LOG(LogTemp, Log, TEXT("IsHit : Sphere Num: %d"), OutOverlapResults.Num());
				}
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Discharge Sphere %f"), CurrentChargingTime);
	}

	APPCharacterPlayer* OwnerCharacter = Cast<APPCharacterPlayer>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->RevertMaxWalkSpeed();
	}

	CurrentChargingTime = 0.0f;
	bRechargingEnable = false;

	if (!AutoDischargeTimeHandler.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoDischargeTimeHandler);
	}
	GetWorld()->GetTimerManager().SetTimer(RechargingDelayTimeHandler, this, &UPPElectricDischargeComponent::SetbRecharging, RechargingDelay, false);

}

void UPPElectricDischargeComponent::ChangeDischargeMode()
{
	if (DischargeMode == EDischargeMode::Capsule)
	{
		DischargeMode = EDischargeMode::Sphere;

		UE_LOG(LogTemp, Log, TEXT("Change Discharge Mode from Capsule to Sphere"));
	}
	else if(DischargeMode == EDischargeMode::Sphere)
	{
		DischargeMode = EDischargeMode::Capsule;

		UE_LOG(LogTemp, Log, TEXT("Change Discharge Mode from Sphere to Capsule"));
	}
}

void UPPElectricDischargeComponent::SetbRecharging()
{
	bRechargingEnable = true;
}

