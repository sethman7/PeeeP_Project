// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_PPRun.h"
#include "Character/PPCharacterPlayer.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

UAnimNotify_PPRun::UAnimNotify_PPRun()
{

}

void UAnimNotify_PPRun::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APPCharacterPlayer* CharacterPlayer = Cast<APPCharacterPlayer>(MeshComp->GetOwner());
	if (nullptr == CharacterPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterPlayer was nullptr."));
		return;
	}

	UNiagaraComponent* PlayerNiagaraComponent = CharacterPlayer->GetPlayerEffectNiagaraComponent();
	if (nullptr == PlayerNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerNiagaraComponent was nullptr."));
		return;
	}

	if (PlayerNiagaraComponent->GetAsset() != RunningEffect)
	{
		PlayerNiagaraComponent->Deactivate();
	}

	if (PlayerNiagaraComponent->GetAsset() != RunningEffect)
	{
		PlayerNiagaraComponent->SetAsset(RunningEffect);
		PlayerNiagaraComponent->SetRelativeLocationAndRotation(FVector::DownVector * EffectPlayPosition, FRotator::ZeroRotator);
	}
	PlayerNiagaraComponent->SetRelativeLocationAndRotation(FVector::DownVector * EffectPlayPosition, FRotator::ZeroRotator);
	PlayerNiagaraComponent->Activate(true);
}
