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
	KeyManualImage_Discharge = Cast<UImage>(GetWidgetFromName(TEXT("KeyManualImage_Discharge")));		// 키 가이드 이미지 - Discharge
	KeyManualImage_Equip = Cast<UImage>(GetWidgetFromName(TEXT("KeyManualImage_Equip")));				// 키 가이드 이미지 - Equip
	KeyManualImage_Unequip = Cast<UImage>(GetWidgetFromName(TEXT("KeyManualImage_Unequip")));			// 키 가이드 이미지 - Unequip
	KeyManualImage_Grab01 = Cast<UImage>(GetWidgetFromName(TEXT("KeyManualImage_Grab01")));				// 키 가이드 이미지 - Grab Parts(Grab)
	KeyManualImage_Grab02 = Cast<UImage>(GetWidgetFromName(TEXT("KeyManualImage_Grab02")));				// 키 가이드 이미지 - Grab Parts(Release)


	KeyManualImage_WASD->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Jump->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Charge->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Discharge->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Equip->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Unequip->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Grab01->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Grab02->SetVisibility(ESlateVisibility::Hidden);

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
	KeyManualImage_Discharge->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Equip->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Unequip->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Grab01->SetVisibility(ESlateVisibility::Hidden);
	KeyManualImage_Grab02->SetVisibility(ESlateVisibility::Hidden);
}

/// <summary>
/// 키 가이드 UI 표시, 입력한 index에 해당하는 하나의 UI만 표시
/// ///
/// 0: 모두 비활성화
/// 1: 기본 조작, 2: 점프, 3: 전기 충전, 4: 전기 방출
/// 5: 파츠 장착, 6: 파츠 해제, 7: 그랩 파츠(그랩), 8: 그랩 파츠(놓기)
/// </summary>
/// <param name="index">표시할 UI</param>
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
	case 4:
	{
		KeyManualImage_Discharge->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	case 5:
	{
		KeyManualImage_Equip->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	case 6:
	{
		KeyManualImage_Unequip->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	case 7:
	{
		KeyManualImage_Grab01->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	case 8:
	{
		KeyManualImage_Grab02->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	default:
		break;
	}
}
