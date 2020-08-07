// Fill out your copyright notice in the Description page of Project Settings.


#include "Octree.h"
#include "OctreeUtils.h"
#include "OctreeNode.h"


void UOctree::Initialize(float _Size, int MaxDepth, FVector _Position) {
	Position = _Position;
	Size = _Size;
	Depth = MaxDepth;
	UOctreeUtils::InitStaticVariable(this);
	if (SourceNode == nullptr) {
		SourceNode = NewObject<UOctreeNode>();
		SourceNode->Initialize(Size, -1.0f, 1, Position);
	}
}

bool UOctree::IsPositionInOctree(FVector& _Position) const {
	//TODO
	return true;
}

void UOctree::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Initialize(Size, Depth, Position);
}

UOctreeNode* UOctree::GetSubNode(FVector& _Position) const{
	UOctreeNode* Node = nullptr;
	if (SourceNode != nullptr && IsPositionInOctree(_Position)) {
		Node = SourceNode->GetActualDeapestNodeForPos(_Position);
	}
	return Node;
}

UOctreeNode * UOctree::GetOctreeNode(FVector& _Position) const{
	UOctreeNode* Node = nullptr;
	if (SourceNode != nullptr && IsPositionInOctree(_Position)) {
		Node = SourceNode->GetActualDeapestNodeRecursivelyForPos(_Position);
	}
	return Node;
}

TArray<UOctreeNode*>& UOctree::GetOctreeLeafNodes(TArray<UOctreeNode*>& OutArray) const{
	return UOctree::GetNodeLeafNodes(SourceNode, OutArray);
}

TArray<UOctreeNode*>& UOctree::GetNodeLeafNodes(UOctreeNode* Node, TArray<UOctreeNode*>& OutArray) {
	if (Node != nullptr) {
		return Node->GetLeafNodes(OutArray);
	}
	return OutArray;
}

void UOctree::InsertValueAtPosition(FVector& _Position, float Value) const{
	if (IsPositionInOctree(_Position)) {
		SourceNode->InsertValueForPosition(_Position, Value);
	}
}

void UOctree::Draw() const{
	if (SourceNode != nullptr) {
		SourceNode->DrawNode();
	}
}

void UOctree::DrawLeaf() const {
	if (SourceNode != nullptr) {
		SourceNode->DrawLeaf();
	}
}
