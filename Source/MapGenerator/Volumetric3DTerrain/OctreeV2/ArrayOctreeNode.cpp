// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrayOctreeNode.h"
#include "MapGenerator/Volumetric3DTerrain/Octree/OctreeUtils.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "DrawDebugHelpers.h"

void UArrayOctreeNode::Initialize(float _Size, float _Value, int _Depth, int _IndexOffset, FVector _NodePosition) {
	Size = _Size;
	Value = _Value;
	Depth = _Depth;
	Index = _IndexOffset;
	State = EState::Dead;
	NodePosition = _NodePosition;
}

int UArrayOctreeNode::GetParentIndex() const {
	int PreviousStartIndex = 0;
	int DepthStartIndex = 1;
	for (int i = 1; i < Depth; i++) {
		DepthStartIndex += FMath::Pow(8, i);
		PreviousStartIndex += FMath::Pow(8, i - 1);
	}
	return PreviousStartIndex + (Index - DepthStartIndex) / 8;
}

int UArrayOctreeNode::GetFirstSubNodeIndex() const{
	int DepthStartIndex = 0;
	int NextStartIndex = 1;
	for (int i = 1; i <= Depth; i++) {
		DepthStartIndex += FMath::Pow(8, i - 1);
		NextStartIndex += FMath::Pow(8, i);
	}
	return NextStartIndex + (Index - DepthStartIndex) * 8;
}

int UArrayOctreeNode::GetSubNodeIndex(FVector& LookingPosition) const {
	int OffsetSubNodesindex = LookingPosition.Z > NodePosition.Z ? 4 : 0;
	OffsetSubNodesindex += LookingPosition.X > NodePosition.X ? 2 : 0;
	OffsetSubNodesindex += LookingPosition.Y > NodePosition.Y ? 1 : 0;
	return GetFirstSubNodeIndex() + OffsetSubNodesindex;	
}

bool UArrayOctreeNode::IsANodeLeaf() const {
	return IsNodeAFinalLeaf() || State == EState::Alive || State == EState::Dead;
}

bool UArrayOctreeNode::IsNodeAFinalLeaf() const {
	return Depth == UOctreeUtils::GetOctreeMaxDepth();
}

void UArrayOctreeNode::DrawNode() const{
	const float Thickness = 15 - Depth * 2;

	const FVector PostionLeftBackBottom = NodePosition - Size + Thickness;
	const FVector PostionRightBackBottom = FVector(NodePosition.X - Size + Thickness, NodePosition.Y + Size - Thickness, NodePosition.Z - Size + Thickness);
	const FVector PostionLeftForBottom = FVector(NodePosition.X + Size - Thickness, NodePosition.Y - Size + Thickness, NodePosition.Z - Size + Thickness);
	const FVector PostionRightForBottom = FVector(NodePosition.X + Size - Thickness, NodePosition.Y + Size - Thickness, NodePosition.Z - Size + Thickness);
	const FVector PostionLeftBackBackTop = FVector(NodePosition.X - Size + Thickness, NodePosition.Y - Size + Thickness, NodePosition.Z + Size - Thickness);
	const FVector PostionRightBackTop = FVector(NodePosition.X - Size + Thickness, NodePosition.Y + Size - Thickness, NodePosition.Z + Size - Thickness);
	const FVector PostionLeftForTop = FVector(NodePosition.X + Size - Thickness, NodePosition.Y - Size + Thickness, NodePosition.Z + Size - Thickness);
	const FVector PostionRightForTop = NodePosition + Size - Thickness;

	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBottom, PostionRightBackBottom, UOctreeUtils::OctreeColorDepth[Depth], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBottom, PostionLeftForBottom, UOctreeUtils::OctreeColorDepth[Depth], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionRightBackBottom, PostionRightForBottom, UOctreeUtils::OctreeColorDepth[Depth ], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftForBottom, PostionRightForBottom, UOctreeUtils::OctreeColorDepth[Depth ], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBackTop, PostionRightBackTop, UOctreeUtils::OctreeColorDepth[Depth ], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBackTop, PostionLeftForTop, UOctreeUtils::OctreeColorDepth[Depth], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionRightBackTop, PostionRightForTop, UOctreeUtils::OctreeColorDepth[Depth ], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftForTop, PostionRightForTop, UOctreeUtils::OctreeColorDepth[Depth ], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBottom, PostionLeftBackBackTop, UOctreeUtils::OctreeColorDepth[Depth ], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionRightBackBottom, PostionRightBackTop, UOctreeUtils::OctreeColorDepth[Depth ], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftForBottom, PostionLeftForTop, UOctreeUtils::OctreeColorDepth[Depth ], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionRightForBottom, PostionRightForTop, UOctreeUtils::OctreeColorDepth[Depth], false, 60.0f, 0, Thickness);

	if (!IsANodeLeaf()) {
		DrawSubNodes();
	}
}

void UArrayOctreeNode::DrawSubNodes() const{
	const int StartSubNodeIndex = GetFirstSubNodeIndex();
	for (int i = StartSubNodeIndex; i < StartSubNodeIndex + 8; i++) {
		UOctreeUtils::GetOctree()->Nodes[i]->DrawNode();
	}
}
