// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPCustomWidget.generated.h"

/**
 * 플레이어를 저장하기 위한 변수와 함수만을 가진 커스텀 위젯. 플레이어에 접근할 목적으로 제작
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPCustomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }

protected:
	// 현재 위젯을 소유하고 있는 액터 저장용 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;
};
