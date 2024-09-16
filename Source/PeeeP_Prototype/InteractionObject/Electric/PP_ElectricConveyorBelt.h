#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PP_ElectricConveyorBelt.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APP_ElectricConveyorBelt : public AActor, public IPPElectricObjectInterface
{
    GENERATED_BODY()

public:
    APP_ElectricConveyorBelt();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UInterpToMovementComponent> InterpToMovement;

    bool bIsCharged;
    void Charge() override;

    void ReverseDirection();

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* BeltMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BeltSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BeltLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Tolerance;

private:
    bool bIsActivated;

    UPROPERTY()
    TArray<AActor*> OverlappingActors;

    UPROPERTY()
    TMap<AActor*, FVector> ActorVelocities;  // 물체의 속도를 저장

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    bool HasReachedEndOfBelt(AActor* Actor) const;
};
