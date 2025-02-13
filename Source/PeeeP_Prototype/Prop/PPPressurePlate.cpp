#include "Prop/PPPressurePlate.h"
#include "Gimmick/PPMovingPlate.h"
#include "Prop/PPDoor.h"
#include "Gimmick/PPCrane.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

APPPressurePlate::APPPressurePlate()
{
    PrimaryActorTick.bCanEverTick = true;
    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    RootComponent = Body;
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(Body);
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APPPressurePlate::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APPPressurePlate::OnOverlapEnd);
    MovingPlate = nullptr;
    Door = nullptr;
    Crane = nullptr;
}

void APPPressurePlate::BeginPlay()
{
    Super::BeginPlay();
}

void APPPressurePlate::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APPPressurePlate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;
    if (MovingPlate) MovingPlate->SetSwitchState(true);
    if (Door) Door->OnInteract();
    if (Crane)
    {
        Crane->OnSwitchPressed();
        OverlappedActorRef = OtherActor;
        GetWorld()->GetTimerManager().ClearTimer(PickupTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(PickupTimerHandle, this, &APPPressurePlate::DelayedPickup, 2.0f, false);
    }
}

void APPPressurePlate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || OtherActor == this) return;
    if (MovingPlate) MovingPlate->SetSwitchState(false);
    if (Door) Door->OnInteract();
}

void APPPressurePlate::DelayedPickup()
{
    if (!Crane) return;
    if (!IsValid(OverlappedActorRef))
    {
        Crane->OnSwitchReleased();
        return;
    }
    //Crane->AttachActor(OverlappedActorRef);
    Crane->OnSwitchReleased();
    OverlappedActorRef = nullptr;
}
