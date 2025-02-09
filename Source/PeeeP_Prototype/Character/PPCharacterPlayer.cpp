// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
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
#include "GameMode/PPPlayerController.h"
//#include "Animation/AnimMontage.h"
#include "NiagaraComponent.h"
#include "Prop/PPCleaningRobot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameMode/PPGameModeBase.h"
#include "GameMode/PPPlayerState.h"
#include "Components/AudioComponent.h"

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
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Run.IA_Run'"));
	if (!InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
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

	// Default Mesh & Animation Setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/PeePCharacter/rig_new.rig_new'"));
	if (MeshRef.Object)
	{
		DefaultSkeletonMesh = MeshRef.Object;
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprintRef(TEXT("/Game/Characters/PeePCharacter/Animation/ABP_PeeeP.ABP_PeeeP_C"));
	if (AnimBlueprintRef.Class)
	{
		DefaultAnimClass = AnimBlueprintRef.Class;
		GetMesh()->SetAnimInstanceClass(AnimBlueprintRef.Class);
	}

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// CameraBoom Ïª¥Ìè¨?Ñå?ä∏Î•? Í∞??†∏?ò¥
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 100.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 5.0f;
	CameraBoom->CameraRotationLagSpeed = 20.0f;
	CameraBoom->CameraLagMaxDistance = 500.f;
	CameraBoom->ProbeSize = 8.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));	// FollowCamera Ïª¥Ìè¨?Ñå?ä∏Î•? Í∞??†∏?ò¥
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 90.0f;

	ElectricDischargeComponent = CreateDefaultSubobject<UPPElectricDischargeComponent>(TEXT("ElectricDischargeComponent"));

	// Player Movement Setting
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 1.6f;
	this->MaxWalkSpeed = 60.0f;										// Setting Default Max Walk Speed
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;		// Apply Default Max Walk Speed
	GetCharacterMovement()->MaxStepHeight = 5.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.f);
	this->bIsRunning = false;

	ElectricNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ElectricEffectComponent"));
	ElectricNiagaraComponent->SetupAttachment(RootComponent);

	PlayerEffectNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PlayerEffectComponent"));
	PlayerEffectNiagaraComponent->SetupAttachment(RootComponent);

	// ?ù∏Î≤§ÌÜ†Î¶? Ïª¥Ìè¨?Ñå?ä∏
	InventoryComponent = CreateDefaultSubobject<UPPInventoryComponent>(TEXT("InventoryComponent"));

	AttachedMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttachedMesh"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APPCharacterPlayer::OnBeginOverlap);
}

void APPCharacterPlayer::OnDeath()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->DisableInput(PlayerController);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		ElectricDischargeComponent->Reset();

		APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
		if (IsValid(GameMode))
		{
			GameMode->MoveCharacterToSpawnLocation(this);
		}

		PlayerController->SetControlRotation(FRotator(0.0f, 90.0f, 0.0f));
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		PlayerController->EnableInput(PlayerController);
	}
}

void APPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		GameMode->SetInitialSpawnLocation(PlayerController);
	}

	//Test_EquipGrabParts();

	//// Parts ?ûÑ?ãúÎ°? ?Éù?Ñ±?ûê?óê?Ñú Î∂??ó¨?ï®
	//// ?ï¥?ãπ Î∂?Î∂ÑÏ?? ?ÇòÏ§ëÏóê ?ù∏Î≤§ÌÜ†Î¶¨Ïóê?Ñú ?ç∞?ù¥?Ñ∞ ?ù¥?ö©?ï¥?Ñú ?ååÏ∏? Î≥?Í≤ΩÌïò?äî ?ï®?àò ?î∞Î°? ÎßåÎì§?ñ¥?Ñú ?†Å?ö©?ïòÎ©? ?ê† ?ìØ
	//UActorComponent* PartsComponent = AddComponentByClass(UPPGrabParts::StaticClass(), true, FTransform::Identity, false);
	//Parts = CastChecked<UPPPartsBase>(PartsComponent);
}

