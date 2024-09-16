#include "InteractionObject/Electric/PP_ElectricConveyorBelt.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InterpToMovementComponent.h"

// Sets default values
APP_ElectricConveyorBelt::APP_ElectricConveyorBelt()
{
    PrimaryActorTick.bCanEverTick = true;

    // Mesh 및 TriggerBox 설정
    BeltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BP_Conveyor"));
    RootComponent = BeltMesh;
    BeltMesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APP_ElectricConveyorBelt::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APP_ElectricConveyorBelt::OnOverlapEnd);

    // 기본 속성 초기화
    BeltSpeed = 300.0f;
    bIsActivated = false;
    BeltLength = 1000.0f;
    Tolerance = 10.0f;
    bIsCharged = false; // 초기에는 충전되지 않음

    // InterpToMovementComponent 초기화
    InterpToMovement = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("InterpToMovementComponent"));
    InterpToMovement->bSweep = false;
}

void APP_ElectricConveyorBelt::BeginPlay()
{
    Super::BeginPlay();

    // InterpToMovement가 제대로 설정되었는지 확인
    if (!InterpToMovement)
    {
        UE_LOG(LogTemp, Error, TEXT("InterpToMovement is not initialized properly!"));
        return;
    }
}

void APP_ElectricConveyorBelt::Tick(float DeltaTime)
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

void APP_ElectricConveyorBelt::Charge()
{
    if (bIsCharged)	// 이미 충전된 상태라면 중복으로 처리하지 않음
    {
        UE_LOG(LogTemp, Log, TEXT("This Platform is already Charged!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Platform Charged!"));
    bIsCharged = true;

    // 방향을 반전
    ReverseDirection();
}

void APP_ElectricConveyorBelt::ReverseDirection()
{
    // 벨트의 이동 속도를 음수로 반전
    BeltSpeed = -BeltSpeed;
}

// 충돌 발생 시 처리
void APP_ElectricConveyorBelt::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        OverlappingActors.Add(OtherActor);
        bIsActivated = true;
    }
}

// 충돌 끝날 때 처리
void APP_ElectricConveyorBelt::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

// 벨트 끝에 도달했는지 확인
bool APP_ElectricConveyorBelt::HasReachedEndOfBelt(AActor* Actor) const
{
    FVector BeltEndLocation = GetActorLocation() + GetActorForwardVector() * BeltLength;
    return FVector::Dist(Actor->GetActorLocation(), BeltEndLocation) < Tolerance;
}
