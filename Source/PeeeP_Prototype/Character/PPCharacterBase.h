// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class PEEEP_PROTOTYPE_API APPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPCharacterBase();

protected:
	virtual void SetCharacterControlData(const class UPPCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	// TMap -> python에 있는 Dictionary랑 비슷
	// TMap<[Key], [Value]>
	TMap<ECharacterControlType, class UPPCharacterControlData*> CharacterControlManager;
};
