// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PP_ElectricMovingPlatformTest.h"
#include "Components/InterpToMovementComponent.h"

// Sets default values
APP_ElectricMovingPlatformTest::APP_ElectricMovingPlatformTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �޽� ������Ʈ ����
	// �����̴� �÷����� ��� �߷��� ������ ���� �����Ƿ� SetSimulatePhysics�� false�� �����ؾ� ��.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(false);								// �߷��� ������ ���� �ʴ´ٸ� �ݵ�� false�� ����.
	Mesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));	// �÷��̾��� ���� ������ �ޱ� ���� �ݸ��� ������ ����.

	// InterpToMovement ������Ʈ �κ�
	InterpToMovement = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("Interp To Movement"));
	
	// InterpToMovement ������Ʈ ���� �κ�
	// ���߿� ��� ������ �߰��� ���� �ڵ忡�� �߰��ϴ� ���� �ƴ� BP���� �߰��ϴ� ����� �� ���� ��
	// �ڵ忡�� ���� �� ��Ȱ���� �����
	bIsCharged = false;								// �ߺ����� ������ ���� �����ϱ� ���� flag�Դϴ�. ���� ���� �۵� �ð� �� �����Ϸ��� ��Ȳ�� �°� ������ �ּ���.
	InterpToMovement->bAutoActivate = false;		// �ڵ� ���� ��Ȱ��ȭ

	// ��� ���� ���� �κ�
	FVector StartPosition = GetActorLocation();
	FInterpControlPoint Point1;
	Point1.PositionControlPoint = StartPosition;	// ���� ��ġ

	FInterpControlPoint Point2;
	Point2.PositionControlPoint = FVector(StartPosition.X, StartPosition.Y, StartPosition.Z + 1000.f);	// ��ǥ ��ġ

	// ��� ���� �߰� �κ�
	InterpToMovement->ControlPoints.Add(Point1);
	InterpToMovement->ControlPoints.Add(Point2);

	InterpToMovement->Duration = 5.0f;	// �̵� �ð�
	InterpToMovement->bSweep = false;	// ��ü �浹 �� ��� ���� ���

}

// Called when the game starts or when spawned
void APP_ElectricMovingPlatformTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void APP_ElectricMovingPlatformTest::Charge()
{
	if (bIsCharged)	// �̹� ��¡�� �Ǿ����Ƿ� �ߺ��� �ʿ� ����
	{
		UE_LOG(LogTemp, Log, TEXT("This Platform is already Charged!"));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Platform Charged!"));
	bIsCharged = true;
	
	InterpToMovement->Activate();	// �ڵ� Ȱ��ȭ�� false�̹Ƿ� �������� Ȱ��ȭ
}

// Called every frame
void APP_ElectricMovingPlatformTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

