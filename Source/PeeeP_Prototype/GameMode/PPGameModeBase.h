// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APPGameModeBase();

	/*UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);*/

	void MoveCharacterToSpawnLocation(class APPCharacterPlayer* Character);

	void SetInitialSpawnLocation(APlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;*/
};
