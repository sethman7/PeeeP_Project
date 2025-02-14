// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PPElectricDischargeComponent.h"
#include "CollisionQueryParams.h"
#include "Interface/PPElectricObjectInterface.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "UI/PPChargingLevelHUD.h"

// Sets default values for this component's properties
UPPElectricDischargeComponent::UPPElectricDischargeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DischargeMode = EDischargeMode::Sphere;
	MaxChargingTime = 3.0f;
	CurrentChargingTime = 0.0f;
	CurrentChargingAmount = 0.0f;
	RechargingDelay = 1.0f;
	MoveSpeedReductionRate = 0.5f;
	CurrentChargeLevel = 0;
	MaxChargeLevel = 3;
	ThresholdChargeLevel = 1;
	RequireCapacityForNextLevel = 1.0f;

	bChargingEnable = false;

	bChargeStart = false;

	// ������Ʈ(�÷��̾�)�� ���ⷮ �ʱ�ȭ
	CurrentElectricCapacity = 0.1f;
	MaxElectricCapacity = 12.0f;
	bElectricIsEmpty = false;

	DischargeEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	ElectricSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	TempChargeLevel = 0;
}


// Called when the game starts
void UPPElectricDischargeComponent::BeginPlay()
{
	Super::BeginPlay();

	APPCharacterPlayer* OwnerCharacter = Cast<APPCharacterPlayer>(GetOwner());
	if (OwnerCharacter)
	{
		DischargeEffectComponent = OwnerCharacter->GetElectricNiagaraComponent();
	}

	BroadCastToUI();
}


void UPPElectricDischargeComponent::PlayChargeLevelSound()
{
	if (CurrentChargeLevel == 0)
	{
		return;
	}

	if (TempChargeLevel != CurrentChargeLevel)
	{
		if (nullptr != ChargeLevelSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ChargeLevelSound, 0.5f * (float)CurrentChargeLevel, 0.3f * (float)CurrentChargeLevel);
		}
		TempChargeLevel = CurrentChargeLevel;
	}
}

// Called every frame
void UPPElectricDischargeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentElectricCapacity <= 0.0f)
	{
		bElectricIsEmpty = true;
	}

	if (bElectricIsEmpty)
	{
		APPCharacterPlayer* OwnerCharacter = Cast<APPCharacterPlayer>(GetOwner());
		if (IsValid(OwnerCharacter))
		{
			OwnerCharacter->DeadEventDelegate.Execute(true);
			//Reset();
		}
	}
}

