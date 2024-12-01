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

		// 해당 아이템 타입(이 프로젝트에서는 파츠)의 잉ㄴ벤토리의 빈 칸 찾아 데이터 추가하기
		// 이 프로젝트에서는 아이템 타입이 파츠만 있고, 인벤토리 슬롯 또한 파츠만 있기에 그냥 인벤토리 슬롯에 하나 추가한다고 보면 된다.
		int32 Index = 0;
		for (UPPInventoryPartsItem* Item : PartsItems)
		{
			if (!IsValid(Item))
			{
				PartsItems[Index] = NewItem;	// ?
				bIsResult = true;
				OnChangeInven.Broadcast();
				break;
			}
			Index++;
		}

		if (!bIsResult)
		{
			OutItemQuantity = NewItem->ItemQuantity;
			return bIsResult;
		}
		else
		{
			return bIsResult;
		}
	}

	OutItemQuantity = InItemQuantity;
	return bIsResult;
}

void UPPInventoryComponent::UseItem(int32 InSlotIndex)
{
	/// 아이템 사용 함수에서는 아이템 슬롯의 타입과 인덱스를 받아와서 해당 인벤토리의 아이템을 사용하는 로직을 추가할 것입니다.
	/// 앞으로도 소비 아이템만 사용할 것 같지만 혹시 몰라 Switch문을 사용해 타입별로 동작을 변경해주도록 설정했습니다. (ex.무기 장착 등)
	/// 아이템 사용 함수는 실제 스탯에 반영할 일이 많을 것 같아 플레이어의 함수를 호출하는 쪽으로 구현 방향을 잡았습니다.
	/// 소비 아이템 사용 시 수량을 감소시키고, 만약 남은 수가 0이라면 해당 아이템을 소멸시켜주도록 하였습니다.
	/// https://velog.io/@apth1023/13.-12.-%EC%95%84%EC%9D%B4%ED%85%9C-%EC%9D%B8%EB%B2%A4%ED%86%A0%EB%A6%AC-%EC%8B%9C%EC%8A%A4%ED%85%9C-2
	
	if (PartsItems.IsValidIndex(InSlotIndex) && IsValid(PartsItems[InSlotIndex]))
	{
		// 수량을 줄어들게 할 것인지, 아니면 사용한다는 flag로 할 것인지는 추후 논의
		// 사실 장착 중이면 못쓰게 flag하는 것이 맞긴한데 일단 예재대로 한다고 하자.
		// PartsItems[InSlotIndex]->ItemQuantity--;
		
		// 아이템 사용
		UE_LOG(LogTemp, Warning, TEXT("Parts Item Use"));

		// 수량이 0 이하라면 소멸시켜주는 부분
		// 이 프로젝트에서는 파츠가 소멸되면 안되므로 적용하지 않음.
	}
}

void UPPInventoryComponent::RemoveItem(int32 InSlotIndex)
{
	if (PartsItems.IsValidIndex(InSlotIndex) && IsValid(PartsItems[InSlotIndex]))
	{
		PartsItems[InSlotIndex] = nullptr;
	}
}

void UPPInventoryComponent::SwapItem(int32 InprevIndex, int32 InCurrentIndex)
{
	// 추후 인벤토리 내에서 교체 기능이 있을 때 구현 예정
}

void UPPInventoryComponent::SortItem()
{
	// 추후 인벤토리 아이템 정렬 기능 구현 예정
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
					FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets.FindByKey(Key)[0]));
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



// Called every frame
void UPPInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

