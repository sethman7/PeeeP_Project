// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PPGameMenuHUD.h"
#include "UI/Menu/PPMenuButtonWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PPLobbyActor.h"

UPPGameMenuHUD::UPPGameMenuHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LobbyActorClass = APPLobbyActor::StaticClass();
}

void UPPGameMenuHUD::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_StartButton")));
	LoadButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_LoadButton")));
	SettingButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_SettingButton")));
	ExitButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_ExitButton")));

	LobbyActor = Cast<APPLobbyActor>(UGameplayStatics::GetActorOfClass(GetWorld(), LobbyActorClass));

	if (StartButton && StartButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Start"));
		StartButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::StartButtonClick);
		StartButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::StartButtonHover);
	}

	if (LoadButton && LoadButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Load"));
		LoadButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::LoadButtonClick);
		LoadButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::LoadButtonHover);
	}

	if (SettingButton && SettingButton->Button)
	{
		SettingButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::SettingButtonClick);
		SettingButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::SettingButtonHover);
	}

	if (ExitButton && ExitButton->Button)
	{
		ExitButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::ExitButtonClick);
		ExitButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::ExitButtonHover);
	}
}

void UPPGameMenuHUD::StartButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("StartGame"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage1"));
}

void UPPGameMenuHUD::LoadButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("LoadOpen"));
}

void UPPGameMenuHUD::SettingButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("SettingOpen"));
}

void UPPGameMenuHUD::ExitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UPPGameMenuHUD::StartButtonHover()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(2);
}

void UPPGameMenuHUD::LoadButtonHover()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(1);
}

void UPPGameMenuHUD::SettingButtonHover()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(0);
}

void UPPGameMenuHUD::ExitButtonHover()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(3);
}
