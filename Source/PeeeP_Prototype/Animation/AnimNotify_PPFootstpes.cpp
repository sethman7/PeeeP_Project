// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_PPFootstpes.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"


UAnimNotify_PPFootstpes::UAnimNotify_PPFootstpes()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> FootstepSoundRef(TEXT("/Script/Engine.SoundCue'/Game/SFX/Character/Footstep/Default/cues/SC_DefualtFootstep.SC_DefualtFootstep'"));
	if (nullptr != FootstepSoundRef.Object)
	{
		FootstepSound = FootstepSoundRef.Object;
	}

	VolumeMultiplier = 0.25f;
	PitchMultiplier = 1.0f;
}

void UAnimNotify_PPFootstpes::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//UE_LOG(LogTemp, Log, TEXT("Footstep Begin"));

	// Play Footstep Sound
	// Later, we will add a feature to play different footstep sounds based on the surface type.
	// So, I added several surface types in project settings.
	// We need to a feature that what surface type is the character stepping on.
	// Some blogs used line trace to detect the surface type.

	// Test Sound(Default Footsteps)
	/*
	if (nullptr != FootstepSound)
	{
		UGameplayStatics::PlaySoundAtLocation(MeshComp, FootstepSound, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
	}
	*/

	// Write Code Here
	AActor* Owner = MeshComp->GetOwner();
	if (nullptr == Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is nullptr."));
		return;
	}

	FVector StartPos = Owner->GetActorLocation();
	FVector EndPos = Owner->GetActorUpVector() * -150.0f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(Visibility), false, Owner);

	bool IsHit = Owner->GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility, CollisionParam, FCollisionResponseParams(ECR_Block));
	if (IsHit)
	{
		// I want to get the hitresult's actor.
		AActor* HitActor = HitResult.GetActor();
		if (nullptr != HitActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor"));
			auto Surface = HitResult.PhysMaterial.Get()->SurfaceType;
			// 여기서 추가로 작업 필요
			// 표면의 타입에 따라 다른 사운드를 재생하도록 구현해야 함.

		}
		
	}
	/*
	FVector CameraPos = FollowCamera->GetComponentLocation();
	FVector CameraForwardVector = FollowCamera->GetForwardVector();
	FVector EndPos = CameraPos + CameraForwardVector * 600.f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(Button), false, this);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraPos, EndPos, ECC_GameTraceChannel1, CollisionParam, FCollisionResponseParams(ECR_Block));

	if (IsHit)
	{
		AActor* HitActor = HitResult.GetActor();
		IPPInteractableObjectInterface* ButtonActor = Cast<IPPInteractableObjectInterface>(HitActor);
		//ensure(ButtonActor);
		if (ButtonActor != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("FindButton"));
			ButtonActor->Interact();
		}

	}

	FColor DebugColor(255, 0, 0);

	DrawDebugLine(GetWorld(), CameraPos, EndPos, DebugColor, false, 5.0f);
	*/
}
