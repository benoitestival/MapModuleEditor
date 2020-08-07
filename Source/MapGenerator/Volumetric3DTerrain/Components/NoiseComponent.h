// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseVolumetricTerrainComponent.h"
#include "MapGenerator/Noise/FastNoise.h"
#include "NoiseComponent.generated.h"

/**
 * 
 */
UCLASS()
class MAPGENERATOR_API UNoiseComponent : public UBaseVolumetricTerrainComponent
{
	GENERATED_BODY()
public:

	static FastNoise NoiseModule;

	static void Init();
	
	static float GetPerlin(int x, int y, int z);

	
};
