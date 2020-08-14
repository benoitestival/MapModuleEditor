// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeightMapPlanet.generated.h"

class UHeightMapPlanetManager;
class AHeightMapPlanetChunk;

UCLASS()
class MAPGENERATOR_API AHeightMapPlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeightMapPlanet();

	UPROPERTY()
	FVector PlanetCenter;

	UPROPERTY()
	int ChunkSize;

	UPROPERTY()
	float ElemSize;
	
	UPROPERTY()
	UHeightMapPlanetManager* Manager;

	UPROPERTY()
	TArray<AHeightMapPlanetChunk*> Chunks;

public:

	static TArray<FVector> ChunksOrientation;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize();
	void Remove();
};
