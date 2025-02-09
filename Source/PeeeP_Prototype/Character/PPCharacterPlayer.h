// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCharacterBase.h"
#include "InputActionValue.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Interface/UI/PPElectricHUDInterface.h"

#include "../Inventory/PPInventoryComponent.h"
#include "Interface/UI/PPInventoryInterface.h"
#include "Interface/PPAnimationGrabInterface.h"
#include "PPCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPCharacterPlayer : public APPCharacterBase, public IPPElectricHUDInterface, public IPPInventoryInterface, public IPPAnimationGrabInterface

{
	GENERATED_BODY()	
	
public:
	APPCharacterPlayer();
	
	void OnDeath();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
// Character Control Section
protected:
	// Character Movement Component
	// �ܺο� ���ؼ� �÷��̾ ������ ���� ���� �� �������κ��� �����Ͽ� ���
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComponent;


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UPPCharacterControlData* CharacterControlData) override;

// �Է� ����
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
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ButtonInteract;

	// Move MaxWalkSpeed Variable here from ElectricDischarge Setting Section
	// You can Setting Player's Max Walk Speed
	float MaxWalkSpeed;

	bool bIsRunning;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ButtonInteraction(const FInputActionValue& Value);
	void OnRunningStart(const FInputActionValue& Value);
	void OnRunningEnd(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

protected:

	TObjectPtr<class USkeletalMesh> DefaultSkeletonMesh;

	TSubclassOf<class UAnimInstance> DefaultAnimClass;
public:
	void SetDefaultMeshAndAnim();

protected:
	// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

public:
	UCameraComponent* GetCamera();

	//Animation

	void PlayAnimation(UAnimMontage* InAnimMontage);
protected:


//Parts
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPPPartsBase> Parts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> AttachedMesh;

	//임시
	TArray<TObjectPtr<class UPPPartsBase>> PartsArray;

public:
	void SwitchParts(class UPPPartsDataBase* InPartsData);

	void RemoveParts();

	void Test_EquipGrabParts();

	virtual void GrabHitCheck() override;

protected:
	//ElectricComponent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Electric, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ElectricDischargeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Electric, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ElectricDischargeModeChangeAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Electric)
	TObjectPtr<class UPPElectricDischargeComponent> ElectricDischargeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Electric)
	TObjectPtr<class UNiagaraComponent> ElectricNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effect)
	TObjectPtr<class UNiagaraComponent> PlayerEffectNiagaraComponent;

	float ChargeTime;

public:
	// Getter
	UPPElectricDischargeComponent* GetElectricDischargeComponent();

protected:
	//InventoryComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Electric)
	TObjectPtr<class UPPInventoryComponent> InventoryComponent;

public:
	void ReduationMaxWalkSpeedRatio(float InReductionRatio);
	void RevertMaxWalkSpeed();

	class UNiagaraComponent* GetElectricNiagaraComponent() const;
	class UNiagaraComponent* GetPlayerEffectNiagaraComponent() const;

	// Getter
	// InventoryComponent(IPPInventoryInterface에 의해)
	virtual class UPPInventoryComponent* GetInventoryComponent() override;

//protected:
//	UPROPERTY()
//	TObjectPtr<class UAudioComponent>

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> OpenMenuInteract;

	void OpenMenu();


protected:	// Quick Slot Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuickSlotMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuickSlotUseAction;

	FTimerHandle QuickSlotMoveTimer;
	bool bIsAllowWheelInput;

	void QuickSlotMove(const FInputActionValue& Value);
	void QuickSlotUse(const FInputActionValue& Value);

	void SetWheelInputAllow(bool Value);
};
