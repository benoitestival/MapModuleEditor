// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "HeightMapPlanetChunk.generated.h"

class AHeightMapPlanet;

UCLASS()
class MAPGENERATOR_API AHeightMapPlanetChunk : public ARuntimeMeshActor
{
	GENERATED_BODY()
public:

	UPROPERTY()
	AHeightMapPlanet* PlanetOwner;

	UPROPERTY()
	FVector ChunkOrientation;

	UPROPERTY()
	FVector ChunkAxisA;

	UPROPERTY()
	FVector ChunkAxisB;
	
	UPROPERTY()
	int ChunkSize;

	UPROPERTY()
	float ElemSize;

	UPROPERTY()
	float ChunkTotalHalfSize;

public:

	void Initialize(AHeightMapPlanet* _Owner, FVector _ChunkOrientation);

	void GenerateChunk();
};
