// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnumsUtils.generated.h"

UENUM(BlueprintType, Blueprintable)
enum EMapType {
	ProceduralLandscape = 0,
	ProceduralHMPlanet = 1,
	Volumetric3DLandscape = 2
};

UENUM()
enum NoiseType { Value, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise, Cubic, CubicFractal };

UENUM()
enum FractalType { FBM, Billow, RigidMulti };

UCLASS()
class MAPGENERATOR_API UEnumsUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static TArray<EMapType> DifferentMapTypes;
};
