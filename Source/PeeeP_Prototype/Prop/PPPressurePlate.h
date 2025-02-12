#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPPressurePlate.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPPressurePlate : public AActor
{
    GENERATED_BODY()

public:
    APPPressurePlate();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* Body;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* CollisionBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    class APPMovingPlate* MovingPlate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    class APPDoor* Door;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    class APPCrane* Crane;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    AActor* OverlappedActorRef = nullptr;
    FTimerHandle PickupTimerHandle;

    UFUNCTION()
    void DelayedPickup();
};
