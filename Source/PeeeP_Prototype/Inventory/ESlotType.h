// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESlotType: uint8
{
	ST_None,
	ST_InventoryParts,		// 인벤토리(파츠)
	ST_InventoryConsumable,	// 인벤토리(소비)
	ST_InventoryOther,		// 인벤토리(기타)
};
