// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPPAnimInstance::UPPAnimInstance()
{
	MovingThreshould = 3.f;
	JunpingThreshould = 100.f;

	bIsDead = false;
}

void UPPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		
		Movement = Owner->GetCharacterMovement();
	}
}

void UPPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = (Velocity.Z > JunpingThreshould) & bIsFalling;
		bIsRunning = GroundSpeed > 100.0f;
	}

}

void UPPAnimInstance::SetbIsDead(uint8 IsDead)
{
	bIsDead = IsDead;
}
