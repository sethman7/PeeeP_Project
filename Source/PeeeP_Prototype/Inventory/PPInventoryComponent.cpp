// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PPInventoryComponent.h"
#include "Engine/AssetManager.h"

// Sets default values for this component's properties
UPPInventoryComponent::UPPInventoryComponent()
{
	bWantsInitializeComponent = true;

	MaxInventorySlotNum = 6;
	MaxItemNum = 1;

	// 초기 사이즈 지정
	PartsItems.Init(nullptr, MaxInventorySlotNum);
}

void UPPInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 인벤토리 초기화
	InitInventory();
}


// Called when the game starts
void UPPInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


bool UPPInventoryComponent::AddItem(FName InItemName, int32 InItemQuantity, int32& OutItemQuantity)
{
	// 성공적으로 추가했는지에 대한 결과 반환용 변수
	bool bIsResult = false;

	// 애셋 매니저 불러오기
	UAssetManager& Manager = UAssetManager::Get();

	// 애셋 아이디 리스트 받아오기
	TArray<FPrimaryAssetId> Assets;
	// 태그 정보를 넘겨줘서 동일한 태그를 가진 애셋들의 목록을 배열로 변환받음
	Manager.GetPrimaryAssetIdList(TEXT("PPPartsData"), Assets);

	// 특정 아이템 키 생성
	FPrimaryAssetId Key;
	Key.PrimaryAssetType = TEXT("PPPartsData");
	Key.PrimaryAssetName = InItemName;

	// 해당 키의 애셋이 존재한다면
	if (Assets.Contains(Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Data"));

		// 아이템 생성
		UPPInventoryPartsItem* NewItem = NewObject<UPPInventoryPartsItem>();
		if (NewItem)
		{
			FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets.FindByKey(Key)[0]));
			if (AssetPtr.IsPending())
			{
				AssetPtr.LoadSynchronous();
			}
			UPPPartsDataBase* ItemData = Cast<UPPPartsDataBase>(AssetPtr.Get());
			if (ItemData)
			{
				NewItem->PartsData = ItemData;
				NewItem->ItemQuantity = InItemQuantity;
			}
			else
			{
				return bIsResult;
			}
		}
	}

}

void UPPInventoryComponent::UseItem(int32 InSlotIndex)
{
}

void UPPInventoryComponent::InitInventory()
{
	// 기존에 있던 정보들은 어디서 가져올 것인가?

	// 에셋 매니저 싱글톤 불러오기
	UAssetManager& Manager = UAssetManager::Get();

	// 에셋 아이디 리스트 받아오기
	TArray<FPrimaryAssetId> Assets;
	// 태그 정보를 넘겨줘서 동일한 태그를 가진 애셋들의 목록을 배열로 반환받음
	Manager.GetPrimaryAssetIdList(TEXT("PPPartsData"), Assets);

	if (Assets.Num() > 0)
	{
		TMap<int32, TPair<FName, int32>> InventoryPartsArray;
		// 테스트 블록(실제로는 저장된 파일에서 데이터를 읽어와야 함)
		{
			InventoryPartsArray.Add(1, { TEXT("GrabParts"), 1 });
		}

		for (const auto& InvItem : InventoryPartsArray)
		{
			// 특정 아이템 키 생성
			FPrimaryAssetId Key;
			Key.PrimaryAssetType = TEXT("PPPartsData");
			Key.PrimaryAssetName = InvItem.Value.Key;

			if (Assets.Contains(Key))
			{
				// 아이템 생성
				UPPInventoryPartsItem* NewItem = NewObject<UPPInventoryPartsItem>();
				if (NewItem)
				{
					FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets.FindByKey(Key)[0]);
					if (AssetPtr.IsPending())
					{
						AssetPtr.LoadSynchronous();
					}
					UPPPartsDataBase* ItemData = Cast<UPPPartsDataBase>(AssetPtr.Get());
					if (ItemData)
					{
						NewItem->PartsData = ItemData;
						NewItem->ItemQuantity = InvItem.Value.Value;
						// 아이템 넣기
						PartsItems[InvItem.Key] = NewItem;
					}
				}
			}
		}

	}
}

void UPPInventoryComponent::RemoveItem(int32 InSlotIndex)
{
}

// Called every frame
void UPPInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

