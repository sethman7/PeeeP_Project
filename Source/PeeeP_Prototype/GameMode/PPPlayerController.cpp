// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"
#include "UI/Menu/PPPauseMenyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/PPInGameUIMain.h"


APPPlayerController::APPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPPPauseMenyHUD> PauseMenuRef(TEXT("/Game/UI/MenuHUD/WBP_PauseMenu.WBP_PauseMenu_C"));
	if (PauseMenuRef.Class)
	{
		PauseUIClass = PauseMenuRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UPPInGameUIMain> InGameUIClassRef(TEXT("/Game/UI/WBP_InGameMain.WBP_InGameMain_C"));
	if (InGameUIClassRef.Class)
	{
		InGameUIMainClass = InGameUIClassRef.Class;
	}
}

void APPPlayerController::OpenMenu()
{
	//SetPause(true);
	GetWorld()->GetWorldSettings()->SetTimeDilation(0.001f);

	PauseUI->SetVisibility(ESlateVisibility::Visible);
	
	FInputModeUIOnly InputUIOnly;
	SetInputMode(InputUIOnly);
	bShowMouseCursor = true;
}

void APPPlayerController::CloseMenu()
{
	PauseUI->SetVisibility(ESlateVisibility::Hidden);

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	bShowMouseCursor = false;

	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	//SetPause(false);
}

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	PauseUI = CreateWidget<UUserWidget>(this, PauseUIClass);
	UPPPauseMenyHUD* PauseMenu = Cast<UPPPauseMenyHUD>(PauseUI);
	if (PauseMenu)
	{
		PauseMenu->AddToViewport(1);
		CloseMenu();
	}
	//SetInputMode(FInputModeGameAndUI());

	if (IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("LocalController"));
		if (InGameUIMainClass)
		{
			UE_LOG(LogTemp, Log, TEXT("InGameUIClass"));
			InGameUIMain = CreateWidget<UPPInGameUIMain>(this, InGameUIMainClass);
			InGameUIMain->SetOwningActor(GetPawn());
			if (InGameUIMain)
			{
				InGameUIMain->AddToViewport();	// ȭ�鿡 ǥ��
			}
		}

	}
}
