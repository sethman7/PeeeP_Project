// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PPPartsDataBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Parts,	// 파츠
};

/**
 * 
 */

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
	// 아이템 타입(파츠 고정), 추후 확장해야되는데 그러면 코드 한 번 갈아엎어야됨.
	// 아마 기본 아이템 클래스가 있고 그것을 파츠가 상속받는 형식으로
	UPROPERTY(EditAnywhere, Category = Base)
	EItemType ItemType;

	// 아이템 이미지
	UPROPERTY(EditAnywhere, Category = Base)
	TObjectPtr<UTexture2D> ItemTexture;

	// 아이템 이름
	UPROPERTY(EditAnywhere, Category= Base)
	FString ItemName;
};
