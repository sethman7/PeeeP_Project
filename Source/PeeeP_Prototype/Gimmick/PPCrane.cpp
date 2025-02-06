#include "PPCrane.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

APPCrane::APPCrane()
{
    PrimaryActorTick.bCanEverTick = true;
    CraneBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraneBody"));
    RootComponent = CraneBody;

    GrabCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabCollision"));
    GrabCollision->SetupAttachment(CraneBody);
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

    // ▼ 추가
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

    // ▼ MovingDown 시, 혹은 Tick 시작 시점에 이전 위치 저장
    PreviousLocation = GetActorLocation();

    switch (CurrentState)
    {
    case EPPCraneState::Idle:
        break;

    case EPPCraneState::MovingDown:
    {
        // ▼ Overlap으로 인해 멈춰야 한다면, 더 이상 MoveCraneDown을 하지 않음
        if (!bStopDownMovement)
        {
            MoveCraneDown(DeltaTime);
        }

        if (bStopDownMovement)
        {
            // ▼ 이미 Overlap이 발생해 멈춤 → 바로 break
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
            if (GrabbedActor && !bIsHoldingObject) AttachActor(GrabbedActor);
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
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    // ▼ MovingDown 중이고, 아직 안 잡았다면
    if (CurrentState == EPPCraneState::MovingDown && !bIsHoldingObject)
    {
        // ▼ "이전 위치"로 되돌려서 즉시 멈춤
        SetActorLocation(PreviousLocation);

        // ▼ 실제로 멈추도록 flag 설정
        bStopDownMovement = true;

        // ▼ 곧바로 Attach
        AttachActor(OtherActor);

        // ▼ 상태를 MovingUp으로
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
    FVector dir = (TargetLocation - loc);
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
            GetWorldTimerManager().ClearTimer(DropTimerHandle);
            GetWorldTimerManager().SetTimer(DropTimerHandle, this, &APPCrane::DropActor, 0.1f, false);
        }
        CurrentState = EPPCraneState::Idle;
        return;
    }
    FVector dir = (TargetLocation - loc);
    dir.Normalize();
    loc += dir * XYMoveSpeed * DeltaTime;
    SetActorLocation(loc);
}

void APPCrane::SelectOverlapActor()
{
    if (bIsHoldingObject) return;
    TArray<AActor*> acts;
    GrabCollision->GetOverlappingActors(acts);
    if (acts.Num() > 0)
    {
        AActor* a = acts[0];
        if (a != this) GrabbedActor = a;
    }
    else GrabbedActor = nullptr;
}

void APPCrane::AttachActor(AActor* ActorToGrab)
{
    if (!ActorToGrab) return;
    if (bIsHoldingObject) return;
    if (UPrimitiveComponent* c = Cast<UPrimitiveComponent>(ActorToGrab->GetRootComponent()))
    {
        c->SetSimulatePhysics(false);
    }
    ActorToGrab->AttachToComponent(CraneBody, FAttachmentTransformRules::KeepWorldTransform);
    bIsHoldingObject = true;
    GrabbedActor = ActorToGrab;
}

void APPCrane::DropActor()
{
    if (GrabbedActor && bIsHoldingObject)
    {
        GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        if (UPrimitiveComponent* c = Cast<UPrimitiveComponent>(GrabbedActor->GetRootComponent()))
        {
            c->SetSimulatePhysics(true);
        }
    }
    GrabbedActor = nullptr;
    bIsHoldingObject = false;
}

void APPCrane::OnSwitchPressed()
{
    bIsSwitchPressed = true;
    if (CurrentState == EPPCraneState::Idle)
    {
        CurrentState = EPPCraneState::MovingDown;
        // ▼ 다시 내려갈 때마다 초기화
        bStopDownMovement = false;
    }
}

void APPCrane::OnSwitchReleased()
{
    bIsSwitchPressed = false;
}
