// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../PartsData/PPPartsDataBase.h"
#include "PPPartsBase.generated.h"

//파츠 베이스 클래스
//파츠는 기본적으로 파츠에 맞는 데이터를 갖고 있음
// 해당 파츠의 생성자에서 파츠 데이터에 있는 정보들을 갖고 해당 파츠를 초기화함
// 베이스 클래스에서는 파츠 교체시 착용중인 파츠 삭제과정만 구현 나머지 파츠별 구현은 하위 클래스에서 구현


//파츠별로 개별 애니메이션이 있으니, 애니메이션 델리게이트 추가.
DECLARE_MULTICAST_DELEGATE(FOnPlayAnimationDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEEEP_PROTOTYPE_API UPPPartsBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPPPartsBase();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	void DetachParts();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	inline TObjectPtr<class UPPPartsDataBase> GetPartsData() { return PartsData; }

	//파츠 On/Off기능. 플레이어는 여러 파츠를 보유하고 있고, 다른 파츠로 변경 시 기존 파츠에 기능은 동작을 멈춰야 한다. 이 함수는 파츠별로 tick과 충돌,물리 설정을 끄거나 켜준다.
	virtual void SetPartsActive(bool flag); 

	//파츠 장착 시, 맵핑 연결과 기능 초기화 작업. 
	virtual void SetUp() {};	

	//플레이어에 부착될 소켓이름.
	FName AttachmentSocket;

	//기능 동작시킬 소켓이름.  ex) GrabParts , 그랩소켓 
	FName HitSocket;		

	//파츠별로 있을 애니메이션 연결 
	FOnPlayAnimationDelegate OnPlayAnimation;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPPPartsDataBase> PartsData;
	
	//Controller를 얻어오기 위해 Owner를 APawn으로 변경.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class APawn> Owner;

};
