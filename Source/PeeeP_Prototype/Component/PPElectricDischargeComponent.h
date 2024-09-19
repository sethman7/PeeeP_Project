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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxChargingTime;

	FTimerHandle AutoDischargeTimeHandler;
	FTimerHandle RechargingDelayTimeHandler;

	float RechargingDelay;
	float MoveSpeedReductionRate;

	bool bRechargingEnable;
	bool bChargeStart;

	// 컴포넌트(플레이어) 배터리 양
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentElectricCapacity;	// 컴포넌트(플레이어)가 현재 보유중인 전기량
	float MaxElectricCapacity;		// 컴포넌트(플레이어)가 최대 보유할 수 있는 전기량

	bool bElectricIsEmpty;			// 보유 중인 전기량이 없을 경우

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

	// 컴포넌트(플레이어) 전기 충전 함수
	void ChargeElectric(float amount);

private:
	void BroadCastToUI();
};
