// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PP_ElectricMovingPlatformTest.h"
#include "Components/InterpToMovementComponent.h"

// Sets default values
APP_ElectricMovingPlatformTest::APP_ElectricMovingPlatformTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메쉬 컴포넌트 지정
	// 움직이는 플랫폼의 경우 중력의 영향을 받지 않으므로 SetSimulatePhysics를 false로 설정해야 함.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(false);								// 중력의 영향을 받지 않는다면 반드시 false로 설정.
	Mesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));	// 플레이어의 전기 방출을 받기 위한 콜리전 프로필 세팅.

	// InterpToMovement 컴포넌트 부분
	InterpToMovement = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("Interp To Movement"));
	
	// InterpToMovement 컴포넌트 세팅 부분
	// 나중에 경로 지점을 추가할 때는 코드에서 추가하는 것이 아닌 BP에서 추가하는 방법이 더 편할 듯
	// 코드에서 지정 시 재활용이 어려움
	bIsCharged = false;								// 중복으로 켜지는 것을 방지하기 위한 flag입니다. 추후 발판 작동 시간 등 조절하려면 상황에 맞게 수정해 주세요.
	InterpToMovement->bAutoActivate = false;		// 자동 시작 비활성화

	// 경로 지점 설정 부분
	FVector StartPosition = GetActorLocation();
	FInterpControlPoint Point1;
	Point1.PositionControlPoint = StartPosition;	// 시작 위치

	FInterpControlPoint Point2;
	Point2.PositionControlPoint = FVector(StartPosition.X, StartPosition.Y, StartPosition.Z + 1000.f);	// 목표 위치

	// 경로 지점 추가 부분
	InterpToMovement->ControlPoints.Add(Point1);
	InterpToMovement->ControlPoints.Add(Point2);

	InterpToMovement->Duration = 5.0f;	// 이동 시간
	InterpToMovement->bSweep = false;	// 물체 충돌 시 경로 결정 방법

}

// Called when the game starts or when spawned
void APP_ElectricMovingPlatformTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void APP_ElectricMovingPlatformTest::Charge()
{
	if (bIsCharged)	// 이미 차징이 되었으므로 중복은 필요 없음
	{
		UE_LOG(LogTemp, Log, TEXT("This Platform is already Charged!"));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Platform Charged!"));
	bIsCharged = true;
	
	InterpToMovement->Activate();	// 자동 활성화가 false이므로 수동으로 활성화
}

// Called every frame
void APP_ElectricMovingPlatformTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