void APPCharacterPlayer::Tick(float DeltaTime)
{
	//Idle?ÉÅ?Éú?óê?Ñú?ùò ?îå?†à?ù¥?ñ¥Í∞? ???ÏßÅÏù¥?äî actor????ùò Ï∂©Îèå?ùÑ Î¨¥Ïãú?ïòÍ≤? ?êòÎ≤ÑÎ†§ ?ã§?ùå ÏΩîÎìúÎ•? Ï∂îÍ???ï®.
	FHitResult OutHit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, 0.03f), GetActorRotation(), true, OutHit);
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, -0.03f), GetActorRotation(), true, OutHit);
}

void APPCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	/// Binding Section
	/// If you added new action, you should add new binding action here.
	/// Create some function for actions about added input and bind it.
	
	// Jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	// Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::Move);
	// Look
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::Look);
	// Running
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::OnRunningStart);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APPCharacterPlayer::OnRunningEnd);
	// Button Interaction(Unused)
	EnhancedInputComponent->BindAction(ButtonInteract, ETriggerEvent::Triggered, this, &APPCharacterPlayer::ButtonInteraction);
	// Open Menu
	EnhancedInputComponent->BindAction(OpenMenuInteract, ETriggerEvent::Triggered, this, &APPCharacterPlayer::OpenMenu);

	// Electric Discharge Component
	if (ElectricDischargeComponent)
	{
		EnhancedInputComponent->BindAction(ElectricDischargeAction, ETriggerEvent::Ongoing, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::Charging);
		EnhancedInputComponent->BindAction(ElectricDischargeAction, ETriggerEvent::Completed, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::Discharge);
		EnhancedInputComponent->BindAction(ElectricDischargeModeChangeAction, ETriggerEvent::Completed, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::ChangeDischargeMode);
	}
	// Quick Slot
	EnhancedInputComponent->BindAction(QuickSlotMoveAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::QuickSlotMove);
	EnhancedInputComponent->BindAction(QuickSlotUseAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::QuickSlotUse);
}

void APPCharacterPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APPCleaningRobot* CleaingRobotRef = Cast<APPCleaningRobot>(OtherActor))
	{
		FRotator Rotation = CleaingRobotRef->GetActorRotation();
		FVector KnockbackVelocity = UKismetMathLibrary::GetForwardVector(Rotation) * CleaingRobotRef->KnockbackStrength;

		// ?îå?†à?ù¥?ñ¥?óêÍ≤? ?ÑâÎ∞? ?†Å?ö©
		LaunchCharacter(KnockbackVelocity, true, true);
		ElectricDischargeComponent->ChargeElectric(CleaingRobotRef->ElectricLossRate);
	}
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
		//ensure(ButtonActor);
		if (ButtonActor != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("FindButton"));
			ButtonActor->Interact();
		}

	}

	FColor DebugColor(255, 0, 0);

	DrawDebugLine(GetWorld(), CameraPos, EndPos, DebugColor, false, 5.0f);
}

void APPCharacterPlayer::OnRunningStart(const FInputActionValue& Value)
{
	// Set Player Max Walk Speed for Running.
	if (!this->bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = 150.f;	// Here is Running Max Walk Speed. You can Setting Running Max Walk Speed.
		this->bIsRunning = true;
		UE_LOG(LogTemp, Log, TEXT("Running Start"));
	}
}

void APPCharacterPlayer::OnRunningEnd(const FInputActionValue& Value)
{
	// Set Player Max Walk Speed to Default Max Walk Speed.
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;
	this->bIsRunning = false;
	UE_LOG(LogTemp, Log, TEXT("Running End"));
}

void APPCharacterPlayer::SetDefaultMeshAndAnim()
{
	GetMesh()->SetSkeletalMesh(DefaultSkeletonMesh);
	GetMesh()->SetAnimInstanceClass(DefaultAnimClass);
}

