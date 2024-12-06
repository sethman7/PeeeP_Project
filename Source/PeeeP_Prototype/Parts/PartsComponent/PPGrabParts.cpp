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



	// Grab() -> GrabRelease() 호출 순서가 무조건 보장되어야만 함. 하지만 그랩 애니메이션 발동 후 overlap된 오브젝트가 있을때만 Grab()이 호출되므로, 호출 직전에 키를 때버려서
	// GrabRelase()가 호출되버리면 순서가 다음과 같이 GrabRelease() -> Grab() 바뀌는 경우가 생길 수 있음.  
	// 그렇게 될 경우, 키를 누르고 있지 않은 상황에서도 오브젝트를 계속 들고있게 되버림. 
	IsReleased = true;

}


void UPPGrabParts::BeginPlay()
{
	Super::BeginPlay();
}

// 그랩 파츠로 그랩 한 오브젝트 이동 업데이트
void UPPGrabParts::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//GrabHandle 이 컴포넌트를 잡고 있고 Owner가 유효할 때만, 잡고있는 오브젝트 위치 업데이트.
	if (GrabHandle->GetGrabbedComponent() && Owner)
	{
		UpdateGrabbedObjectPosition();
	}
}


void UPPGrabParts::HandleGrabAnimation()
{
	IsReleased = false;
	if (OnPlayAnimation.IsBound())
	{
		OnPlayAnimation.Broadcast();  // 델리게이트 호출
	}
}

void UPPGrabParts::Grab(FHitResult& InHitResult)
{
	UE_LOG(LogTemp, Log, TEXT("Grab Start"));
	if (IsReleased) return;
	GrabHandle->GrabComponentAtLocationWithRotation(InHitResult.GetComponent(), TEXT("None"), InHitResult.GetComponent()->GetComponentLocation(), FRotator::ZeroRotator);
}




// 그랩 끝날 때 작동
void UPPGrabParts::GrabRelease()
{	
	UE_LOG(LogTemp, Log, TEXT("Grab End"));
	IsReleased = true;
	if (GrabHandle->GetGrabbedComponent())
	{
		GrabHandle->ReleaseComponent();
	}
}

void UPPGrabParts::UpdateGrabbedObjectPosition()
{
	UCameraComponent* FollowCamera = Owner->FindComponentByClass<UCameraComponent>();
	FVector Start = Owner->GetActorLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector GrabbedObjectPosition = (Start + (ForwardVector * 300.0f));

	GrabHandle->SetTargetLocation(GrabbedObjectPosition);
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

			EnhancedInputComponent->BindAction(GrabPartsData->GrabAction, ETriggerEvent::Started, this, &UPPGrabParts::HandleGrabAnimation);
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
