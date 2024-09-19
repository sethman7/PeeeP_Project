// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PPElectricDischargeComponent.h"
#include "CollisionQueryParams.h"
#include "Interface/PPElectricObjectInterface.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "Components/WidgetComponent.h"

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

	// 컴포넌트(플레이어)의 전기량 초기화
	CurrentElectricCapacity = 0.0f;
	MaxElectricCapacity = 3.0f;
	bElectricIsEmpty = true;


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

	// 현재 보유한 전기량이 0.0 이하일 경우
	// 차징 시 속도 감소 전에 체크하여 보유 전기량이 0일 경우 속도 감소가 안되도록 먼저 검사
	if (CurrentElectricCapacity <= 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Not Enough Electric."));

		// 1.0초 후 자동으로 Discharge
		if (!AutoDischargeTimeHandler.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(AutoDischargeTimeHandler, this, &UPPElectricDischargeComponent::Discharge, 1.0f, false);
			UE_LOG(LogTemp, Warning, TEXT("Timer"));
		}
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

	/*
	if (CurrentChargingTime >= MaxChargingTime)
	{
		// 1.0초 후 자동으로 DisCharge
		if (!AutoDischargeTimeHandler.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(AutoDischargeTimeHandler, this, &UPPElectricDischargeComponent::Discharge, 1.0f, false);
			UE_LOG(LogTemp, Warning, TEXT("Timer"));
		}
		return;
	}
	*/

	CurrentChargingTime += GetWorld()->GetDeltaSeconds();
	// 차징 중일 경우 계속해서 현재 전기 보유량을 빼줌
	CurrentElectricCapacity -= GetWorld()->GetDeltaSeconds();

	CurrentChargingTime = FMath::Clamp(CurrentChargingTime, 0, MaxChargingTime);
	CurrentElectricCapacity = FMath::Clamp(CurrentElectricCapacity, 0, MaxElectricCapacity);

	// UI에 브로드캐스트
	BroadCastToUI();

	UE_LOG(LogTemp, Log, TEXT("Charging Time: %f"), CurrentChargingTime);
	UE_LOG(LogTemp, Log, TEXT("Electric Capacity: %f / %f"), CurrentElectricCapacity, MaxElectricCapacity);
}

void UPPElectricDischargeComponent::Discharge()
{
	if (!bRechargingEnable)
	{
		return;
	}

	if (bElectricIsEmpty)
	{
		UE_LOG(LogTemp, Log, TEXT("Not Enough Electric"));
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

		bool bIsHit = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel5,

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

		bool bIsHit = GetWorld()->OverlapMultiByChannel(OutOverlapResults, Owner->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel5,
			FCollisionShape::MakeSphere(SphereRadius), CollisionParam);

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
	
	// 보유한 전기량이 0 이하일 경우
	if (CurrentElectricCapacity <= 0.0f)
	{
		bElectricIsEmpty = true;
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

/// <summary>
/// ElectricDischargeComponent의 CurrentElectricCapacity를 증가시켜주는 함수
/// </summary>
/// <param name="amount">CurrentElectricCapacity의 증가량</param>
void UPPElectricDischargeComponent::ChargeElectric(float amount)
{
	if (bElectricIsEmpty)
	{
		bElectricIsEmpty = false;
	}

	if (CurrentElectricCapacity < MaxElectricCapacity)
	{
		UE_LOG(LogTemp, Log, TEXT("Charing +%f"), amount);
		CurrentElectricCapacity += amount;
		CurrentElectricCapacity = FMath::Clamp(CurrentElectricCapacity, 0, MaxElectricCapacity);
		// UI에 브로드캐스트
		BroadCastToUI();
	}
}

void UPPElectricDischargeComponent::BroadCastToUI()
{
	// 현재 전기 용량을 비율로 만들어 UI에 적용될 수 있게 브로드 캐스트
	float CurrentElectircCapacityRate = FMath::Clamp((CurrentElectricCapacity / MaxElectricCapacity), 0, 1);
	IPPElectricHUDInterface* ElectircHUDInterface = Cast<IPPElectricHUDInterface>(GetOwner());
	if (ElectircHUDInterface)
	{
		UE_LOG(LogTemp, Log, TEXT("Succeessed Cast to IPPElectricHUDInterface."));
		if (ElectircHUDInterface->ElectircCapacityDelegate.IsBound())
		{
			UE_LOG(LogTemp, Log, TEXT("Succeessed to Bound ElectircCapacityDelegate."));
			ElectircHUDInterface->ElectircCapacityDelegate.Broadcast(CurrentElectircCapacityRate);
		}
	}
}

