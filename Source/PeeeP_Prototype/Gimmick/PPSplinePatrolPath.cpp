// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPSplinePatrolPath.h"
#include "Components/SplineComponent.h"

// Sets default values
APPSplinePatrolPath::APPSplinePatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Root);
	Spline->Duration = 10.0f;
	Spline->bDrawDebug = true;
}

// Called when the game starts or when spawned
void APPSplinePatrolPath::BeginPlay()
{
	Super::BeginPlay();
	if (ActorToMoveClass != nullptr)
	{
		ActorToMove = GetWorld()->SpawnActor<AActor>(ActorToMoveClass, Spline->GetComponentTransform());
		if (ActorToMove != nullptr)
		{
			StartTime = GetWorld()->GetTimeSeconds();
			bCanMoveActor = true;
		}
	}
}

// Called every frame
void APPSplinePatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((ActorToMove == nullptr) || !bCanMoveActor) return;

	float CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTime;
	float Distance = Spline->GetSplineLength() * CurrentSplineTime;

	if (bIsMovingForward) //정방향
	{
		MoveActor(Distance);

		//Reach the end
		if (CurrentSplineTime > 1.0f)
		{
			if (bSplineInLoop)
			{
				bIsMovingForward = false;
				bCanMoveActor = true;
				StartTime = GetWorld()->GetTimeSeconds();
			}
			else
			{
				bCanMoveActor = false;
			}
		}
	}
	else //역방향
	{

		float ReverseDistance = Spline->GetSplineLength() - Distance;
		MoveActor(ReverseDistance);

		//Reach the end
		if (ReverseDistance <= 0.0f)
		{
			if (bSplineInLoop)
			{
				bIsMovingForward = true;
				StartTime = GetWorld()->GetTimeSeconds();
			}
			else
			{
				bCanMoveActor = false;
			}
		}
	}
}

void APPSplinePatrolPath::MoveActor(float Distance)
{
	FVector Position = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	ActorToMove->SetActorLocation(Position);


	//회전하는 트랩을 넣을 때, 이 계산과 곁쳐 회전을 못 함.
	//FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
	//ActorToMove->SetActorRotation(Rotator);
}

