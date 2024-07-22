// Fill out your copyright notice in the Description page of Project Settings.


#include "PartsData/PPPartsDataBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UPPPartsDataBase::AttachParts(APlayerController* PlayerController)
{
	if (UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		PlayerSubsystem->AddMappingContext(PartsMappingContext, 0);
	}
}

void UPPPartsDataBase::DettachParts(APlayerController* PlayerController)
{
	if (UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		PlayerSubsystem->RemoveMappingContext(PartsMappingContext);
	}
}
