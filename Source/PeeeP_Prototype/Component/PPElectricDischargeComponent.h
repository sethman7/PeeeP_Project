// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "PPElectricDischargeComponent.generated.h"

UENUM()
enum class EDischargeMode : uint8
{
	Sphere = 0,
	Capsule
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEEEP_PROTOTYPE_API UPPElectricDischargeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPPElectricDischargeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	EDischargeMode DischargeMode;

	float CurrentChargingTime;
	// Current Charging Amount(by Time)
	float CurrentChargingAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxChargingTime;

	FTimerHandle AutoDischargeTimeHandler;
	FTimerHandle RechargingDelayTimeHandler;
	FTimerHandle ElectricLevelHUDTimeHandler;

	float RechargingDelay;
	float MoveSpeedReductionRate;

	bool bChargingEnable;
	bool bChargeStart;

	// ������Ʈ(�÷��̾�) ���͸� ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentElectricCapacity;	// ������Ʈ(�÷��̾�)�� ���� �������� ���ⷮ
	float MaxElectricCapacity;		// ������Ʈ(�÷��̾�)�� �ִ� ������ �� �ִ� ���ⷮ

	bool bElectricIsEmpty;			// ���� ���� ���ⷮ�� ���� ���

	int8 CurrentChargeLevel;
	int8 MaxChargeLevel;
	int8 ThresholdChargeLevel;
	float RequireCapacityForNextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Electric)
	TObjectPtr<class UNiagaraSystem> ChargingEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Electric)
	TObjectPtr<class UNiagaraComponent> DischargeEffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Electric)
	TMap<FName, TObjectPtr<class UNiagaraSystem>> DischargeEffects;

	UPROPERTY(EditAnywhere, Category = Electric)
	TObjectPtr<class UAudioComponent> ElectricSoundComponent;

	UPROPERTY(EditAnywhere, Category = Electric)
	TObjectPtr<class USoundBase> ChargeSound;

	UPROPERTY(EditAnywhere, Category = Electric)
	TObjectPtr<class USoundBase> DischargeSound;

	UPROPERTY(EditAnywhere, Category = Electric)
	TObjectPtr<class USoundBase> ChargeLevelSound;

	int TempChargeLevel;
	void PlayChargeLevelSound();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UFUNCTION()
	void Charging();

	//UFUNCTION()
	void Discharge();

	UFUNCTION()
	void ChangeDischargeMode();
	void SetChargingEnable();

	void PlayDischargeEffect(FName EffectType, int8 ChargingLevel, FVector Location, FRotator Rotation);

	// ������Ʈ(�÷��̾�) ���� ���� �Լ�
	void AddCurrentCapacity(float amount);
	void SetCurrentCapacity(float Amount);
	void Reset();

public:
	void BroadCastToUI();

protected:
	void SetElectricLevelHUDVisible(bool bFlag);
};
