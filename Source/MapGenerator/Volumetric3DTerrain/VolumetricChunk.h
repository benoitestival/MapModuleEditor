// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolumetricChunk.generated.h"

class UOctree;
class UArrayOctree;
class UVolumetric3DTerrainManager;

UCLASS()
class MAPGENERATOR_API AVolumetricChunk : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	int MaxDepth;

	UPROPERTY()
	int Elementize;

	UPROPERTY()
	int NumElementsSide;

	UPROPERTY()
	bool DrawOctree;

	/*UPROPERTY()
	UOctree* Octree;*/

	UPROPERTY()
	UArrayOctree* Octree;

	UPROPERTY()
	UVolumetric3DTerrainManager* Manager;
	
	// Sets default values for this actor's properties
	AVolumetricChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(UVolumetric3DTerrainManager* _Manager);
	void GenerateAndInsertNoiseMap();
};