UCameraComponent* APPCharacterPlayer::GetCamera()
{
	return FollowCamera;
}


void APPCharacterPlayer::SwitchParts(UPPPartsDataBase* InPartsData)
{
	RemoveParts();

	UE_LOG(LogTemp, Log, TEXT("Creat New Parts"));
	UActorComponent* PartsComponent = AddComponentByClass(InPartsData->PartsComponent, true, FTransform::Identity, false);
	Parts = CastChecked<UPPPartsBase>(PartsComponent);
	if (Parts)
	{
		GetMesh()->SetSkeletalMesh(Parts->GetPartsData()->PartsMesh);
		GetMesh()->SetAnimClass(Parts->GetPartsData()->AnimClass);
	}
}

void APPCharacterPlayer::RemoveParts()
{
    if (Parts)
    {
        UE_LOG(LogTemp, Log, TEXT("Destroy Parts"));
        Parts->DestroyComponent();
    }
}

void APPCharacterPlayer::Test_EquipGrabParts()
{

	UActorComponent* PartsComponent = AddComponentByClass(UPPGrabParts::StaticClass(), true, FTransform::Identity, false);
	Parts = CastChecked<UPPPartsBase>(PartsComponent);

	if (Parts)
	{
		GetMesh()->SetSkeletalMesh(Parts->GetPartsData()->PartsMesh);
		GetMesh()->SetAnimClass(Parts->GetPartsData()->AnimClass);
	}
}

void APPCharacterPlayer::PlayAnimation(UAnimMontage* InAnimMontage)
{
	UAnimMontage* CurrentMontage = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
	if (CurrentMontage == nullptr)
	{
		PlayAnimMontage(InAnimMontage);
	}
}

//Í∑∏Îû© ?ï†?ãàÎ©îÏù¥?Öò ?ûë?èô ?õÑ, NotifyÎ•? ?Üµ?ï¥ ?ò∏Ï∂úÎê®. Í∑∏Îû©?óê ?ãø??? ?ò§Î∏åÏ†ù?ä∏Í∞? ?ûà?äîÏß? Ï≤¥ÌÅ¨. 
void APPCharacterPlayer::GrabHitCheck()
{
	UPPGrabParts* GrabParts = Cast<UPPGrabParts>(Parts);
	if (GrabParts == nullptr) return;

	FHitResult HitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Grab), false, this);
	const FVector StartPos = GetMesh()->GetSocketLocation(Parts->HitSocket) + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector EndPos = StartPos + GetActorForwardVector() * 5.0f;

	bool HitDetected = GetWorld()->SweepSingleByChannel(HitResult, StartPos, EndPos, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(10.0f), Params);
	if (HitDetected)
	{
		UE_LOG(LogTemp, Log, TEXT("GrabHit"));
		GrabParts->SetIsGrabbed(true);
		GrabParts->Grab(HitResult);
	}


#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = StartPos + (EndPos - StartPos) * 0.5f;
	float CapsuleHalfHeight = 5.0f * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 10.0f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif
}



void APPCharacterPlayer::ReduationMaxWalkSpeedRatio(float InReductionRatio)
{
	GetCharacterMovement()->MaxWalkSpeed *= InReductionRatio;
}

void APPCharacterPlayer::RevertMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;
}

UNiagaraComponent* APPCharacterPlayer::GetElectricNiagaraComponent() const
{
	return ElectricNiagaraComponent;
}

UNiagaraComponent* APPCharacterPlayer::GetPlayerEffectNiagaraComponent() const
{
	return PlayerEffectNiagaraComponent;
}

UPPInventoryComponent* APPCharacterPlayer::GetInventoryComponent()
{
	return InventoryComponent;
}

