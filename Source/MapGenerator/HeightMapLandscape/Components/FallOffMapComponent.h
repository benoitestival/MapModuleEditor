// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseLandscapeComponent.h"
#include "FallOffMapComponent.generated.h"

UENUM()
enum EFalloffType{
	Square = 0,
	Circle = 1
};


UCLASS()
class MAPGENERATOR_API UFallOffMapComponent : public UBaseLandscapeComponent
{
	GENERATED_BODY()
public:

	virtual void Initialize(ALandscapeChunk* Chunk) override;

	virtual void Remove(ULandscapeManager* Manager) override;
	
	UFUNCTION()
	void CalculateFalloff(TArray<float>& HeightMap, ALandscapeChunk* Chunk);


	UPROPERTY()
	TArray<ALandscapeChunk*> Chunks;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EFalloffType> FallOffType = EFalloffType::Square;
	
	UPROPERTY(EditAnywhere)
	float Radius = 10000;

	UPROPERTY(EditAnywhere)
	float A = 3.0f;

	UPROPERTY(EditAnywhere)
	float B = 2.2f;

	UPROPERTY(EditAnywhere)
	int ChunkFallofScale = 5;
};
