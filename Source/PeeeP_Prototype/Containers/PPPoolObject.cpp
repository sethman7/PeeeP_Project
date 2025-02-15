// Fill out your copyright notice in the Description page of Project Settings.


#include "Containers/PPPoolObject.h"
#include "Interface/PPPoolableInterface.h"

DEFINE_LOG_CATEGORY(Pooling)

UPPPoolObject::UPPPoolObject()
{
	PoolingObjectClass = nullptr;
}

AActor* UPPPoolObject::GetPoolObject()
{
	if (Pool.IsEmpty())
	{
		if (PoolingObjectClass)
		{
			AActor* PoolableObject = GetWorld()->SpawnActor<AActor>(PoolingObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
			PoolableObject->SetActorHiddenInGame(true);

			IPPPoolableInterface* PoolingActor = Cast<IPPPoolableInterface>(PoolableObject);
			if (PoolingActor)
			{
				PoolingActor->SetObjectPool(this);
			}
			return PoolableObject;
		}
		else
		{
			UE_LOG(Pooling, Warning, TEXT("PoolingObjectClass is nullptr."));
			return nullptr;
		}
	}
	return Pool.Pop();
}

void UPPPoolObject::ReturnObject(AActor* InPoolObject)
{
	if (InPoolObject->Implements<UPPPoolableInterface>())
	{
		Pool.Emplace(InPoolObject);
	}
}

bool UPPPoolObject::SetPoolingObjectClass(TSubclassOf<AActor> InPoolingObjectClass)
{
	if (!PoolingObjectClass)
	{
		if (InPoolingObjectClass->ImplementsInterface(UPPPoolableInterface::StaticClass()))
		{
			PoolingObjectClass = InPoolingObjectClass;
			return true;
		}
		UE_LOG(Pooling, Warning, TEXT("InPoolingObject is not a PoolableObject."));
		return false;
	}
	UE_LOG(Pooling, Warning, TEXT("PoolingObjectClass already exists."));
	return false;
}
