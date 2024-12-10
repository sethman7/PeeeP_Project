// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPSplinePatrolPath.h"
#include "Components/SplineComponent.h"

//에디터 상 Spline태그 설정에서 bIsLoop , ActorToMoveClass , IsClosedLoop , Duration , bCanMoveActor 제공

// Sets default values
APPSplinePatrolPath::APPSplinePatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void APPSplinePatrolPath::BeginPlay()
{
	Super::BeginPlay();

	//움직일 액터의 클래스가 설정되지 않은 경우 리턴.
	if (ActorToMoveClass == nullptr) return; 
	
	//Spline 시작 위치에 액터 스폰
	ActorToMove = GetWorld()->SpawnActor<AActor>(ActorToMoveClass, Spline->GetComponentTransform()); 

	if (ActorToMove != nullptr)
	{
		// 시작 시간 저장  GetTimeSeconds() : 게임을 시작한 후 현재까지 경과된 시간을 리턴함.
		StartTime = GetWorld()->GetTimeSeconds();

		//액터 이동 가능 여부
		bCanMoveActor = true; 

		//참일 경우 정방향으로 이동, 거짓일 경우 역방향으로 이동.
		bIsMovingForward = true; 
	}
	
}


// Called every frame
void APPSplinePatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((ActorToMove == nullptr) || !bCanMoveActor) return;

	/*
	설정된 Duration 값 동안 액터가 이동하도록 로직을 짬.
	분자 값이 계속 증가하다가 분모(duration)랑 같아질 경우 나눴을 때 CurrentSplineTime 값은 1.0이 되고,
	Distance == Spline->GetSplineLength() 이 되어 액터가 끝까지 도달했음을 의미함.
	*/
	float CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / Spline->Duration; 
	float Distance = Spline->GetSplineLength() * CurrentSplineTime;

	//정방향인 경우
	if (bIsMovingForward) 
	{
		SetActorRotationAndLocation(Distance);

		//정방향 끝에 도달한 경우.
		if (Distance >= Spline->GetSplineLength())
		{

			//무한 반복인 경우
			if (bIsLoop)
			{
				/*
				IsClosedLoop 가 참인 경우는 시작 지점과 끝 지점이 연결되어 선이 이어진 형태일 때다.
				이 경우 역방향으로 이동하지 않고 계속 정방향으로 반복 이동한다.
				IsClosedLoop 가 거짓인 경우는 시작 지점에서 끝 지점까지 갔다가 다시 돌아오는 형태로 스플라인의 경로를 왔다 갔다 할때이다. 
				이 경우 정방향->역방향 을 반복 이동한다.
				*/
				if (!Spline->IsClosedLoop()) 
				{
					//다음 역방향 이동을 위한 설정
					bIsMovingForward = false;
				}

				//경과된 시간 저장하여 초기화
				StartTime = GetWorld()->GetTimeSeconds();
				bCanMoveActor = true;
			}
			//무한 반복 아닌 경우
			else
			{
				bCanMoveActor = false;
			}
		}
	}
	//역방향인 경우
	else 
	{

		//위 코드 에서 정방향 기준으로 얻어온 Distance값을 SplineLength에 계속 빼주어 역방향 거리값을 얻어온다.
		float ReverseDistance = Spline->GetSplineLength() - Distance;
		SetActorRotationAndLocation(ReverseDistance);

		//역방향 끝에 도달한 경우.
		if (ReverseDistance <= 0.0f)
		{

			//무한 반복인 경우
			if (bIsLoop)
			{
				if (!Spline->IsClosedLoop())
				{
					//다음 정방향 이동을 위한 설정
					bIsMovingForward = true;
				}

				//경과된 시간 저장하여 초기화
				StartTime = GetWorld()->GetTimeSeconds();
				bCanMoveActor = true;
			}
			//무한 반복 아닌 경우
			else
			{
				bCanMoveActor = false;
			}
		}
	}
}

//다른 클래스에서 사용가능하도록 임시로 만든 함수
void APPSplinePatrolPath::MoveActor()
{
	bCanMoveActor = true;
}


//액터를 이동,회전시키는 로직.
void APPSplinePatrolPath::SetActorRotationAndLocation(float Distance)
{
	//Spline 선에 (Tick에서 계속 업데이트 되고 있는) Distance값을 기준으로 위치,회전값을 얻어온다.   
	FVector Position = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();

	//얻어온 값들에 플레이어를 위치시킨다.
	UStaticMeshComponent* ActorMesh = ActorToMove->FindComponentByClass<UStaticMeshComponent>();
	ActorMesh->SetWorldLocationAndRotation(Position, Rotator, true);
}

