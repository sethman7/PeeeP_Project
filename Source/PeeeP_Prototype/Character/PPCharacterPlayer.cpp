// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PPCharacterControlData.h"
#include "Interface/PPInteractableObjectInterface.h"
#include "Parts/PartsComponent/PPGrabParts.h"
#include "Parts/PartsData/PPPartsDataBase.h"
#include "Component/PPElectricDischargeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/PPPlayerController.h"
#include "NiagaraComponent.h"


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
	if (!ButtonInteractRef.Object)
	{
		ButtonInteract = ButtonInteractRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ElectricDischargeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Discharge.IA_Discharge'"));
	if (!ElectricDischargeActionRef.Object)
	{
		ElectricDischargeAction = ElectricDischargeActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ElectricDischargeModeChangeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_DischargeModeChange.IA_DischargeModeChange'"));
	if (!ElectricDischargeModeChangeActionRef.Object)
	{
		ElectricDischargeModeChangeAction = ElectricDischargeModeChangeActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> OpenMenuActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OpenMenu.IA_OpenMenu'"));
	if (!OpenMenuActionRef.Object)
	{
		OpenMenuInteract = OpenMenuActionRef.Object;
	}

	// Quick Slot Section
	static ConstructorHelpers::FObjectFinder<UInputAction> QuickSlotMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_QuickSlotMove.IA_QuickSlotMove'"));
	if (!QuickSlotMoveRef.Object)
	{
		QuickSlotMoveAction = QuickSlotMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> QuickSlotUseRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_QuickSlotUse.IA_QuickSlotUse'"));
	if (!QuickSlotUseRef.Object)
	{
		QuickSlotUseAction = QuickSlotUseRef.Object;
	}
	bIsAllowWheelInput = true;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// CameraBoom 컴포넌트를 가져옴
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 100.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 5.0f;
	CameraBoom->CameraRotationLagSpeed = 20.f;
	CameraBoom->CameraLagMaxDistance = 500.f;
	CameraBoom->ProbeSize = 8.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));	// FollowCamera 컴포넌트를 가져옴
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 80.0f;

	ElectricDischargeComponent = CreateDefaultSubobject<UPPElectricDischargeComponent>(TEXT("ElectricDischargeComponent"));

	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 1.6f;
	this->MaxWalkSpeed = 150.0f;
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;
	GetCharacterMovement()->MaxStepHeight = 10.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.f);

	PlayerCharacterNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectComponent"));
	PlayerCharacterNiagaraComponent->SetupAttachment(RootComponent);
	
	// 인벤토리 컴포넌트
	InventoryComponent = CreateDefaultSubobject<UPPInventoryComponent>(TEXT("InventoryComponent"));
}

void APPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	//// Parts 임시로 생성자에서 부여함
	//// 해당 부분은 나중에 인벤토리에서 데이터 이용해서 파츠 변경하는 함수 따로 만들어서 적용하면 될 듯
	//UActorComponent* PartsComponent = AddComponentByClass(UPPGrabParts::StaticClass(), true, FTransform::Identity, false);
	//Parts = CastChecked<UPPPartsBase>(PartsComponent);
}

void APPCharacterPlayer::Tick(float DeltaTime)
{

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
	EnhancedInputComponent->BindAction(OpenMenuInteract, ETriggerEvent::Triggered, this, &APPCharacterPlayer::OpenMenu);
	if (ElectricDischargeComponent)
	{
		EnhancedInputComponent->BindAction(ElectricDischargeAction, ETriggerEvent::Ongoing, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::Charging);
		EnhancedInputComponent->BindAction(ElectricDischargeAction, ETriggerEvent::Completed, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::Discharge);
		EnhancedInputComponent->BindAction(ElectricDischargeModeChangeAction, ETriggerEvent::Completed, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::ChangeDischargeMode);
	}
	
	EnhancedInputComponent->BindAction(QuickSlotMoveAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::QuickSlotMove);
	EnhancedInputComponent->BindAction(QuickSlotUseAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::QuickSlotUse);
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
		IPPInteractableObjectInterface* ButtonActor = Cast<IPPInteractableObjectInterface>(HitActor);
		ensure(ButtonActor);
		if (ButtonActor != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("FindButton"));
			ButtonActor->Interact();
		}
	}

	FColor DebugColor(255, 0, 0);

	DrawDebugLine(GetWorld(), CameraPos, EndPos, DebugColor, false, 5.0f);
}

