// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPGrabParts.h"
#include "Parts/PartsData/PPGrabPartsData.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CollisionQueryParams.h"
#include "Camera/CameraComponent.h"
#include "Character/PPCharacterPlayer.h"

UPPGrabParts::UPPGrabParts()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 파츠에 대한 데이터 갖고있는 파츠 데이터 연결
	static ConstructorHelpers::FObjectFinder<UPPGrabPartsData> GrabPartsDataRef(TEXT("/Game/Parts/Grab/GrabPartsData.GrabPartsData"));
	if (GrabPartsDataRef.Object)
	{
		PartsData = GrabPartsDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GrabAnimMontageRef(TEXT("/Game/Characters/PeePCharacter/Animation/AM_Grab.AM_Grab"));
	if (GrabAnimMontageRef.Object)
	{
		GrabAnimMontage = GrabAnimMontageRef.Object;
	}


	HitSocket = TEXT("Bone010");

	// Grab() -> GrabRelease() 호출 순서가 무조건 보장되어야만 함. 하지만 그랩 애니메이션 발동 후 overlap된 오브젝트가 있을때 만 Grab()이 호출되므로, 호출 직전에 키를 때버려서
	// GrabRelase()가 호출되버리면 순서가 다음과 같이 GrabRelease() -> Grab() 바뀌는 경우가 생길 수 있음.  
	// 그렇게 될 경우, 키를 누르고 있지 않은 상황에서도 오브젝트를 계속 들고있게 되버림. 따라서 그랩 애니메이션 동작 중에 GrabRelease() 호출 되는 경우 Grab()함수내에서
	// 그랩을 하지못하도록 bool변수 IsGrabbed으로 체크함. 
	IsGrabbed = false;	
}


void UPPGrabParts::BeginDestroy()
{
	Super::BeginDestroy();

	if (GrabHandle)
	{
		GrabHandle->DestroyComponent();
	}
}

void UPPGrabParts::OnComponentCreated()
{
    Super::OnComponentCreated();

    Owner = Cast<APPCharacterPlayer>(GetOwner());

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

                EnhancedInputComponent->BindAction(GrabPartsData->GrabAction, ETriggerEvent::Started, this, &UPPGrabParts::HandleGrabAnimation);
                EnhancedInputComponent->BindAction(GrabPartsData->GrabAction, ETriggerEvent::Completed, this, &UPPGrabParts::GrabRelease);
            }
        }
    }
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
	if (!IsGrabbed)
	{
		Owner->PlayAnimation(GrabAnimMontage);
	}
	
}

void UPPGrabParts::Grab(FHitResult& InHitResult)
{
	UE_LOG(LogTemp, Log, TEXT("Grab Start"));
	if (!IsGrabbed) return;
	GrabHandle->GrabComponentAtLocationWithRotation(InHitResult.GetComponent(), TEXT("None"), InHitResult.GetComponent()->GetComponentLocation(), FRotator::ZeroRotator);
}


// 그랩 끝날 때 작동
void UPPGrabParts::GrabRelease()
{	
	if (IsGrabbed)
	{
		UE_LOG(LogTemp, Log, TEXT("Grab End"));
		IsGrabbed = false;
		if (GrabHandle->GetGrabbedComponent())
		{
			GrabHandle->ReleaseComponent();
		}
	}
}

void UPPGrabParts::UpdateGrabbedObjectPosition()
{
	/*UCameraComponent* FollowCamera = Owner->FindComponentByClass<UCameraComponent>();
	FVector Start = Owner->GetActorLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector GrabbedObjectPosition = (Start + (ForwardVector * 100.0f));*/

	FVector GrabbedObjectPosition = Owner->GetMesh()->GetSocketLocation(HitSocket);

	GrabHandle->SetTargetLocation(GrabbedObjectPosition);
}



