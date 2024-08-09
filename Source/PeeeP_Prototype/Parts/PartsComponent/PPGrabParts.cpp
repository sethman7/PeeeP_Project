// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPGrabParts.h"
#include "Parts/PartsData/PPGrabPartsData.h"
#include "Character/PPCharacterPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CollisionQueryParams.h"
#include "Camera/CameraComponent.h"

UPPGrabParts::UPPGrabParts()
{
	PrimaryComponentTick.bCanEverTick = true;

	Owner = GetOwner();

	static ConstructorHelpers::FObjectFinder<UPPGrabPartsData> GrabPartsDataRef(TEXT("/Script/PeeeP_Prototype.PPGrabPartsData'/Game/Parts/Grab/GrabData.GrabData'"));
	if (GrabPartsDataRef.Object)
	{
		PartsData = GrabPartsDataRef.Object;
	}

	//Setup
	APPCharacterPlayer* PlayerCharacter = Cast<APPCharacterPlayer>(Owner);
	if (PlayerCharacter)
	{
		UActorComponent* GrabComponent = PlayerCharacter->AddComponentByClass(UPhysicsHandleComponent::StaticClass(), true, FTransform::Identity, false);
		GrabHandle = CastChecked<UPhysicsHandleComponent>(GrabComponent);

		APlayerController* PlayerController = CastChecked<APlayerController>(PlayerCharacter->GetController());
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UPPGrabPartsData* GrabPartsData = Cast<UPPGrabPartsData>(PartsData);
			if (GrabPartsData)
			{
				Subsystem->AddMappingContext(GrabPartsData->PartsMappingContext, 1);
				UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent);

				EnhancedInputComponent->BindAction(GrabPartsData->GrabAction, ETriggerEvent::Triggered, this, &UPPGrabParts::GrabInteraction);
				EnhancedInputComponent->BindAction(GrabPartsData->GrabAction, ETriggerEvent::Completed, this, &UPPGrabParts::GrabRelease);
			}
		}
	}
}

void UPPGrabParts::BeginDestroy()
{
	Super::BeginDestroy();

	if (GrabHandle)
	{
		GrabHandle->DestroyComponent();
	}
}


void UPPGrabParts::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GrabHandle->GetGrabbedComponent())
	{
		if (Owner)
		{
			GrabHandle->SetTargetLocation((Owner->GetActorForwardVector() * 100.0f) + Owner->GetActorLocation());
		}
	}
}

void UPPGrabParts::GrabInteraction()
{
	UE_LOG(LogTemp, Log, TEXT("Grab Start"));

	APPCharacterPlayer* PlayerCharacter = Cast<APPCharacterPlayer>(Owner);
	if (PlayerCharacter)
	{
		FVector CameraPos = PlayerCharacter->GetCamera()->GetComponentLocation();
		FVector CameraForwardVector = PlayerCharacter->GetCamera()->GetForwardVector();
		FVector EndPos = CameraPos + CameraForwardVector * 800.f;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(Grab), false, PlayerCharacter);

		bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraPos, EndPos, ECC_GameTraceChannel2, CollisionParam, FCollisionResponseParams(ECR_Block));
		if (IsHit)
		{
			GrabHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), TEXT("None"), HitResult.GetComponent()->GetComponentLocation(), FRotator::ZeroRotator);

			UE_LOG(LogTemp, Log, TEXT("GrabHit"));
		}

		FColor DebugColor(255, 0, 0);

		DrawDebugLine(GetWorld(), CameraPos, EndPos, DebugColor, false, 5.0f);
	}
}

void UPPGrabParts::GrabRelease()
{
	UE_LOG(LogTemp, Log, TEXT("Grab End"));

	if (GrabHandle->GetGrabbedComponent())
	{
		GrabHandle->ReleaseComponent();
	}
}
