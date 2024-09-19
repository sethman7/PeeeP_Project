// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPLobbyActor.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPLobbyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPLobbyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class UMaterialInstance> Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SceneCapture)
	TObjectPtr<class USceneCaptureComponent2D> SceneCapture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SceneCapture)
	TObjectPtr<class UTextureRenderTarget2D> RenderTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UMaterialInstance* GetMaterial();

	void ChangeEmessive(float Type);
};
