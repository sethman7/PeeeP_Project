#include "PPConveyorBelt.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
APPConveyorBelt::APPConveyorBelt()
{
    PrimaryActorTick.bCanEverTick = true;

    BeltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Conveyor"));
    RootComponent = BeltMesh;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPConveyorBelt::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APPConveyorBelt::OnOverlapEnd);

    BeltSpeed = 600.0f;
    bIsActivated = false;
    BeltLength = 1000.0f; // 필요에 따라 조정
    Tolerance = 10.0f; // 필요에 따라 조정
}

// Called when the game starts or when spawned
void APPConveyorBelt::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APPConveyorBelt::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActivated)
    {
        TArray<AActor*> OverlappingActorsCopy = OverlappingActors;  // 배열 복사

        for (AActor* OverlappingActor : OverlappingActorsCopy)
        {
            if (OverlappingActor && !HasReachedEndOfBelt(OverlappingActor))
            {
                FVector NewLocation = OverlappingActor->GetActorLocation();
                NewLocation += GetActorForwardVector() * BeltSpeed * DeltaTime;
                OverlappingActor->SetActorLocation(NewLocation);
            }
        }
    }
}

void APPConveyorBelt::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        OverlappingActors.Add(OtherActor);
        bIsActivated = true;
    }
}

void APPConveyorBelt::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        OverlappingActors.Remove(OtherActor);
        if (OverlappingActors.Num() == 0)
        {
            bIsActivated = false;
        }
    }
}

bool APPConveyorBelt::HasReachedEndOfBelt(AActor* Actor) const
{
    FVector BeltEndLocation = GetActorLocation() + GetActorForwardVector() * BeltLength;
    return FVector::Dist(Actor->GetActorLocation(), BeltEndLocation) < Tolerance;
}
