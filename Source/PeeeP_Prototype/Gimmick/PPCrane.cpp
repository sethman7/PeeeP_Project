#include "PPCrane.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

APPCrane::APPCrane()
{
    PrimaryActorTick.bCanEverTick = true;

    // ▼ UStaticMesh 대신 USkeletalMeshComponent 생성
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
}

void APPCrane::BeginPlay()
{
    Super::BeginPlay();
    OriginalLocation = GetActorLocation();
}

void APPCrane::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // MovingDown 시점 혹은 Tick 시작 시점에 이전 위치 저장
    PreviousLocation = GetActorLocation();

    switch (CurrentState)
    {
    case EPPCraneState::Idle:
        // 아무 것도 하지 않음
        break;

    case EPPCraneState::MovingDown:
    {
        // Overlap으로 인해 멈춰야 한다면, 더 이상 아래로 이동하지 않음
        if (!bStopDownMovement)
        {
            MoveCraneDown(DeltaTime);
        }

        if (bStopDownMovement)
        {
            break;
        }

        float dist = OriginalLocation.Z - GetActorLocation().Z;
        if (dist >= DownDistance)
        {
            FVector pos = OriginalLocation;
            pos.Z -= DownDistance;
            SetActorLocation(pos);

            SelectOverlapActor();

            CurrentState = EPPCraneState::Waiting;
            WaitingTimer = 0.f;
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
        float z = GetActorLocation().Z;
        if (z >= OriginalLocation.Z)
        {
            SetActorLocation(OriginalLocation);

            CurrentState = EPPCraneState::MovingX;
            TargetLocation = OriginalLocation + FVector(150.f, 0.f, 0.f);
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

    // MovingDown 상태에서만 오버랩 즉시 잡기
    if (CurrentState == EPPCraneState::MovingDown && !bIsHoldingObject)
    {
        // 이전 위치로 되돌려서 멈춤
        SetActorLocation(PreviousLocation);
        bStopDownMovement = true;

        // 곧바로 Attach
        AttachActor(OtherActor);

        // 상태 전환
        CurrentState = EPPCraneState::MovingUp;
    }
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

    FVector dir = TargetLocation - loc;
    dir.Normalize();
    loc += dir * XYMoveSpeed * DeltaTime;
    SetActorLocation(loc);
}

void APPCrane::MoveOnY(float DeltaTime)
{
    FVector loc = GetActorLocation();
    float dist = (TargetLocation - loc).Size();
    if (dist <= AcceptableRadius)
    {
        loc = TargetLocation;
        SetActorLocation(loc);

        if (bIsHoldingObject && GrabbedActor)
        {
            // 0.1초 후에 떨어뜨리기
            GetWorldTimerManager().ClearTimer(DropTimerHandle);
            GetWorldTimerManager().SetTimer(DropTimerHandle, this, &APPCrane::DropActor, 0.1f, false);
        }
        CurrentState = EPPCraneState::Idle;
        return;
    }

    FVector dir = TargetLocation - loc;
    dir.Normalize();
    loc += dir * XYMoveSpeed * DeltaTime;
    SetActorLocation(loc);
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
    if (!ActorToGrab || bIsHoldingObject) return;

    if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(ActorToGrab->GetRootComponent()))
    {
        RootComp->SetSimulatePhysics(false);
    }

    // ▼ SkeletalMesh인 Claw에 붙임
    ActorToGrab->AttachToComponent(Claw, FAttachmentTransformRules::KeepWorldTransform);

    bIsHoldingObject = true;
    GrabbedActor = ActorToGrab;
}

void APPCrane::DropActor()
{
    if (GrabbedActor && bIsHoldingObject)
    {
        GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

        if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(GrabbedActor->GetRootComponent()))
        {
            RootComp->SetSimulatePhysics(true);
        }
    }

    GrabbedActor = nullptr;
    bIsHoldingObject = false;
}

void APPCrane::OnSwitchPressed()
{
    bIsSwitchPressed = true;

    CurrentState = EPPCraneState::MovingDown;
    bStopDownMovement = false;

    // 2) 컴포넌트와 몽타주가 유효하면 재생
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
