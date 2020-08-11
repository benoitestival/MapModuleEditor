// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseMapManager.h"
#include "Curves/CurveFloat.h"
#include "Volumetric3DTerrainManager.generated.h"

class AVolumetricChunk;

UCLASS()
class MAPGENERATOR_API UVolumetric3DTerrainManager : public UBaseMapManager
{
	GENERATED_BODY()

	
	
public:

	UPROPERTY(EditAnywhere, Category="VolumetricManager | Grid Parameters")
	int NbrChunkX = 1;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Grid Parameters")
	int NbrChunkY = 1;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Grid Parameters")
	int NbrChunkZ = 1;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	float HeightMultiplicator = 200.0f;
	
	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	float NoiseScale = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	int NumOctaves = 3;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	float Lacunarity = 5.0f;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	float Persistance = 0.1f;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	bool ApplyZToFunc = true;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	bool Planet = false;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	float PlanetRadius = 7000.0f;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	float DistGroundToCave = 500.0f;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	int NumOfHoles = 3;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	float MinSizeHole = 500.0f;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	float MaxSizeHole = 1500.0f;
	
	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Noise")
	UCurveFloat* Fallof;
	
	UPROPERTY(EditAnywhere, Category= "VolumetricManager | Octree")
	int MaxDepth = 6;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Octree")
	float Elementize = 280.0f;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Octree")
	bool DrawOctree = false;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Octree")
	bool DrawLeaf = false;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | MarchingCubes")
	bool StandardMarch = true;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Other")
	bool Toogle = false;
	
	UPROPERTY()
	TArray<AVolumetricChunk*> Chunks;
	
public:

	UVolumetric3DTerrainManager();
	
	virtual void Enter() override;

	virtual void Construct() override;

	virtual void Leave() override;
};
