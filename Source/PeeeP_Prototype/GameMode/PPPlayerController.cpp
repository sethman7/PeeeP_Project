// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/PPInGameUIMain.h"

APPPlayerController::APPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPPInGameUIMain> InGameUIClassRef(TEXT("/Game/UI/WBP_InGameMain.WBP_InGameMain_C"));
	if (InGameUIClassRef.Class)
	{
		InGameUIMainClass = InGameUIClassRef.Class;
	}
}

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	//SetInputMode(FInputModeGameAndUI());

	if (IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("LocalController"));
		if (InGameUIMainClass)
		{
			UE_LOG(LogTemp, Log, TEXT("InGameUIClass"));
			InGameUIMain = CreateWidget<UPPInGameUIMain>(this, InGameUIMainClass);
			if (InGameUIMain)
			{
				InGameUIMain->AddToViewport();	// 화면에 표시
			}
		}
	}
}
