// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PPCharacterControlData.h"
#include "Interface/PPButtonExecuteInterface.h"


void APPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APPCharacterPlayer::Tick(float DeltaTime)
{
	if (GrabHandle->GetGrabbedComponent())
	{
		GrabHandle->SetTargetLocation((GetActorForwardVector() * 50.0f) + GetActorLocation());
	}
}

void APPCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(ButtonInteract, ETriggerEvent::Triggered, this, &APPCharacterPlayer::ButtonInteraction);
	EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::GrabInteraction);
	EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &APPCharacterPlayer::GrabRelease);

}

void APPCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UPPCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void APPCharacterPlayer::SetCharacterControlData(const UPPCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void APPCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APPCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APPCharacterPlayer::ButtonInteraction(const FInputActionValue& Value)
{
	FVector CameraPos = FollowCamera->GetComponentLocation();
	FVector CameraForwardVector = FollowCamera->GetForwardVector();
	FVector EndPos = CameraPos + CameraForwardVector * 600.f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(Button), false, this);
	

	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraPos, EndPos, ECC_GameTraceChannel1, CollisionParam, FCollisionResponseParams(ECR_Block));

	if (IsHit)
	{
		AActor* HitActor = HitResult.GetActor();
		IPPButtonExecuteInterface* ButtonActor = Cast<IPPButtonExecuteInterface>(HitActor);
		ensure(ButtonActor);
		if (ButtonActor != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("FindButton"));
			ButtonActor->Execute();
		}
	}

	FColor DebugColor(255, 0, 0);

	DrawDebugLine(GetWorld(), CameraPos, EndPos, DebugColor, false, 5.0f);

}

void APPCharacterPlayer::GrabInteraction()
{
	UE_LOG(LogTemp, Log, TEXT("Button Click"));
	FVector CameraPos = FollowCamera->GetComponentLocation();
	FVector CameraForwardVector = FollowCamera->GetForwardVector();
	FVector EndPos = CameraPos + CameraForwardVector * 600.f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(Grab), true, this);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraPos, EndPos, ECC_GameTraceChannel2, CollisionParam, FCollisionResponseParams(ECR_Block));
	if (IsHit)
	{
		GrabObectDistanceFromCamera = HitResult.Distance;
		GrabHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), TEXT("None"), HitResult.GetComponent()->GetComponentLocation(), FRotator::ZeroRotator);

		UE_LOG(LogTemp, Log, TEXT("GrabHit"));
	}

	FColor DebugColor(255, 0, 0);

	DrawDebugLine(GetWorld(), CameraPos, EndPos, DebugColor, false, 5.0f);
}

void APPCharacterPlayer::GrabRelease()
{
	if (GrabHandle->GetGrabbedComponent())
	{
		GrabHandle->ReleaseComponent();
	}
}



APPCharacterPlayer::APPCharacterPlayer()
{
	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ButtonInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Button.IA_Button'"));
	if(!ButtonInteractRef.Object)
	{
		ButtonInteract = ButtonInteractRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> GrabInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Grab.IA_Grab'"));
	if (nullptr != GrabInteractRef.Object)
	{
		GrabAction = GrabInteractRef.Object;
	}

	// Grab
	GrabHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// CameraBoom 컴포넌트를 가져옴
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 25.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));	// FollowCamera 컴포넌트를 가져옴
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}


