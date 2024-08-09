// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PPElectricDischargeComponent.h"
#include "CollisionQueryParams.h"
#include "Interface/PPElectricObjectInterface.h"

// Sets default values for this component's properties
UPPElectricDischargeComponent::UPPElectricDischargeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DischargeMode = EDischargeMode::Sphere;
	MaxChargingTime = 3.0f;
	CurrentChargingTime = 0.0f;
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
	float PrevChargeTime = CurrentChargingTime;

	if (PrevChargeTime >= MaxChargingTime)
	{
		return;
	}

	CurrentChargingTime += GetWorld()->GetDeltaSeconds();
	if (CurrentChargingTime >= MaxChargingTime)
	{
		CurrentChargingTime = MaxChargingTime;
	}
}

void UPPElectricDischargeComponent::Discharge(FVector StartLocation, float ChargeTime)
{
	AActor* Owner = GetOwner();

	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(ElectricDischarge), false, Owner);

	if (DischargeMode == EDischargeMode::Capsule)
	{
		FHitResult OutHitResult;

		float DefaultEndRange = 300.0f;

		FVector Start = StartLocation;
		FVector End = Start + Owner->GetActorForwardVector() * (DefaultEndRange + ChargeTime * 50.0f);
		float SphereRadius = 50.0f;

		bool bIsHit = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2 /*ÀÓ½Ã·Î ±×·¦ ³Ö¾îµÒ*/,
			FCollisionShape::MakeSphere(SphereRadius), CollisionParam);

		if (bIsHit)
		{
			IPPElectricObjectInterface* HitElectricObject = CastChecked<IPPElectricObjectInterface>(OutHitResult.GetActor());
			if (HitElectricObject)
			{
				HitElectricObject->Charge();
			}
		}
	}
	else if (DischargeMode == EDischargeMode::Sphere)
	{
		TArray<FOverlapResult> OutOverlapResults;

		float DefaultSphereRadius = 300.0f;
		float SphereRadius = DefaultSphereRadius + ChargeTime * 50.0f;

		bool bIsHit = GetWorld()->OverlapMultiByChannel(OutOverlapResults, Owner->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel2 /*ÀÓ½Ã·Î ±×·¦ ³Ö¾îµÒ*/,
			FCollisionShape::MakeSphere(ChargeTime), CollisionParam);

		if (bIsHit)
		{
			for (const FOverlapResult OverlapResult : OutOverlapResults)
			{
				IPPElectricObjectInterface* HitElectricObject = CastChecked<IPPElectricObjectInterface>(OverlapResult.GetActor());
				if (HitElectricObject)
				{
					HitElectricObject->Charge();
				}
			}
		}
	}

	CurrentChargingTime = 0.0f;
}

void UPPElectricDischargeComponent::ChangeDischargeMode()
{
	if (DischargeMode == EDischargeMode::Capsule)
	{
		DischargeMode = EDischargeMode::Sphere;
	}
	else if(DischargeMode == EDischargeMode::Sphere)
	{
		DischargeMode = EDischargeMode::Capsule;
	}
}

