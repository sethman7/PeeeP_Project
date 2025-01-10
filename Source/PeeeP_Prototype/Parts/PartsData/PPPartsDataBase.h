// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PPPartsDataBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Parts,	// ����
};

/**
 * 
 */

//파츠를 생성하기 위한 데이터를 보관하는 클래스
//기본적으로 파츠 매쉬, 맵핑 컨텍스트, 해당 파츠 컴포넌트 클래스에 대한 정보를 갖고있음
//해당 클래스를 상속받아서 각 개별 파츠에 대한 정보를 추가 구성한다(인풋 액션 등)
//

UCLASS()
class PEEEP_PROTOTYPE_API UPPPartsDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMesh> PartsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputMappingContext> PartsMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UPPPartsBase> PartsComponent;


public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	// ������ Ÿ��(���� ����), ���� Ȯ���ؾߵǴµ� �׷��� �ڵ� �� �� ���ƾ���ߵ�.
	// �Ƹ� �⺻ ������ Ŭ������ �ְ� �װ��� ������ ��ӹ޴� ��������
	UPROPERTY(EditAnywhere, Category = Base)
	EItemType ItemType;

	// ������ �̹���
	UPROPERTY(EditAnywhere, Category = Base)
	TObjectPtr<UTexture2D> ItemTexture;

	// ������ �̸�
	UPROPERTY(EditAnywhere, Category= Base)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAnimInstance> AnimClass;

};