void UPPElectricDischargeComponent::Charging()
{
	// ���� ������ ���ⷮ�� 0.0 ������ ���
	// ��¡ �� �ӵ� ���� ���� üũ�Ͽ� ���� ���ⷮ�� 0�� ��� �ӵ� ���Ұ� �ȵǵ��� ���� �˻�
	if (!bChargingEnable)
	{
		UE_LOG(LogTemp, Log, TEXT("Not Enough Electric."));
		
		// 1.0�� �� �ڵ����� Discharge
		if ((!AutoDischargeTimeHandler.IsValid()) && bChargeStart)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoDischargeTimeHandler, this, &UPPElectricDischargeComponent::Discharge, 1.0f, false);
			UE_LOG(LogTemp, Warning, TEXT("!bChargingEnable Timer"));
		}
		return;
	}

	if (!bChargeStart)
	{
		APPCharacterPlayer* OwnerCharacter = Cast<APPCharacterPlayer>(GetOwner());
		if (OwnerCharacter)
		{
			OwnerCharacter->ReduationMaxWalkSpeedRatio(MoveSpeedReductionRate);
			// Set Visible Level Gauge
			OwnerCharacter->GetElectricChargingLevelWidget()->SetVisibility(ESlateVisibility::Visible);
		}

		bChargeStart = true;


		// Play Charge Sound Here
		if (ChargeSound != nullptr)
		{
			ElectricSoundComponent->Sound = ChargeSound;
			ElectricSoundComponent->VolumeMultiplier = 0.5f;
			ElectricSoundComponent->PitchMultiplier = 1.0f;
			ElectricSoundComponent->Play();
		}
	}

	if (CurrentChargeLevel >= MaxChargeLevel)
	{
		// 1.0�� �� �ڵ����� DisCharge
		if (!AutoDischargeTimeHandler.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(AutoDischargeTimeHandler, this, &UPPElectricDischargeComponent::Discharge, 1.0f, false);
			UE_LOG(LogTemp, Warning, TEXT("MaxTimer"));
		}
		return;
	}

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	CurrentChargingTime += DeltaTime;
	CurrentElectricCapacity -= DeltaTime;

	CurrentChargingTime = FMath::Clamp(CurrentChargingTime, 0, MaxChargingTime);
	CurrentElectricCapacity = FMath::Clamp(CurrentElectricCapacity, 0, MaxElectricCapacity);

	// UI and SFX
	BroadCastToUI();

	int32 IntCurrentChargingTime = FMath::TruncToInt(CurrentChargingTime);

	if (CurrentChargeLevel < IntCurrentChargingTime)
	{
		// Set Current Charge Level from CurrentChargingTime
		CurrentChargeLevel = IntCurrentChargingTime;
		CurrentChargingTime = FMath::TruncToInt(CurrentChargingTime);
	}

	PlayChargeLevelSound();

	SetChargingEnable();
	
	if (ChargingEffect != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ChargingEffect Is Valid"));

		if (DischargeEffectComponent == nullptr)
		{
			return;
		}

		if (DischargeEffectComponent->GetAsset() != ChargingEffect)
		{
			DischargeEffectComponent->Deactivate();
		}

		if (!DischargeEffectComponent->IsActive())
		{
			if (DischargeEffectComponent->GetAsset() != ChargingEffect)
			{
				DischargeEffectComponent->SetAsset(ChargingEffect);
				DischargeEffectComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
			}
			DischargeEffectComponent->Activate(true);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Charging Time: %f"), CurrentChargingTime);
	UE_LOG(LogTemp, Log, TEXT("Electric Capacity: %f / %f"), CurrentElectricCapacity, MaxElectricCapacity);
}

void UPPElectricDischargeComponent::Discharge()
{
	APPCharacterPlayer* OwnerCharacter = Cast<APPCharacterPlayer>(GetOwner());

	bChargeStart = false;
	TempChargeLevel = 0;

	// Play Discharge Sound Here
	if (ElectricSoundComponent->IsPlaying())
	{
		ElectricSoundComponent->Stop();
	}

	if (CurrentChargeLevel == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentChargeLevel 0"));
		CurrentChargingTime = 0.0f;
		DischargeEffectComponent->Deactivate();
		OwnerCharacter->RevertMaxWalkSpeed();
		OwnerCharacter->GetElectricChargingLevelWidget()->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (bElectricIsEmpty)
	{
		UE_LOG(LogTemp, Warning, TEXT("bElectricIsEmpty true"));
		return;
	}

	if (nullptr != DischargeSound)
	{
		ElectricSoundComponent->Sound = DischargeSound;
		ElectricSoundComponent->PitchMultiplier = 0.33f * CurrentChargeLevel;
		ElectricSoundComponent->VolumeMultiplier = 0.5f * CurrentChargeLevel;
		ElectricSoundComponent->Play();
	}

	AActor* Owner = GetOwner();

	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(ElectricDischarge), false, Owner);

	if (DischargeMode == EDischargeMode::Capsule)
	{
		FHitResult OutHitResult;

		float DefaultEndRange = 15.0f;
		float FinalEndRange = DefaultEndRange + CurrentChargingTime * 55.0f;

		float CapsuleRadius = 10.0f;

		FVector Start = Owner->GetActorLocation() + Owner->GetActorForwardVector() * CapsuleRadius;
		FVector End = Start + Owner->GetActorForwardVector() * FinalEndRange;

		bool bIsHit = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel5,
			FCollisionShape::MakeCapsule(CapsuleRadius, FinalEndRange * 0.5f), CollisionParam);

		PlayDischargeEffect(TEXT("Capsule"), CurrentChargeLevel, FVector::ForwardVector * CapsuleRadius, FRotator(0.0f,-90.0f,0.0f));

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

		float SphereRadius = CurrentChargeLevel * 60.0f;

		bool bIsHit = GetWorld()->OverlapMultiByChannel(OutOverlapResults, Owner->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel5,
			FCollisionShape::MakeSphere(SphereRadius), CollisionParam);

		PlayDischargeEffect(TEXT("Sphere"), CurrentChargeLevel, FVector::ZeroVector, FRotator::ZeroRotator);

		if (bIsHit)
		{
			for (const FOverlapResult OverlapResult : OutOverlapResults)
			{
				IPPElectricObjectInterface* HitElectricObject = CastChecked<IPPElectricObjectInterface>(OverlapResult.GetActor());
				if (HitElectricObject)
				{
					HitElectricObject->Charge();
					UE_LOG(LogTemp, Log, TEXT("IsHit : Sphere HitObjectNum: %d"), OutOverlapResults.Num());
					UE_LOG(LogTemp, Log, TEXT("ChargeLevel: %d"), CurrentChargeLevel);
				}
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Discharge Sphere %f"), CurrentChargingTime);
	}

	if (OwnerCharacter)
	{
		OwnerCharacter->RevertMaxWalkSpeed();
	}

	CurrentChargingTime = 0.0f;
	CurrentChargeLevel = 0;

	bChargingEnable = false;

	UE_LOG(LogTemp, Warning, TEXT("ClearTimer"));
	GetWorld()->GetTimerManager().ClearTimer(AutoDischargeTimeHandler);

	GetWorld()->GetTimerManager().SetTimer(RechargingDelayTimeHandler, this, &UPPElectricDischargeComponent::SetChargingEnable, RechargingDelay, false);
	GetWorld()->GetTimerManager().SetTimer(ElectricLevelHUDTimeHandler, FTimerDelegate::CreateLambda([this]() {SetElectricLevelHUDVisible(false); }), 1.0f, false);
}

void UPPElectricDischargeComponent::ChangeDischargeMode()
{
	if (DischargeMode == EDischargeMode::Capsule)
	{
		DischargeMode = EDischargeMode::Sphere;

		UE_LOG(LogTemp, Log, TEXT("Change Discharge Mode from Capsule to Sphere"));
	}
	else if (DischargeMode == EDischargeMode::Sphere)
	{
		DischargeMode = EDischargeMode::Capsule;

		UE_LOG(LogTemp, Log, TEXT("Change Discharge Mode from Sphere to Capsule"));
	}
}

void UPPElectricDischargeComponent::SetChargingEnable()
{
	if (CurrentChargeLevel != 0)
	{
		if (CurrentElectricCapacity + FMath::Fmod(CurrentChargingTime, CurrentChargeLevel) < ThresholdChargeLevel * RequireCapacityForNextLevel)
		{
			bChargingEnable = false;
			UE_LOG(LogTemp, Warning, TEXT("SetChargingEnable False"));
			return;
		}
	}
	else if(CurrentElectricCapacity + CurrentChargingTime < ThresholdChargeLevel * RequireCapacityForNextLevel)
	{
		bChargingEnable = false;
		UE_LOG(LogTemp, Warning, TEXT("SetChargingEnable False"));
		return;
	}
	
	bChargingEnable = true;
}

void UPPElectricDischargeComponent::PlayDischargeEffect(FName EffectType, int8 ChargingLevel, FVector Location, FRotator Rotation)
{
	FString EffectKey = EffectType.ToString() + FString::FromInt(ChargingLevel);

	UE_LOG(LogTemp, Log, TEXT("%s"), *EffectKey);

	UNiagaraSystem* bHasEffect = DischargeEffects.Find(*EffectKey)->Get();
	if (bHasEffect)
	{
		DischargeEffectComponent->Deactivate();
		DischargeEffectComponent->SetRelativeLocationAndRotation(Location, Rotation);
		DischargeEffectComponent->SetAsset(bHasEffect);
		DischargeEffectComponent->Activate(true);
	}

	
}
/// <summary>
/// ElectricDischargeComponent�� CurrentElectricCapacity�� ���������ִ� �Լ�
/// </summary>
/// <param name="amount">CurrentElectricCapacity�� ������</param>
void UPPElectricDischargeComponent::AddCurrentCapacity(float amount)
{
	if (bElectricIsEmpty)
	{
		bElectricIsEmpty = false;
	}

	if (CurrentElectricCapacity <= MaxElectricCapacity)
	{
		UE_LOG(LogTemp, Log, TEXT("Charing +%f"), amount);
		CurrentElectricCapacity += amount;
		CurrentElectricCapacity = FMath::Clamp(CurrentElectricCapacity, 0, MaxElectricCapacity);
		SetChargingEnable();
		// UI�� ��ε�ĳ��Ʈ
		BroadCastToUI();
	}
}

void UPPElectricDischargeComponent::SetCurrentCapacity(float Amount)
{
	CurrentElectricCapacity = Amount;
	BroadCastToUI();
}

void UPPElectricDischargeComponent::BroadCastToUI()
{
	// ���� ���� �뷮�� ������ ����� UI�� ����� �� �ְ� ��ε� ĳ��Ʈ
	float CurrentElectircCapacityRate = FMath::Clamp((CurrentElectricCapacity / MaxElectricCapacity), 0, 1);
	float CurrentElectricCapacityRatePerLevel = FMath::Clamp(CurrentChargingTime - CurrentChargeLevel, 0, 1);
	IPPElectricHUDInterface* ElectircHUDInterface = Cast<IPPElectricHUDInterface>(GetOwner());
	if (ElectircHUDInterface)
	{
		UE_LOG(LogTemp, Log, TEXT("Succeessed Cast to IPPElectricHUDInterface."));
		if (ElectircHUDInterface->ElectircCapacityDelegate.IsBound())
		{
			UE_LOG(LogTemp, Log, TEXT("Succeessed to Bound ElectircCapacityDelegate."));
			ElectircHUDInterface->ElectircCapacityDelegate.Broadcast(CurrentElectircCapacityRate);
		}
		if (ElectircHUDInterface->ChargingLevelDelegate.IsBound())	// Level Gauge
		{
			ElectircHUDInterface->ChargingLevelDelegate.Broadcast(CurrentChargeLevel + 1, CurrentElectricCapacityRatePerLevel);
		}
	}
}

void UPPElectricDischargeComponent::SetElectricLevelHUDVisible(bool bFlag)
{
	APPCharacterPlayer* OwnerCharacter = Cast<APPCharacterPlayer>(GetOwner());
	if (OwnerCharacter)
	{
		if (bFlag)
		{
			OwnerCharacter->GetElectricChargingLevelWidget()->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			OwnerCharacter->GetElectricChargingLevelWidget()->SetVisibility(ESlateVisibility::Hidden);
			for (int i = 1; i <= 3; i++)
			{
				OwnerCharacter->GetElectricChargingLevelWidget()->SetChargingCapacityAmount(i, 0.0f);
				OwnerCharacter->GetElectricChargingLevelWidget()->SetGaugeGlowEffectVisible(false);
			}
		}
	}
}

void UPPElectricDischargeComponent::Reset()
{
	CurrentChargingTime = 0.0f;
	CurrentChargeLevel = 0;

	bChargingEnable = true;
	bChargeStart = false;
	CurrentElectricCapacity = 12.0f;
	MaxElectricCapacity = 12.0f;
	bElectricIsEmpty = false;

	GetWorld()->GetTimerManager().ClearTimer(AutoDischargeTimeHandler);
	GetWorld()->GetTimerManager().ClearTimer(RechargingDelayTimeHandler);
	GetWorld()->GetTimerManager().ClearTimer(ElectricLevelHUDTimeHandler);

	BroadCastToUI();
}

