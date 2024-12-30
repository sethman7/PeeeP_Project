// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsData/PPGrabPartsData.h"
#include "Parts/PartsComponent/PPGrabParts.h"
#include "InputMappingContext.h"

UPPGrabPartsData::UPPGrabPartsData()
{
	PartsComponentClass = UPPGrabParts::StaticClass();

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> PartsMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Parts/Grab/IMC_Grab.IMC_Grab'"));
	if (PartsMappingContextRef.Object)
	{
		PartsMappingContext = PartsMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> GrabActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Parts/Grab/IA_Grab.IA_Grab'"));
	if (GrabActionRef.Object)
	{
		GrabAction = GrabActionRef.Object;
	}
}
