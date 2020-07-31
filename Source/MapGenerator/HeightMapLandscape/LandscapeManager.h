// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MapGenerator/Objects/BaseMapManager.h"
#include "Curves/CurveFloat.h"
#include "Materials/MaterialInterface.h"
#include "LandscapeManager.generated.h"

class ALandscapeChunk;
class UBaseLandscapeComponent;

UENUM()
enum ETerrainType{
	InfinteLand = 0,
	Island = 1
};

USTRUCT()
struct FProcLandscapeCache {

	GENERATED_BODY()

	FProcLandscapeCache()
		: FProcLandscapeCache(-1,-1){
	}
	
	FProcLandscapeCache(int _X, int _Y)
		: NbrX(_X), NbrY(_Y){
	}
	
	UPROPERTY()
	int NbrX = 1;

	UPROPERTY()
	int NbrY = 1;
};


UCLASS()
class MAPGENERATOR_API ULandscapeManager : public UBaseMapManager
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category="LandScapeManager | Grid Parameters")
	int NbrChunkX = 5;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Grid Parameters")
	int NbrChunkY = 5;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Grid Parameters")
	int ChunkSizeX = 64;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Grid Parameters")
	int ChunkSizeY = 64;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Grid Parameters")
	float CellResolution = 80.0f;

	/*UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	TEnumAsByte<ETerrainType> TerrainType = ETerrainType::InfinteLand;*/
	
	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float HeightMultiplicator = 30000.0f;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	int OctavesNumber = 5;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float Lacunarity = 5.0f;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float Persistance = 0.1f;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float Scale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float SeaLevel = -1000000.0f;
	
	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	UCurveFloat* Fallof;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	int IslandSizeInChunks = 2;

	UPROPERTY(EditAnywhere, Instanced, Category="LandScapeManager | Components")
	TArray<UBaseLandscapeComponent*> MapComponents;

	UPROPERTY()
	TArray<UBaseLandscapeComponent*> ComponentsCache;
	
	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Mesh")
	UMaterialInterface* Material;
	
	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Mesh")
	bool EnableCollision = false;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Mesh")
	bool AddNormals = true;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Mesh")
	bool AddUVs = false;

	UPROPERTY(EditAnywhere)
	bool UseElevationScale = true;
	
	UPROPERTY(EditAnywhere, Category = "LandScapeManager | OtherParameters")
	bool DrawGrid = false;
	
	UPROPERTY(EditAnywhere, Category = "LandScapeManager | OtherParameters")
	bool ShowNoiseLog = false;

	UPROPERTY(VisibleAnywhere, Category = "LandScapeManager | Chunks")
	TArray<ALandscapeChunk*> Chunks;

	UPROPERTY()
	FProcLandscapeCache Cache;

	bool Initialized = false;
	
public:

	ULandscapeManager();

	virtual void Enter() override;

	virtual void Leave() override;

	virtual void Construct() override;

	virtual void Load() override;

	/*UFUNCTION(BlueprintCallable, CallInEditor, Category= "LandScapeManager | Actions")
	void ErodeTerrain();*/
	
	FVector2D FindSizeXYOfChunkArray(TArray<ALandscapeChunk*>& _Chunks);
};
