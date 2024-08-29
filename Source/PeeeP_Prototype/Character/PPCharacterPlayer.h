// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCharacterBase.h"
#include "InputActionValue.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PPCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPCharacterPlayer : public APPCharacterBase
{
	GENERATED_BODY()	
	
public:
	APPCharacterPlayer();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Character Control Section
protected:
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UPPCharacterControlData* CharacterControlData) override;

// 입력 셋팅
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ButtonInteract;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ButtonInteraction(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

protected:
	// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

public:
	UCameraComponent* GetCamera();


protected:
//Parts
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPPPartsBase> Parts;

public:
	void SwitchParts(class UPPPartsDataBase* InPartsData);


	
protected:
//ElectricComponent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Electric, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ElectricDischargeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Electric, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ElectricDischargeModeChangeAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Electric)
	TObjectPtr<class UPPElectricDischargeComponent> ElectricDischargeComponent;

	float ChargeTime;
	float MaxWalkSpeed;

public:
	void ReduationMaxWalkSpeedRatio(float InReductionRatio);
	void RevertMaxWalkSpeed();
};
