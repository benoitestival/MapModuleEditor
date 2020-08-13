// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseVolumetricTerrainComponent.h"
#include "MapGenerator/Noise/FastNoise.h"
#include "Curves/CurveFloat.h"
#include "NoiseComponent.generated.h"



USTRUCT()
struct FHole {

	GENERATED_BODY()
	
	UPROPERTY()
	FVector Position;

	UPROPERTY()
	float Radius;
	
};

UCLASS()
class MAPGENERATOR_API UNoiseComponent : public UBaseVolumetricTerrainComponent
{
	GENERATED_BODY()
public:

	static FastNoise NoiseModule;

	static float Scale;
	static bool ApplyZ;
	static float Height;
	static bool Planet;
	static bool SPlitPlanet;
	static FVector VectorForSplit;
	static bool Flat;
	static float PlanetRadius;
	static float MinElemSize;
	static UCurveFloat* Curve;
	static int NumOctaves;
	static float Persistence;
	static float Lacunarity;
	static float DistGroundCave;
	static TArray<FHole> Holes;
	static float HoleRadiusMin;
	static float HoleRadiusMax;

	
	static void Init();
	
	static float GetPerlin(float x, float y, float z);

	
};
