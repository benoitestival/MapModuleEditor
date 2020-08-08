// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseMapManager.h"
#include "Volumetric3DTerrainManager.generated.h"

class AVolumetricChunk;

UCLASS()
class MAPGENERATOR_API UVolumetric3DTerrainManager : public UBaseMapManager
{
	GENERATED_BODY()

	
	
public:

	UPROPERTY(EditAnywhere, Category="VolumetricManager | Grid Parameters")
	int NbrChunkX = 5;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Grid Parameters")
	int NbrChunkY = 5;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Grid Parameters")
	int NbrChunkZ = 5;
	
	UPROPERTY(EditAnywhere, Category= "VolumetricManager | Octree")
	int MaxDepth = 4;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Octree")
	float Elementize = 80.0f;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Octree")
	bool DrawOctree = false;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Octree")
	bool DrawLeaf = false;

	UPROPERTY(EditAnywhere, Category = "VolumetricManager | Octree")
	bool Toogle = false;

	UPROPERTY()
	TArray<AVolumetricChunk*> Chunks;
	
public:

	UVolumetric3DTerrainManager();
	
	virtual void Enter() override;

	virtual void Construct() override;

	virtual void Leave() override;
};
