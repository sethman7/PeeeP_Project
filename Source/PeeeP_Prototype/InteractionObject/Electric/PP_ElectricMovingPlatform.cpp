// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PP_ElectricMovingPlatform.h"
#include "Components/InterpToMovementComponent.h"

// Sets default values
APP_ElectricMovingPlatform::APP_ElectricMovingPlatform()
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
	InterpToMovement->bSweep = false;				// ��ü �浹 �� ��� ���� ���

	// �÷��� �̵� �ð�, �̵� ��δ� BP���� ������ �ּ���.
}

// Called when the game starts or when spawned
void APP_ElectricMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

void APP_ElectricMovingPlatform::Charge()
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
void APP_ElectricMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

