// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "MapGenerator/Noise/FastNoise.h"
#include "MapGenerator/HeightMapLandscape/LandscapeManager.h"
#include "MapGenerator/Utils/EnumsUtils.h"
#include "LandscapeChunk.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNoiseValueCalculated, int, X, int, Y, float&, Noise)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNoiseValueHeightUpdate, int, X, int, Y, float&, Noise)
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNoiseMapFinishGeneration, const TArray<float>&, HeightM)

USTRUCT()
struct FNoiseSettings {

	GENERATED_BODY()
	
	FNoiseSettings() = default;
	
	UPROPERTY()
	float HeightMultiplicator = 0;

	UPROPERTY()
	int NumOctaves = 1;

	UPROPERTY()
	float Persistance = 1;

	UPROPERTY()
	float Lacunarity = 1;
	
	UPROPERTY()
	int XOffset = 0;

	UPROPERTY()
	int YOffset = 0;

	UPROPERTY()
	float Scale = 1;

	UPROPERTY()
	float SeaLevel;

	UPROPERTY()
	UCurveFloat* Fallof;

	UPROPERTY()
	bool UseFallof;

	UPROPERTY()
	TEnumAsByte<ETerrainType> Type;

	UPROPERTY()
	int IslandSize;

	UPROPERTY()
	TEnumAsByte<NoiseType> TypeOfNoise;

	UPROPERTY()
	TEnumAsByte<FractalType> TypeOfFractal;
	
	FastNoise NoiseModule;

	void LoadNoiseSettings(int X, int Y, ULandscapeManager* _Manager) {
		HeightMultiplicator = _Manager->HeightMultiplicator;
		Persistance = _Manager->Persistance;
		NumOctaves = _Manager->OctavesNumber;
		Lacunarity = _Manager->Lacunarity;
		Scale = _Manager->Scale;
		XOffset = _Manager->ChunkSizeX * X;
		YOffset = _Manager->ChunkSizeY * Y;
		SeaLevel = _Manager->SeaLevel;
		UseFallof = _Manager->UseFallof;
		Fallof = _Manager->Fallof;
		Type = _Manager->TerrainType;
		IslandSize = _Manager->IslandSizeInChunks;
	}
};

UCLASS()
class MAPGENERATOR_API ALandscapeChunk : public ARuntimeMeshActor
{
	GENERATED_BODY()
public:

	//typedef float
	
	UPROPERTY()
	int ChunkX;

	UPROPERTY()
	int ChunkY;

	UPROPERTY()
	int ChunkSizeX;

	UPROPERTY()
	int ChunkSizeY;
	
	UPROPERTY()
	FNoiseSettings NoiseSettings;
	
	UPROPERTY()
	ULandscapeManager* Manager;

	UPROPERTY()
	TArray<float> HeightMap;

	UPROPERTY(BlueprintAssignable)
	FNoiseValueCalculated NoiseValueCalculated;

	UPROPERTY(BlueprintAssignable)
	FNoiseValueHeightUpdate NoiseValueHeightUpdate;

	
public:
	
	// Sets default values for this actor's properties
	ALandscapeChunk();

	void Init();
	
	void GenerateMesh();
	void GenerateNoiseMap();

	float ApplyHeightMultiplicator(int x, int y);

	float CalculateFallofMap(int x, int y);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
