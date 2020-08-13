// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseMapManager.h"
#include "HeightMapPlanetManager.generated.h"

/**
 * 
 */

class AHeightMapPlanet;

UCLASS()
class MAPGENERATOR_API UHeightMapPlanetManager : public UBaseMapManager
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	FVector PlanetCenter;

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	int ChunkSize = 64;

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	float ElemSize = 80.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "HeightMapPlanetManager | Planet")
	AHeightMapPlanet* Planet;
	
public:

	virtual void Enter() override;

	virtual void Leave() override;

	virtual void Construct() override;

	virtual void Load() override;
};
