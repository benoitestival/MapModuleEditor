// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapGenerator/Volumetric3DTerrain/OctreeV2/ArrayOctreeNode.h"
#include "ArrayOctree.generated.h"

//class UArrayOctreeNode;

UCLASS()
class MAPGENERATOR_API UArrayOctree : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	float Size;

	UPROPERTY()
	float MinElemSpace;

	UPROPERTY()
	int Depth;
	
	UPROPERTY()
	FVector Position;
	
	UPROPERTY()
	TArray<UArrayOctreeNode*> Nodes;

public:

	void Initialize(float _Size, float _MinElemSpace, int MaxDepth, FVector _Position);

	void InsertDataAtAxisValue(int x, int y, int z, float Value);
	void InsertDataAtPosition(FVector& LookingPosition, int PreviousSearchIndex, int ActualSearchIndex, float Value);

	void UpdateStateToParent(int ParentIndex, TEnumAsByte<EState> ChildNewState);
	
	int GetOctreeNumOfNodes() const;
	void Draw();
};
