// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PP_ElectricMovingPlatformTest.h"
#include "Components/InterpToMovementComponent.h"

// Sets default values
APP_ElectricMovingPlatformTest::APP_ElectricMovingPlatformTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));

	// InterpToMovement 컴포넌트 부분
	InterpToMovement = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("Interp To Movement"));
	
	bIsCharged = false;
	InterpToMovement->bAutoActivate = false;	// 자동 시작 비활성화
	FVector StartPosition = GetActorLocation();
	FInterpControlPoint Point1;
	Point1.PositionControlPoint = StartPosition;	// 시작 위치

	FInterpControlPoint Point2;
	Point2.PositionControlPoint = FVector(StartPosition.X, StartPosition.Y, StartPosition.Z + 1000.f);	// 목표 위치

	// 경로 지점 추가
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

