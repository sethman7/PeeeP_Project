// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacterPlayer.h"
<<<<<<< Updated upstream
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APPCharacterPlayer::APPCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// CameraBoom ÄÄÆ÷³ÍÆ®¸¦ °¡Á®¿È
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));	// FollowCamera ÄÄÆ÷³ÍÆ®¸¦ °¡Á®¿È
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

=======
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APPCharacterPlayer::APPCharacterPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamearaBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
>>>>>>> Stashed changes
}
