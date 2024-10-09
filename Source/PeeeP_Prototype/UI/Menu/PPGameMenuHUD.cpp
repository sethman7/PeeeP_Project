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

	/*StartButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_StartButton")));
	LoadButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_LoadButton")));
	SettingButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_SettingButton")));
	ExitButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_ExitButton")));*/

	LobbyActor = Cast<APPLobbyActor>(UGameplayStatics::GetActorOfClass(GetWorld(), LobbyActorClass));

	if (WBP_StartButton && WBP_StartButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Start"));
		WBP_StartButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::StartButtonClick);
		WBP_StartButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::StartButtonHover);
	}

	if (WBP_LoadButton && WBP_LoadButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Load"));
		WBP_LoadButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::LoadButtonClick);
		WBP_LoadButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::LoadButtonHover);
	}

	if (WBP_SettingButton && WBP_SettingButton->Button)
	{
		WBP_SettingButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::SettingButtonClick);
		WBP_SettingButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::SettingButtonHover);
	}

	if (WBP_ExitButton && WBP_ExitButton->Button)
	{
		WBP_ExitButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::ExitButtonClick);
		WBP_ExitButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::ExitButtonHover);
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