UCameraComponent* APPCharacterPlayer::GetCamera()
{
	return FollowCamera;
}


void APPCharacterPlayer::SwitchParts(UPPPartsDataBase* InPartsData)
{
	if (Parts)
	{
		UE_LOG(LogTemp, Log, TEXT("Destroy Parts"));
		Parts->DestroyComponent();
	}

	UE_LOG(LogTemp, Log, TEXT("Creat New Parts"));
	UActorComponent* PartsComponent = AddComponentByClass(InPartsData->PartsComponent, true, FTransform::Identity, false);
	Parts = CastChecked<UPPPartsBase>(PartsComponent);
}

void APPCharacterPlayer::ReduationMaxWalkSpeedRatio(float InReductionRatio)
{
	GetCharacterMovement()->MaxWalkSpeed *= InReductionRatio;
}

void APPCharacterPlayer::RevertMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;
}


UNiagaraComponent* APPCharacterPlayer::GetPlayerCharacterNiagaraComponent() const
{
	return PlayerCharacterNiagaraComponent;;
}

UPPInventoryComponent* APPCharacterPlayer::GetInventoryComponent()
{
	return InventoryComponent;
}

void APPCharacterPlayer::QuickSlotMove(const FInputActionValue& Value)
{
	// 마우스 휠 업: 양수, 마우스 휠 다운: 음수
	float MoveDir = Value.Get<float>();

	// 마우스 휠 연속된 입력 방지
	float InputInterval = 0.2f;	// 다음 인풋까지 간격
	if (bIsAllowWheelInput)
	{
		UE_LOG(LogTemp, Log, TEXT("Quick Slot Move: %f"), MoveDir);
		bIsAllowWheelInput = false;
		GetWorld()->GetTimerManager().SetTimer(QuickSlotMoveTimer, [&]() {bIsAllowWheelInput = true; }, InputInterval, false);
	}

	// 슬롯 움직임 구현부
	// 슬롯이 처음이나 끝에 다다랐을 경우 화살표는 사라지지 않지만 더이상 움직이지 않아야 함.
	if (MoveDir < 0)		// Wheel Down
	{
		InventoryComponent->ModifyCurrentSlotIndex(1);
	}
	else if (MoveDir > 0)	// Wheel Up
	{
		InventoryComponent->ModifyCurrentSlotIndex(-1);
	}

}

void APPCharacterPlayer::QuickSlotUse(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Quick Slot Used!"));

	// 선택된 슬롯 사용부
	// 미장착 상태에서는 어떠한 파츠(현재 보유하고 있는)를 선택하더라도 정상 착용이 되어야 함.
	// 이미 장착하고 있는 파츠를 재선택 할 경우 장착하고 있는 파츠를 해제해야 함.
	// 파츠를 장착하기 위해서는 현재 슬롯의 인덱스를 알아야 가능

	// 테스트 부분: 슬롯 인덱스 0번의 파츠 슬롯 타입의 아이템을 사용해 주세요.
	// 결과: 로그 정상 출력
	//InventoryComponent->UseItem(0, ESlotType::ST_InventoryParts);
	InventoryComponent->UseItemCurrentIndex(ESlotType::ST_InventoryParts);

}

void APPCharacterPlayer::SetWheelInputAllow(bool Value)
{
	bIsAllowWheelInput = Value;
}

void APPCharacterPlayer::OpenMenu()
{
	APPPlayerController* PlayerController = Cast<APPPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->OpenMenu();
	}
}

UPPElectricDischargeComponent* APPCharacterPlayer::GetElectricDischargeComponent()
{
	return ElectricDischargeComponent;

}





