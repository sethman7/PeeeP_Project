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

	// 파츠에 대한 데이터 갖고있는 파츠 데이터 연결
	static ConstructorHelpers::FObjectFinder<UPPGrabPartsData> GrabPartsDataRef(TEXT("/Script/PeeeP_Prototype.PPGrabPartsData'/Game/Parts/Grab/GrabData.GrabData'"));
	if (GrabPartsDataRef.Object)
	{
		PartsData = GrabPartsDataRef.Object;
	}


	//파츠 데이터에 있는 정보들을 이용해 해당 파츠 기본 정보 세팅
	//이 과정에서는 데이터에 있는 인풋 맵핑과 인풋액션을 연결하고 플레이어 컨트롤러에 우선순위 1로 해서 기본 조작 바로 다음 우선순위를 갖게 함
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

// 그랩 파츠로 그랩 한 오브젝트 이동 업데이트
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
// 그랩 시작 시 로직 구현
// 현재 Linetrace로 하고있는데 이는 애님 추가 시 손바닥 본을 기준으로 Overlap으로 검사해서 판정 변경 예정
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
// 그랩 끝날 때 작동
void UPPGrabParts::GrabRelease()
{
	UE_LOG(LogTemp, Log, TEXT("Grab End"));

	if (GrabHandle->GetGrabbedComponent())
	{
		GrabHandle->ReleaseComponent();
	}
}
