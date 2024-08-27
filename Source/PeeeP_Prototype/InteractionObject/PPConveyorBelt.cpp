#include "PPConveyorBelt.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

APPConveyorBelt::APPConveyorBelt()
{
    PrimaryActorTick.bCanEverTick = true;

    BeltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BP_Conveyor"));
    RootComponent = BeltMesh;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPConveyorBelt::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APPConveyorBelt::OnOverlapEnd);

    BeltSpeed = 300.0f;
    bIsActivated = false;
    BeltLength = 1000.0f;
    Tolerance = 10.0f;
}

void APPConveyorBelt::BeginPlay()
{
    Super::BeginPlay();
}

void APPConveyorBelt::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActivated)
    {
        TArray<AActor*> OverlappingActorsCopy = OverlappingActors;

        for (AActor* OverlappingActor : OverlappingActorsCopy)
        {
            if (OverlappingActor && !HasReachedEndOfBelt(OverlappingActor))
            {
                FVector NewLocation = OverlappingActor->GetActorLocation();
                FVector Velocity = GetActorForwardVector() * BeltSpeed;
                NewLocation += Velocity * DeltaTime;
                OverlappingActor->SetActorLocation(NewLocation);

                ActorVelocities.Add(OverlappingActor, Velocity);
            }
        }
    }
}
// UPrimitiveComponent : 오브젝트의 위치, 회전, 크기 및 충돌 처리와 관련된 기능
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
        if (ActorVelocities.Contains(OtherActor))
        {
            FVector LastVelocity = ActorVelocities[OtherActor];

            if (ACharacter* Character = Cast<ACharacter>(OtherActor))
            {
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->Velocity.X = LastVelocity.X;
                    Character->GetCharacterMovement()->Velocity.Y = LastVelocity.Y;
                }
            }
            else if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()))
            {
                PrimitiveComponent->AddImpulse(LastVelocity, NAME_None, true);
            }

            ActorVelocities.Remove(OtherActor);
        }

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
