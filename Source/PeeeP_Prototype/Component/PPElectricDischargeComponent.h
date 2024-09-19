// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxChargingTime;

	FTimerHandle AutoDischargeTimeHandler;
	FTimerHandle RechargingDelayTimeHandler;

	float RechargingDelay;
	float MoveSpeedReductionRate;

	bool bRechargingEnable;
	bool bChargeStart;

	int8 CurrentChargeLevel;
	int8 MaxChargeLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Electric)
	TObjectPtr<class UNiagaraSystem> ChargingEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Electric)
	TObjectPtr<class UNiagaraComponent> DischargeEffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Electric)
	TMap<FName, TObjectPtr<class UNiagaraSystem>> DischargeEffects;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UFUNCTION()
	void Charging();

	//UFUNCTION()
	void Discharge();

	UFUNCTION()
	void ChangeDischargeMode();
	void SetbRecharging();

	void PlayDischargeEffect(FName EffectType, int8 ChargingLevel, FVector Location, FRotator Rotation);
};
