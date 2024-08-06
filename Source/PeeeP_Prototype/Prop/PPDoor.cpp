// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PPDoor.h"

// Sets default values
APPDoor::APPDoor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    bIsDoorClosed = true;
    Speed = 200.0f;
    Max = 300.0f;
}


// Called when the game starts or when spawned
void APPDoor::BeginPlay()
{
    Super::BeginPlay();
    ZLocation = GetActorLocation().Z;
    if (CurveFloat)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindDynamic(this, &APPDoor::OpenDoor); // Timeline 델리게이트에 OpenDoor 함수를 바인딩
        Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
    }
}

// Called every frame
void APPDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Timeline.TickTimeline(DeltaTime);
}

void APPDoor::OnInteract()
{

    UE_LOG(LogTemp, Warning, TEXT("Interact!"));

    if (bIsDoorClosed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Open!"));
        Timeline.Play();    // Open 
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Close!"));
        Timeline.Reverse();    // Close
    }

    bIsDoorClosed = !bIsDoorClosed;    //Flip flop
}

void APPDoor::OpenDoor(float value)
{
    if (GetActorLocation().Z >= ZLocation + Max && !bIsDoorClosed) { Timeline.Stop(); }
    double NewLoc = ZLocation + (value * Speed);
    SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, NewLoc));
}