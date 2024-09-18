// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PPLobbyActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
APPLobbyActor::APPLobbyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Characters/PeePCharacter/Mat/MI_PeepCharcater.MI_PeepCharcater'"));
	if (MaterialRef.Object)
	{
		Material = MaterialRef.Object;
	}

	Mesh->SetMaterial(0, Material);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	SceneCapture->SetupAttachment(RootComponent);
	SceneCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTargetRef(TEXT("/Script/Engine.TextureRenderTarget2D'/Game/UI/MenuHUD/RT_LobbyActorCapture.RT_LobbyActorCapture'"));
	if (RenderTargetRef.Object)
	{
		RenderTarget = RenderTargetRef.Object;
		SceneCapture->TextureTarget = RenderTarget;
	}


}

// Called when the game starts or when spawned
void APPLobbyActor::BeginPlay()
{
	Super::BeginPlay();

	SceneCapture->ShowOnlyActors.Add(this);
}

// Called every frame
void APPLobbyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMaterialInstance* APPLobbyActor::GetMaterial()
{
	return Material.Get();
}

