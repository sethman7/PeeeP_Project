// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PPGear.h"
#include "Character/PPCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
APPGear::APPGear()
{
	// 해야할 일
	// 1. 충돌 처리할 박스 컴포넌트 추가
	// 2. 해당 박스컴포넌트와 충돌 시 넉백 기능(Overlap Begin으로)
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메쉬 컴포넌트 지정
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(false);								// 물리 영향을 받지 않는다면 반드시 false로 설정.
	Mesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));	// 플레이어의 전기 방출을 받기 위한 콜리전 프로필 세팅.

	bIsCharged = false;
}

/// <summary>
/// 액터가 충돌했을 때 호출되는 메서드
/// 현재 여러번 충돌되는 현상 발생
/// </summary>
/// <param name="MyComp"></param>
/// <param name="Other"></param>
/// <param name="OtherComp"></param>
/// <param name="bSelfMoved"></param>
/// <param name="HitLocation"></param>
/// <param name="HitNormal"></param>
/// <param name="NormalImpulse"></param>
/// <param name="Hit"></param>
void APPGear::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	UE_LOG(LogTemp, Log, TEXT("NotifyHit"));
	UE_LOG(LogTemp, Log, TEXT("Hit Noraml: %f, %f, %f"), HitNormal.X, HitNormal.Y, HitNormal.Z);

	/*if (Other)
	{
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Other->GetRootComponent());
		if (PrimitiveComponent)
		{
			UE_LOG(LogTemp, Log, TEXT("Cast Completed"));
			FVector direction = FVector(-HitNormal.X, -HitNormal.Y, 0.0f);
			PrimitiveComponent->SetPhysicsLinearVelocity(direction * 500.f);
			PrimitiveComponent->SetPhysicsLinearVelocity(FVector::UpVector * 500.f, true);
		}
	}*/

	// 플레이어와 충돌하면
	if (Other && Other->IsA(APPCharacterPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Player Hit"));
		if (!bIsCharged)
		{
			APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(Other);

			if (Player != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("Cast Completed to APPCharacterPlayer"));
				UCharacterMovementComponent* CharacterMovementComponent = Player->GetCharacterMovement();
				if (CharacterMovementComponent != nullptr)
				{
					UE_LOG(LogTemp, Log, TEXT("Succeed to Getting A UCharacterMovementComponent"));

					FVector dir = HitLocation - GetActorLocation();
					FVector normalDir = dir.GetSafeNormal2D();

					ApplyKnockback(CharacterMovementComponent, normalDir, 500.0f);
				}

				Player->TakeDamage(1.0f);
			}
		}
	}

}

// Called when the game starts or when spawned
void APPGear::BeginPlay()
{
	Super::BeginPlay();

}

/// <summary>
/// 캐릭터를 넉백시키는 메서드
/// </summary>
/// <param name="CharacterMovementComponent">넉백시킬 캐릭터의 CharacterMovementComponent</param>
/// <param name="Direction">넉백 방향</param>
/// <param name="strength">넉백 강도</param>
void APPGear::ApplyKnockback(UCharacterMovementComponent* CharacterMovementComponent, FVector direction, float strength, bool bVelocityChange)
{
	// 넉백은 스태틱 매쉬의 정중앙 앵커를 Root로 삼고 그 Root의 위치와 부딪힌 캐릭터와의 벡터를 구해서 normalization 시킨 다음 strength를 곱하여 넉백 시킬 예정
	// Character에서 z Velocity를 받아와 strength와의 차이만큼 z축 방향으로 힘을 주어 과도하게 점프하는 현상을 수정.

	double ZVelocity = CharacterMovementComponent->Velocity.Z;

	FVector knockbackDir = FVector(direction.X, direction.Y, (strength - ZVelocity)/strength);

	FVector KnockbackDirection = knockbackDir * strength;
	CharacterMovementComponent->AddImpulse(KnockbackDirection, bVelocityChange);
}

void APPGear::Charge()
{
	bIsCharged = true;
	// 충돌 비활성화
	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));

	// 물리 활성화
	Mesh->SetSimulatePhysics(true);
}


// Called every frame
void APPGear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}