// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Octree.generated.h"

class UOctreeNode;

UCLASS()
class MAPGENERATOR_API UOctree : public UObject{
	
	GENERATED_BODY()
public:
	
	UPROPERTY()
	float Size;

	UPROPERTY()
	int Depth;
	
	UPROPERTY()
	FVector Position;

	UPROPERTY()
	UOctreeNode* SourceNode;

public:

	void Draw() const;
	void DrawLeaf() const;
	
	void Initialize(float _Size, int MaxDepth, FVector _Position);
	bool IsPositionInOctree(FVector& _Position) const;

	UOctreeNode* GetSubNode(FVector& _Position) const;
	UOctreeNode* GetOctreeNode(FVector& _Position) const;
	TArray<UOctreeNode*>& GetOctreeLeafNodes(TArray<UOctreeNode*>& OutArray) const;
	static TArray<UOctreeNode*>& GetNodeLeafNodes(UOctreeNode* Node, TArray<UOctreeNode*>& OutArray);

	void InsertValueAtPosition(FVector& _Position, float Value) const;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;;
};
