// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapGenerator/Utils/EnumsUtils.h"
#include "BaseMapManager.generated.h"

class UBaseMapComponent;
/**
 * 
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew,BlueprintType)
class MAPGENERATOR_API UBaseMapManager : public UObject
{
	GENERATED_BODY()
public:
	
	bool Initialized = false;
	EMapType MapType;

	
	virtual void Enter();

	virtual void Construct();

	virtual void Load();
	
	virtual void Leave();
};
