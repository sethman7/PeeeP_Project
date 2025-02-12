#include "PPCrane.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

APPCrane::APPCrane()
{
    PrimaryActorTick.bCanEverTick = true;

    Claw = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Claw"));
    RootComponent = Claw;

    Claw2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Claw2"));
    Claw2->SetupAttachment(Claw);

    GrabCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabCollision"));
    GrabCollision->SetupAttachment(Claw);
    GrabCollision->OnComponentBeginOverlap.AddDynamic(this, &APPCrane::OnGrabOverlapBegin);

    MoveSpeed = 100.f;
    DownDistance = 300.f;
    WaitTime = 2.f;
    XYMoveSpeed = 100.f;
    AcceptableRadius = 1.f;

    CurrentState = EPPCraneState::Idle;
    bIsSwitchPressed = false;
    bIsHoldingObject = false;
    GrabbedActor = nullptr;
    WaitingTimer = 0.f;
    TargetLocation = FVector::ZeroVector;

    bStopDownMovement = false;
    PreviousLocation = FVector::ZeroVector;

    OriginalLocation = FVector::ZeroVector;

    StartLocation = FVector::ZeroVector;
}

void APPCrane::BeginPlay()
{
    Super::BeginPlay();

    OriginalLocation = GetActorLocation();
}

void APPCrane::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    PreviousLocation = GetActorLocation();

    switch (CurrentState)
    {
    case EPPCraneState::Idle:
        break;

    case EPPCraneState::MovingDown:
    {
        if (!bStopDownMovement)
        {
            MoveCraneDown(DeltaTime);

            float currentZ = GetActorLocation().Z;
            float dist = StartLocation.Z - currentZ;
            if (dist >= DownDistance)
            {
                FVector pos = StartLocation;
                pos.Z -= DownDistance;
                SetActorLocation(pos);

                SelectOverlapActor();

                CurrentState = EPPCraneState::Waiting;
                WaitingTimer = 0.f;
            }
        }
    }
    break;

    case EPPCraneState::Waiting:
    {
        WaitingTimer += DeltaTime;
        if (WaitingTimer >= WaitTime)
        {
            if (GrabbedActor && !bIsHoldingObject)
            {
                AttachActor(GrabbedActor);
            }
            CurrentState = EPPCraneState::MovingUp;
        }
    }
    break;

    case EPPCraneState::MovingUp:
    {
        MoveCraneUp(DeltaTime);

        float currentZ = GetActorLocation().Z;
        if (currentZ >= StartLocation.Z)
        {
            FVector pos = GetActorLocation();
            pos.Z = StartLocation.Z;
            SetActorLocation(pos);

            CurrentState = EPPCraneState::MovingX;
            TargetLocation = pos + FVector(150.f, 0.f, 0.f);
        }
    }
    break;

    case EPPCraneState::MovingX:
        MoveOnX(DeltaTime);
        break;

    case EPPCraneState::MovingY:
        MoveOnY(DeltaTime);
        break;
    }
}

void APPCrane::OnGrabOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (!OtherActor || OtherActor == this) return;

    if (CurrentState == EPPCraneState::MovingDown && !bIsHoldingObject)
    {
        SetActorLocation(PreviousLocation);

        bStopDownMovement = true;

        AttachActor(OtherActor);

        CurrentState = EPPCraneState::MovingUp;
    }
}

void APPCrane::SelectOverlapActor()
{
    if (bIsHoldingObject) return;

    TArray<AActor*> OverlappedActors;
    GrabCollision->GetOverlappingActors(OverlappedActors);

    if (OverlappedActors.Num() > 0)
    {
        AActor* FirstActor = OverlappedActors[0];
        if (FirstActor != this)
        {
            GrabbedActor = FirstActor;
        }
    }
    else
    {
        GrabbedActor = nullptr;
    }
}

