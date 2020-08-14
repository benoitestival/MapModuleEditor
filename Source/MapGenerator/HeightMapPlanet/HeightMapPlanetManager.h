// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseMapManager.h"
#include "Curves/CurveFloat.h"
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

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	TArray<float> Offsets;

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	float PlanetRadius = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	float Scale = 0.5f;

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	float height = 500.0f;

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	float Persistance = 0.1f;

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	float Lacunarity = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	bool ShowCube = false;

	UPROPERTY(EditAnywhere, Category = "HeightMapPlanetManager | Planet")
	UCurveFloat* Curve;
	
	UPROPERTY(VisibleAnywhere, Category = "HeightMapPlanetManager | Planet")
	AHeightMapPlanet* Planet;
	
public:

	UHeightMapPlanetManager();
	
	virtual void Enter() override;

	virtual void Leave() override;

	virtual void Construct() override;

	virtual void Load() override;
};
