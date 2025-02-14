// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_PPFootstpes.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"


UAnimNotify_PPFootstpes::UAnimNotify_PPFootstpes()
{
	VolumeMultiplier = 0.75f;
	PitchMultiplier = 1.00f;
}

void UAnimNotify_PPFootstpes::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
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
	FVector EndPos = StartPos + Owner->GetActorUpVector() * -15.0f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(Visibility), false, Owner);

	bool IsHit = Owner->GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_GameTraceChannel7, CollisionParam);
	if (IsHit)
	{
		AActor* HitActor = HitResult.GetActor();
		//UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());

		EPhysicalSurface SurfaceType = UGameplayStatics::GetSurfaceType(HitResult);
		// 여기서 추가로 작업 필요
		// 표면의 타입에 따라 다른 사운드를 재생하도록 구현해야 함.
		// 현재 SurfaceType을 SurfaceType_Default만 받아오는 문제가 발생함. -> 따라서 일단 표면에 상관없이 기본 발소리만 나도록 임시로 구현.
		/*
		switch (SurfaceType)
		{
		case SurfaceType_Default:
			UE_LOG(LogTemp, Log, TEXT("SurfaceType_Default"));
			break;
		case SurfaceType1:
			UE_LOG(LogTemp, Log, TEXT("SurfaceType1"));
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("SurfaceType2~"));
			break;
		}
		*/

		if (nullptr != FootstepSound)
		{
			UGameplayStatics::PlaySoundAtLocation(MeshComp, FootstepSound, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
		}
	}
	
	FColor DebugColor(255, 0, 0);
	DrawDebugLine(GetWorld(), StartPos, EndPos, DebugColor, false, 5.0f);
}
