// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArrayOctreeNode.generated.h"


UENUM()
enum EState {
	Dead = 0,
	Alive = 1,
	Both = 2
};

UCLASS()
class MAPGENERATOR_API UArrayOctreeNode : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	float Size;

	UPROPERTY()
	float Value;

	UPROPERTY()
	int Index;

	UPROPERTY()
	int Depth;

	UPROPERTY()
	TEnumAsByte<EState> State;

	UPROPERTY()
	FVector NodePosition;

public:

	void Initialize(float _Size, float _Value, int _Depth, int _IndexOffset, FVector _NodePosition);

	int GetParentIndex() const;
	int GetFirstSubNodeIndex() const;
	int GetSubNodeIndex(FVector& LookingPosition) const;

	bool IsANodeLeaf() const;
	bool IsNodeAFinalLeaf() const;

	void DrawNode() const;
	void DrawSubNodes() const;
};
