// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Intro/PPIntroWidget.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "Kismet/GameplayStatics.h"

FReply UPPIntroWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	StartGame();

	return FReply::Handled();
}

FReply UPPIntroWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	StartGame();

	return FReply::Handled();
}

void UPPIntroWidget::StartGame()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainLobby"));
}

void UPPIntroWidget::PlayIntro()
{
	if (IntroMedia)
	{
		if (IntroMedia->OpenSource(IntroMediaSource))
		{
			IntroMediaTexture->SetMediaPlayer(IntroMedia);
			IntroMediaTexture->UpdateResource();

			IntroMedia->Play();
			UE_LOG(LogTemp, Log, TEXT("Play Intro"));

			IntroMedia->OnEndReached.AddDynamic(this, &UPPIntroWidget::PlayLoop);
		}
	}
}

void UPPIntroWidget::PlayLoop()
{
	if (IntroMedia)
	{
		if (IntroMedia->OpenSource(IntroMediaLoopSource))
		{
			IntroMedia->Seek(FTimespan::FromMilliseconds(0));

			IntroMediaTexture->SetMediaPlayer(IntroMedia);
			//IntroMediaTexture->UpdateResource();

			IntroMedia->SetLooping(true);
			IntroMedia->Play();
			UE_LOG(LogTemp, Log, TEXT("Play Intro"));

		}
	}
}
