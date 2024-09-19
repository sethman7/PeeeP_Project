// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPauseMenyHUD.h"
#include "UI/Menu/PPMenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameMode/PPPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"
#include "PPLobbyActor.h"


UPPPauseMenyHUD::UPPPauseMenyHUD(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	LobbyActorClass = APPLobbyActor::StaticClass();
}

void UPPPauseMenyHUD::NativeConstruct()
{
	Super::NativeConstruct();

	ContinueButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_ResomeButton")));
	SettingButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_PauseSettingButton")));
	ExitButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_TitleButton")));

	LobbyActor = Cast<APPLobbyActor>(UGameplayStatics::GetActorOfClass(GetWorld(), LobbyActorClass));

	if (ContinueButton && ContinueButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Start"));
		ContinueButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::ContinueButtonClick);
		ContinueButton->Button->OnHovered.AddDynamic(this, &UPPPauseMenyHUD::ContinueButtonHovered);
	}

	if (SettingButton && SettingButton->Button)
	{
		SettingButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::SettingButtonClick);
		SettingButton->Button->OnHovered.AddDynamic(this, &UPPPauseMenyHUD::SettingButtonHovered);
	}

	if (ExitButton && ExitButton->Button)
	{
		ExitButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::ExitButtonClick);
		ExitButton->Button->OnHovered.AddDynamic(this, &UPPPauseMenyHUD::ExitButtonHovered);
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

void UPPPauseMenyHUD::ContinueButtonHovered()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(2);
}

void UPPPauseMenyHUD::SettingButtonHovered()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(0);
}

void UPPPauseMenyHUD::ExitButtonHovered()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(3);
}
