// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_PPPunchJab.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/PPCharacterPlayer.h"

void UAnimNotifyState_PPPunchJab::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
}

void UAnimNotifyState_PPPunchJab::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	FVector StartPos = MeshComp->GetSocketLocation(TEXT("PunchRangeStart"));
	
	FVector EndPos = MeshComp->GetSocketLocation(TEXT("PunchRangeEnd"));

	UWorld* World = MeshComp->GetOwner()->GetWorld();

	DrawDebugPoint(World, StartPos, 5.0f, FColor::Green, false, 2.0f);
	DrawDebugPoint(World, EndPos, 5.0f, FColor::Green, false, 2.0f);

	FVector HitBoxHalfExtent = FVector(50.0f, 60.0f, 75.0f);

	FCollisionQueryParams CollisionParam(TEXT("Punch"), false, MeshComp->GetOwner());

	TArray<FHitResult> HitResults;
	bool bIsHit = World->SweepMultiByChannel(HitResults, StartPos, EndPos, FQuat::Identity, ECC_GameTraceChannel6, FCollisionShape::MakeBox(HitBoxHalfExtent), CollisionParam);
	if (bIsHit)
	{
		AActor* Owner = MeshComp->GetOwner();

		float magnitude = (EndPos - StartPos).Length();
		FVector Up = FVector::UpVector * magnitude;
		FVector Direction = (EndPos - StartPos + Up).GetSafeNormal();
		FVector Velocity = Direction * 500.0f;

		for (auto HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();

			ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
			if (HitCharacter != nullptr)
			{
				HitCharacter->LaunchCharacter(Velocity, true, true);
				APPCharacterPlayer* CharacterPlayer = Cast<APPCharacterPlayer>(HitCharacter);
				if (nullptr != CharacterPlayer)
				{
					CharacterPlayer->TakeDamage(10.0f * FrameDeltaTime);
				}
				UE_LOG(LogTemp, Warning, TEXT("Hit"));
			}
			else
			{
				UPrimitiveComponent* HitComp = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
				HitComp->AddImpulse(Velocity, NAME_None, true);
			}
		}
	}

	FColor HitBoxColor = bIsHit ? FColor::Red : FColor::Green;
	FVector Center = StartPos + ((EndPos - StartPos) / 2.0f);
	DrawDebugBox(World, Center, HitBoxHalfExtent, HitBoxColor, false, 5.0f);

	return;
}

void UAnimNotifyState_PPPunchJab::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
