// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPKeyManualUI.h"
#include "Components/Image.h"
#include "Interface/UI/PPElectricHUDInterface.h"

void UPPKeyManualUI::NativeConstruct()
{
	Super::NativeConstruct();

	KeyManualImage_WASD = Cast<UImage>(GetWidgetFromName(TEXT("KeyManualImage_WASD")));					// 키 가이드 이미지 - WASD
	KeyManualImage_Jump = Cast<UImage>(GetWidgetFromName(TEXT("KeyManualImage_Jump")));					// 키 가이드 이미지 - Jump
	KeyManualImage_Charge = Cast<UImage>(GetWidgetFromName(TEXT("KeyManualImage_Charge")));				// 키 가이드 이미지 - Charge


	KeyManualImage_WASD->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Jump->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Charge->SetVisibility(ESlateVisibility::Hidden);

	// 델리게이트를 위한 인터페이스(플레이어에게 상속됨)
	IPPElectricHUDInterface* ElectricHUDInterface = Cast<IPPElectricHUDInterface>(GetOwningPlayerPawn());
	if (ElectricHUDInterface)
	{
		ElectricHUDInterface->ShowKeyManualDelegate.AddUObject(this, &UPPKeyManualUI::SetKeyManualImageVisible);
	}

}

void UPPKeyManualUI::SetAllKeyManualImageHidden()
{
	KeyManualImage_WASD->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Jump->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Charge->SetVisibility(ESlateVisibility::Hidden);
}

void UPPKeyManualUI::SetKeyManualImageVisible(uint32 index)
{
	SetAllKeyManualImageHidden();
	switch (index)
	{
	case 0:
		break;
	case 1:
	{
		KeyManualImage_WASD->SetVisibility(ESlateVisibility::Visible);
	}
		break;
	case 2:
	{
		KeyManualImage_Jump->SetVisibility(ESlateVisibility::Visible);
	}
		break;
	case 3:
	{
		KeyManualImage_Charge->SetVisibility(ESlateVisibility::Visible);
	}
		break;
	default:
		break;
	}
}
