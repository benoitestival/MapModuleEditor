// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MapGenerator/Objects/BaseMapManager.h"
#include "Curves/CurveFloat.h"
#include "LandscapeManager.generated.h"

class ALandscapeChunk;

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
	int NbrChunkX = 1;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Grid Parameters")
	int NbrChunkY = 1;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Grid Parameters")
	int ChunkSizeX = 64;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Grid Parameters")
	int ChunkSizeY = 64;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Grid Parameters")
	float CellResolution = 80.0f;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float HeightMultiplicator;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float OctavesNumber;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float Lacunarity;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float Persistance;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float Scale;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	float SeaLevel;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	UCurveFloat* Fallof;
	
	UPROPERTY(EditAnywhere, Category = "LandScapeManager | Noise")
	bool UseFallof;
	
	UPROPERTY(EditAnywhere, Category = "LandScapeManager | OtherParameters")
	bool EnableCollision = false;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | OtherParameters")
	bool DrawGrid = false;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | OtherParameters")
	bool AddNormals = false;

	UPROPERTY(EditAnywhere, Category = "LandScapeManager | OtherParameters")
	bool AddUVs = false;
	
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
	
	FVector2D FindSizeXYOfChunkArray(TArray<ALandscapeChunk*>& _Chunks);
};
