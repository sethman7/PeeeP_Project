// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPGrabParts.h"
#include "Parts/PartsData/PPGrabPartsData.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CollisionQueryParams.h"
#include "Camera/CameraComponent.h"

UPPGrabParts::UPPGrabParts()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetPartsActive(false);

	// 파츠에 대한 데이터 갖고있는 파츠 데이터 연결
	static ConstructorHelpers::FObjectFinder<UPPGrabPartsData> GrabPartsDataRef(TEXT("/Game/Parts/Grab/GrabPartsData.GrabPartsData"));
	if (GrabPartsDataRef.Object)
	{
		PartsData = GrabPartsDataRef.Object;
	}

	ensure(PartsData);

	AttachmentSocket = TEXT("Bip001-R-Hand");	//플레이어 오른손
	HitSocket = TEXT("Bone010Socket");		    //그랩 매쉬 받으면 변경할 예정.
}


void UPPGrabParts::BeginPlay()
{
	Super::BeginPlay();
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


void UPPGrabParts::Grab()
{
	UE_LOG(LogTemp, Log, TEXT("Grab Start"));
	OnPlayAnimation.Broadcast(); // 애니메이션 호출 
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



void UPPGrabParts::SetUp()
{

	//파츠 데이터에 있는 정보들을 이용해 해당 파츠 기본 정보 세팅
	//이 과정에서는 데이터에 있는 인풋 맵핑과 인풋액션을 연결하고 플레이어 컨트롤러에 우선순위 1로 해서 기본 조작 바로 다음 우선순위를 갖게 함

	if (GrabHandle == nullptr)
	{
		UActorComponent* GrabComponent = Owner->AddComponentByClass(UPhysicsHandleComponent::StaticClass(), true, FTransform::Identity, false);
		GrabHandle = CastChecked<UPhysicsHandleComponent>(GrabComponent);
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(Owner->GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		UPPGrabPartsData* GrabPartsData = Cast<UPPGrabPartsData>(PartsData);
		if (GrabPartsData)
		{
			Subsystem->AddMappingContext(GrabPartsData->PartsMappingContext, 1);
			UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent);

			EnhancedInputComponent->BindAction(GrabPartsData->GrabAction, ETriggerEvent::Triggered, this, &UPPGrabParts::Grab);
			EnhancedInputComponent->BindAction(GrabPartsData->GrabAction, ETriggerEvent::Completed, this, &UPPGrabParts::GrabRelease);
		}
	}
	
}

void UPPGrabParts::SetPartsActive(bool flag)
{
	Super::SetPartsActive(flag);
	SetActive(flag);
	PrimaryComponentTick.bStartWithTickEnabled = flag;
}
