// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	APPPlayerController();

protected:
	virtual void BeginPlay() override;

	// 위젯 클래스를 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UPPInGameUIMain> InGameUIMainClass;
	// 실제 생성된 위젯을 가리키는 포인터
	UPROPERTY()
	TObjectPtr<class UPPInGameUIMain> InGameUIMain;

public:

};