void APPCrane::AttachActor(AActor* ActorToGrab)
{
    if (!ActorToGrab || bIsHoldingObject)
        return;

    if (APawn* PawnActor = Cast<APawn>(ActorToGrab))
    {
        PawnActor->DisableInput(nullptr);

        if (UPawnMovementComponent* MoveComp = PawnActor->GetMovementComponent())
        {
            MoveComp->StopMovementImmediately();
        }

        if (ACharacter* Char = Cast<ACharacter>(PawnActor))
        {
            if (UCapsuleComponent* Capsule = Char->GetCapsuleComponent())
            {
                Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
        }

        PawnActor->AttachToComponent(
            Claw,
            FAttachmentTransformRules::SnapToTargetNotIncludingScale
        );
    }
    else
    {
        if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(ActorToGrab->GetRootComponent()))
        {
            RootComp->SetSimulatePhysics(false);
        }

        ActorToGrab->AttachToComponent(
            Claw,
            FAttachmentTransformRules::KeepWorldTransform
        );
    }

    bIsHoldingObject = true;
    GrabbedActor = ActorToGrab;
}

void APPCrane::DropActor()
{
    if (GrabbedActor && bIsHoldingObject)
    {
        GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

        if (APawn* PawnActor = Cast<APawn>(GrabbedActor))
        {
            PawnActor->EnableInput(nullptr);

            if (ACharacter* Char = Cast<ACharacter>(PawnActor))
            {
                if (UCapsuleComponent* Capsule = Char->GetCapsuleComponent())
                {
                    Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                }
            }
        }
        else
        {
            if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(GrabbedActor->GetRootComponent()))
            {
                RootComp->SetSimulatePhysics(true);
            }
        }
    }

    GrabbedActor = nullptr;
    bIsHoldingObject = false;
}

void APPCrane::MoveCraneDown(float DeltaTime)
{
    FVector loc = GetActorLocation();
    loc.Z -= MoveSpeed * DeltaTime;
    SetActorLocation(loc);
}

void APPCrane::MoveCraneUp(float DeltaTime)
{
    FVector loc = GetActorLocation();
    loc.Z += MoveSpeed * DeltaTime;
    SetActorLocation(loc);
}

void APPCrane::MoveOnX(float DeltaTime)
{
    FVector loc = GetActorLocation();
    float dist = (TargetLocation - loc).Size();
    if (dist <= AcceptableRadius)
    {
        loc = TargetLocation;
        SetActorLocation(loc);

        CurrentState = EPPCraneState::MovingY;
        TargetLocation = loc + FVector(0.f, 150.f, 0.f);
        return;
    }

    FVector dir = (TargetLocation - loc).GetSafeNormal();
    loc += dir * XYMoveSpeed * DeltaTime;
    SetActorLocation(loc);
}

void APPCrane::MoveOnY(float DeltaTime)
{
    FVector loc = GetActorLocation();
    float dist = (TargetLocation - loc).Size();
    if (dist <= AcceptableRadius)
    {
        // 도착
        loc = TargetLocation;
        SetActorLocation(loc);

        // 여기서 물체 또는 플레이어를 떨궈 줌
        if (bIsHoldingObject && GrabbedActor)
        {
            // 0.1초 후에 떨어뜨리기
            GetWorldTimerManager().ClearTimer(DropTimerHandle);
            GetWorldTimerManager().SetTimer(DropTimerHandle, this, &APPCrane::DropActor, 0.1f, false);
        }

        // 드롭 후에는 Idle 상태로 => 그 자리에서 멈춰 있음
        CurrentState = EPPCraneState::Idle;
        return;
    }

    FVector dir = (TargetLocation - loc).GetSafeNormal();
    loc += dir * XYMoveSpeed * DeltaTime;
    SetActorLocation(loc);
}

void APPCrane::OnSwitchPressed()
{
    bIsSwitchPressed = true;

    // 스위치를 누른 '현재 위치'를 하강 시작점으로 기록
    StartLocation = GetActorLocation();

    CurrentState = EPPCraneState::MovingDown;
    bStopDownMovement = false;

    // 애니메이션 재생
    if (Claw && Claw->GetAnimInstance() && AM_CraneMontage)
    {
        Claw->GetAnimInstance()->Montage_Play(AM_CraneMontage, 0.3f /*재생속도*/);
        UE_LOG(LogTemp, Warning, TEXT("Trying to play montage"));
    }
}

void APPCrane::OnSwitchReleased()
{
    bIsSwitchPressed = false;
}
