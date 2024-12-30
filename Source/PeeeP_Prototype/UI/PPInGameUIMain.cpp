// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPInGameUIMain.h"
#include "Components/ProgressBar.h"
#include "Interface/UI/PPElectricHUDInterface.h"

/// <summary>
/// Main 캔버스인 WBP_InGameUIMain을 통해 인게임의 각종 UserWidget을 관리하는 방식으로 채택
/// 각 요소에 접근하기 위해서는 해당 요소를 갖고있는 UserWidget에 접근해 값을 관리하는 방식으로 진행
/// </summary>

void UPPInGameUIMain::NativeConstruct()
{
	Super::NativeConstruct();

	// Cast 영역
	PlayerStatusWidget = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_PlayerStatusWidget")));		// 조작법 이미지 위젯
	KeyManualWidget = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_KeyManualWidget")));			// 플레이서 상태 위젯
	QuickSlotWidget = Cast<UPPQuickSlotWidget>(GetWidgetFromName(TEXT("WBP_QuickSlot")));			// 퀵슬롯 위젯(테스트)

	// 각 요소 초기화 영역
	QuickSlotWidget->SetOwningActor(OwningActor);
	QuickSlotWidget->Init();

	// Bind 영역
}