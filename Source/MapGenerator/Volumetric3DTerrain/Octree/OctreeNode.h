// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OctreeNode.generated.h"


class UOctree;

//UENUM()
//enum ENodeState{
//	Dead = 0,
//	Alive = 1,
//	Both = 2
//};


UCLASS()
class MAPGENERATOR_API UOctreeNode : public UObject{
	
	GENERATED_BODY()
public:

	UPROPERTY()
	float Size;

	UPROPERTY()
	float Value;

	UPROPERTY()
	int Depth;

	UPROPERTY()
	FVector NodePosition;

	//UPROPERTY()
	//TEnumAsByte<ENodeState> State;
	
	UPROPERTY()
	UOctreeNode* ParentNode;
	
	UPROPERTY()
	TArray<UOctreeNode*> SubNodes;

public:

	void Initialize(float _Size, float _Value, int _Depth, FVector _NodePosition);
	
	bool IsNodeAFinalLeaf() const;
	bool HasChildren() const;
	bool IsNodeALeaf() const;
	bool CanCollapse() const;
	bool NeedDeaperInsertForValue(int SubPartIndex, float _Value);
	/*int GetDepth() const;*/
	
	UOctreeNode* GetActualDeapestNodeForPos(FVector& LookingPosition);
	UOctreeNode* GetActualDeapestNodeRecursivelyForPos(FVector& LookingPosition);
	TArray<UOctreeNode*>& GetLeafNodes(TArray<UOctreeNode*>& OutArray);
	int GetSubNodeIndex(FVector& LookingPosition) const;
	//void UpdateStateToParentNode(TEnumAsByte<ENodeState> ChildNewState);
	
	void InsertValueForPosition(FVector& Position, float _Value);
	void CollapseSubNodes(float NewValue);
	
	void SubDivideNode();
	void SubDivideNodeRecursively();
	
	void DrawNode();
	void DrawLeaf();
	void DrawSubNodes();
	
};
