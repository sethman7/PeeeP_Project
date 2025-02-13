// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPChargingLevelHUD.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Interface/UI/PPElectricHUDInterface.h"

void UPPChargingLevelHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Get Palette Elements
	// Progress Bar
	Level1Bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Level01_Bar")));
	Level2Bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Level02_Bar")));
	Level3Bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Level03_Bar")));

	Level1Bar->SetPercent(0.0f);
	Level2Bar->SetPercent(0.0f);
	Level3Bar->SetPercent(0.0f);

	// Glow Effect Images
	Level1Glow = Cast<UImage>(GetWidgetFromName(TEXT("Level01_Glow")));
	Level2Glow = Cast<UImage>(GetWidgetFromName(TEXT("Level02_Glow")));
	Level3Glow = Cast<UImage>(GetWidgetFromName(TEXT("Level03_Glow")));

	Level1Glow->SetVisibility(ESlateVisibility::Hidden);
	Level2Glow->SetVisibility(ESlateVisibility::Hidden);
	Level3Glow->SetVisibility(ESlateVisibility::Hidden);

	// Bind ¿µ¿ª
	IPPElectricHUDInterface* ElectricHUDInterface = Cast<IPPElectricHUDInterface>(GetOwningPlayerPawn());
	if (ElectricHUDInterface)
	{
		ElectricHUDInterface->ChargingLevelDelegate.AddUObject(this, &UPPChargingLevelHUD::SetChargingCapacityAmount);
	}

	this->SetVisibility(ESlateVisibility::Hidden);
}

void UPPChargingLevelHUD::SetChargingCapacityAmount(int LevelOfBar, float Rate)
{
	switch (LevelOfBar)
	{
	case 1:
		if (IsValid(Level1Bar))
		{
			Level1Bar->SetPercent(Rate);
			if (Level1Bar->Percent >= 1.0f)
			{
				Level1Glow->SetVisibility(ESlateVisibility::Visible);
			}
		}
		break;
	case 2:
		if (IsValid(Level2Bar))
		{
			Level2Bar->SetPercent(Rate);
			Level2Bar->SetPercent(Rate);
			if (Level2Bar->Percent >= 1.0f)
			{
				Level2Glow->SetVisibility(ESlateVisibility::Visible);
			}
		}
		break;
	case 3:
		if (IsValid(Level3Bar))
		{
			Level3Bar->SetPercent(Rate);
			Level3Bar->SetPercent(Rate);
			if (Level3Bar->Percent >= 1.0f)
			{
				Level3Glow->SetVisibility(ESlateVisibility::Visible);
			}
		}
		break;
	default:
		break;
	}
}

void UPPChargingLevelHUD::SetGaugeGlowEffectVisible(bool bFlag)
{
	if (bFlag)
	{
		Level1Glow->SetVisibility(ESlateVisibility::Visible);
		Level2Glow->SetVisibility(ESlateVisibility::Visible);
		Level3Glow->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Level1Glow->SetVisibility(ESlateVisibility::Hidden);
		Level2Glow->SetVisibility(ESlateVisibility::Hidden);
		Level3Glow->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPPChargingLevelHUD::SetWidgetVisible(bool Flag)
{
	if (Flag)
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
}
