// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PPMenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

UPPMenuButtonWidget::UPPMenuButtonWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void UPPMenuButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button")));

	ButtonImage = Cast<UImage>(GetWidgetFromName(TEXT("ButtonImage")));

	//Button->OnHovered.AddDynamic(this, &UPPMenuButtonWidget::HoveredEvent);
}

//void UPPMenuButtonWidget::HoveredEvent()
//{
//	if (ButtonImage)
//	{
//		ButtonImage->SetBrushSize(FVector2D(700.f, 700.f));
//	}
//}

UButton* UPPMenuButtonWidget::GetButton()
{
	return Button.Get();
}
