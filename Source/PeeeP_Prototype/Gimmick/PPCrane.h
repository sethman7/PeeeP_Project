#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPCrane.generated.h"

UENUM(BlueprintType)
enum class EPPCraneState : uint8
{
    Idle,
    MovingDown,
    Waiting,
    MovingUp,
    MovingX,
    MovingY
};

UCLASS()
class PEEEP_PROTOTYPE_API APPCrane : public AActor
{
    GENERATED_BODY()

public:
    APPCrane();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crane")
    class USkeletalMeshComponent* Claw;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crane")
    class USkeletalMeshComponent* Claw2;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crane")
    class UBoxComponent* GrabCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane")
    float MoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane")
    float DownDistance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crane")
    EPPCraneState CurrentState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane")
    bool bIsSwitchPressed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane")
    float WaitTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane")
    float XYMoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane")
    float AcceptableRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* AM_CraneMontage;

    UPROPERTY()
    FVector StartLocation;

protected:
    FVector OriginalLocation;
    bool bIsHoldingObject;
    AActor* GrabbedActor;
    float WaitingTimer;
    FTimerHandle DropTimerHandle;
    FVector TargetLocation;

    // 추가된 멤버 변수
    FVector PreviousLocation;
    bool bStopDownMovement;

    UFUNCTION()
    void OnGrabOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    void MoveCraneDown(float DeltaTime);
    void MoveCraneUp(float DeltaTime);
    void MoveOnX(float DeltaTime);
    void MoveOnY(float DeltaTime);

public:
    UFUNCTION(BlueprintCallable)
    void SelectOverlapActor();

    UFUNCTION(BlueprintCallable)
    void AttachActor(AActor* ActorToGrab);

    UFUNCTION(BlueprintCallable)
    void DropActor();

    UFUNCTION(BlueprintCallable)
    void OnSwitchPressed();

    UFUNCTION(BlueprintCallable)
    void OnSwitchReleased();
};
