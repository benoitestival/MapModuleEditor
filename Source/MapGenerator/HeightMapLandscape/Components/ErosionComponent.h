// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Objects/BaseLandscapeComponent.h"
#include "ErosionComponent.generated.h"


class ULandscapeManager;
class ALandscapeChunk;

USTRUCT()
struct FDroplet {

	GENERATED_BODY()

	FDroplet() = default;
	
	UPROPERTY()
	float X = 0.0f;

	UPROPERTY()
	float Y = 0.0f;

	UPROPERTY()
	FVector2D Direction;

	UPROPERTY()
	float OffsetToGridX = 0.0f;

	UPROPERTY()
	float OffsetToGridY = 0.0f;

	UPROPERTY()
	float Sediment = 0.0f;

	UPROPERTY()
	float Speed = 0.0f;

	UPROPERTY()
	float Water = 0.0f;
};


UCLASS()
class MAPGENERATOR_API UErosionComponent : public UBaseLandscapeComponent
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int MapSizeX;

	UPROPERTY()
	int MapSizeY;
	
	UPROPERTY()
	ULandscapeManager* Manager;
	
	UPROPERTY()
	TArray<ALandscapeChunk*> Chunks;

	UPROPERTY(EditAnywhere)
	int NumberOfChunkImpacted = 1;

	UPROPERTY(EditAnywhere)
	bool DrawDroplets = false;

	UPROPERTY(EditAnywhere)
	float ErosionRadius = 3;
	
	UPROPERTY(EditAnywhere)
	int NbrIterations = 50000;
	
	UPROPERTY(EditAnywhere)
	int MaxDropletLifeTime = 30;

	UPROPERTY(EditAnywhere)
	float Inertia = 0.05f;

	UPROPERTY(EditAnywhere)
	float Gravity = 2.0f;
	
	UPROPERTY(EditAnywhere)
	float InitialSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float InitialWater = 1.0f;

	UPROPERTY(EditAnywhere)
	float MinSedimentCapacity = 0.01f;

	UPROPERTY(EditAnywhere)
	float EvaporateSpeed = 0.01f;

	UPROPERTY(EditAnywhere)
	float DepositSpeed = 0.1f; 

	UPROPERTY(EditAnywhere)
	float ErodeSpeed = 0.1f;
	
	UPROPERTY(EditAnywhere)
	float SedimentCapacityFactor = 4;
	
public:

	UFUNCTION(BlueprintCallable, CallInEditor, Category= "Actions")
	void ErodeTerrain();

	TArray<float>& GetCellDensities	(int CellIndex, TArray<float>& OutArray);

	
	static float CalculateDropletHeight(TArray<float>& CellCorners, FDroplet& Droplet);
	static FVector2D CalculateDropletGradient(TArray<float>& CellCorners, FDroplet& Droplet);
	bool IsDropletInValid(FDroplet& Droplet) const;

	template<class T>
	FORCEINLINE static T LinearInterpolation(T& A, T& B, float C) {
		return A + (B - A) * C;
	}

	template<class T>
	FORCEINLINE static T BilinearInterpolation(T& A, T& B, T& C, T& D, float E, float F) {
		T ResultA = UErosionComponent::LinearInterpolation(A, B, E);
		T ResultB = UErosionComponent::LinearInterpolation(C, D, E);
		return UErosionComponent::LinearInterpolation(ResultA, ResultB, F);
	}
};
