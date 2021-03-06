// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMapComponent.h"
#include "BaseLandscapeComponent.generated.h"

/**
 * 
 */
class ALandscapeChunk;
class ULandscapeManager;

UCLASS(Abstract)
class MAPGENERATOR_API UBaseLandscapeComponent : public UBaseMapComponent
{
	GENERATED_BODY()

public:

	virtual void Initialize(ALandscapeChunk* Chunk){};

	virtual void Remove(ULandscapeManager* Manager){};
};
