// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseMapManager.h"
#include "Volumetric3DTerrainManager.generated.h"

/**
 * 
 */
UCLASS()
class MAPGENERATOR_API UVolumetric3DTerrainManager : public UBaseMapManager
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	int testVolumetric = 0;
	
public:

	UVolumetric3DTerrainManager();
	
	virtual void Enter() override;

	virtual void Construct() override;

	virtual void Leave() override;
};
