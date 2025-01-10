// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsData/PPPartsDataBase.h"

FPrimaryAssetId UPPPartsDataBase::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("PPPartsData", GetFName());
}
