// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPauseMenyHUD.h"
#include "UI/Menu/PPMenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameMode/PPPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"


void UPPPauseMenyHUD::NativeConstruct()
{
	Super::NativeConstruct();

	ContinueButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_ResomeButton")));
	SettingButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_PauseSettingButton")));
	ExitButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_TitleButton")));

	if (ContinueButton && ContinueButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Start"));
		ContinueButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::ContinueButtonClick);
	}

	if (SettingButton && SettingButton->Button)
	{
		SettingButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::SettingButtonClick);
	}

	if (ExitButton && ExitButton->Button)
	{
		ExitButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::ExitButtonClick);
	}
}

FReply UPPPauseMenyHUD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey.IsValid())
	{
		return FReply::Handled();
	}

	if (PressedKey == EKeys::Escape)
	{
		ContinueButtonClick();
	}

	return FReply::Handled();
}

void UPPPauseMenyHUD::ContinueButtonClick()
{
	APPPlayerController* PlayerController = Cast<APPPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->CloseMenu();
	}
}

void UPPPauseMenyHUD::SettingButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("Setting"));
}

void UPPPauseMenyHUD::ExitButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainLobby"));
}