void APPCharacterPlayer::QuickSlotMove(const FInputActionValue& Value)
{
	// ÎßàÏö∞?ä§ ?ú† ?óÖ: ?ñë?àò, ÎßàÏö∞?ä§ ?ú† ?ã§?ö¥: ?ùå?àò
	float MoveDir = Value.Get<float>();

	// ÎßàÏö∞?ä§ ?ú† ?ó∞?Üç?êú ?ûÖ?†• Î∞©Ï??
	float InputInterval = 0.4f;	// ?ã§?ùå ?ù∏?íãÍπåÏ?? Í∞ÑÍ≤©
	if (bIsAllowWheelInput)
	{
		UE_LOG(LogTemp, Log, TEXT("Quick Slot Move: %f"), MoveDir);
		bIsAllowWheelInput = false;
		GetWorld()->GetTimerManager().SetTimer(QuickSlotMoveTimer, [&]() {bIsAllowWheelInput = true; }, InputInterval, false);

		// ?ä¨Î°? ???ÏßÅÏûÑ Íµ¨ÌòÑÎ∂?
		// ?ä¨Î°??ù¥ Ï≤òÏùå?ù¥?Çò ?Åù?óê ?ã§?ã§?ûê?ùÑ Í≤ΩÏö∞ ?ôî?Ç¥?ëú?äî ?Ç¨?ùºÏß?Ïß? ?ïäÏß?Îß? ?çî?ù¥?ÉÅ ???ÏßÅÏù¥Ïß? ?ïä?ïÑ?ïº ?ï®.
		if (MoveDir < 0)		// Wheel Down
		{
			InventoryComponent->ModifyCurrentSlotIndex(1);
		}
		else if (MoveDir > 0)	// Wheel Up
		{
			InventoryComponent->ModifyCurrentSlotIndex(-1);
		}
	}
}

void APPCharacterPlayer::QuickSlotUse(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Quick Slot Used!"));

	// ?Ñ†?Éù?êú ?ä¨Î°? ?Ç¨?ö©Î∂?
	// ÎØ∏Ïû•Ï∞? ?ÉÅ?Éú?óê?Ñú?äî ?ñ¥?ñ†?ïú ?ååÏ∏?(?òÑ?û¨ Î≥¥Ïú†?ïòÍ≥? ?ûà?äî)Î•? ?Ñ†?Éù?ïò?çî?ùº?èÑ ?†ï?ÉÅ Ï∞©Ïö©?ù¥ ?êò?ñ¥?ïº ?ï®.
	// ?ù¥ÎØ? ?û•Ï∞©ÌïòÍ≥? ?ûà?äî ?ååÏ∏†Î?? ?û¨?Ñ†?Éù ?ï† Í≤ΩÏö∞ ?û•Ï∞©ÌïòÍ≥? ?ûà?äî ?ååÏ∏†Î?? ?ï¥?†ú?ï¥?ïº ?ï®.
	// ?ååÏ∏†Î?? ?û•Ï∞©ÌïòÍ∏? ?úÑ?ï¥?Ñú?äî ?òÑ?û¨ ?ä¨Î°??ùò ?ù∏?ç±?ä§Î•? ?ïå?ïÑ?ïº Í∞??ä•

	// ?Öå?ä§?ä∏ Î∂?Î∂?: ?ä¨Î°? ?ù∏?ç±?ä§ 0Î≤àÏùò ?ååÏ∏? ?ä¨Î°? ????ûÖ?ùò ?ïÑ?ù¥?Öú?ùÑ ?Ç¨?ö©?ï¥ Ï£ºÏÑ∏?öî.
	// Í≤∞Í≥º: Î°úÍ∑∏ ?†ï?ÉÅ Ï∂úÎ†•
	//InventoryComponent->UseItem(0, ESlotType::ST_InventoryParts);

	// ?òÑ?û¨ ?Ñ†?Éù?êú ?ä¨Î°??ùÑ Í∏∞Î∞ò?úºÎ°? ?ïò?ó¨ ?ïÑ?ù¥?Öú ?Ç¨?ö©
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





