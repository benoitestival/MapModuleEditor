// Fill out your copyright notice in the Description page of Project Settings.


#include "OctreeNode.h"
#include "OctreeUtils.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "DrawDebugHelpers.h"

void UOctreeNode::Initialize(float _Size, float _Value, int _Depth, FVector _NodePosition) {
	Size = _Size;
	Value = _Value;
	Depth = _Depth;
	//State = ENodeState::Dead;
	NodePosition = _NodePosition;
	if (!IsNodeAFinalLeaf()) {
		SubNodes.Reserve(8);
	}
}

bool UOctreeNode::IsNodeAFinalLeaf() const {
	return Depth == UOctreeUtils::GetOctreeMaxDepth();
}

bool UOctreeNode::HasChildren() const {
	return SubNodes.Num() > 0;
}

bool UOctreeNode::IsNodeALeaf() const{
	return IsNodeAFinalLeaf() || !HasChildren();
}

//int UOctreeNode::GetDepth() const{
//	return UOctreeUtils::GetOctreeSize() / Size;
//}

bool UOctreeNode::CanCollapse() const {
	bool CanCollapse = true;
	/*if (State == ENodeState::Both) {
		CanCollapse = false;
	}*/
	return CanCollapse;
}

bool UOctreeNode::NeedDeaperInsertForValue(int SubPartIndex, float _Value) {

	bool NeedDeaperInsert = true;
	/*const int Sign = FMath::Sign(_Value);
	const int Index = (Sign + 1) * 0.5f;
	if (HasChildren()) {
		SubNodes[SubPartIndex]
	}
	else {
		NeedDeaperInsert = FMath::Sign(Value) != Sign;
	}*/
	//TODO
	return NeedDeaperInsert;
}

//void UOctreeNode::UpdateStateToParentNode(TEnumAsByte<ENodeState> ChildNewState) {
//
//	const ENodeState OriginalState = State;
//	if (State == ENodeState::Both) {
//		if (ChildNewState != ENodeState::Both) {
//			bool NeedStateChange = true;
//			for (auto SubNode : SubNodes) {
//				if (SubNode->State != ChildNewState) {
//					NeedStateChange = false;
//					break;
//				}
//			}
//
//			if (NeedStateChange) {
//				State = ChildNewState;
//			}
//		}
//	}
//	else {
//		if (State != ChildNewState) {
//			State = ENodeState::Both;
//		}
//	}
//	if (ParentNode != nullptr && OriginalState != State) {
//		ParentNode->UpdateStateToParentNode(State);
//	}
//}

int UOctreeNode::GetSubNodeIndex(FVector& LookingPosition) const {
	int index = 0;
	index += LookingPosition.Z > NodePosition.Z ? 4 : 0;
	index += LookingPosition.X > NodePosition.X ? 2 : 0;
	index += LookingPosition.Y > NodePosition.Y ? 1 : 0;
	return index;
}

UOctreeNode * UOctreeNode::GetActualDeapestNodeForPos(FVector& LookingPosition) {
	UOctreeNode* Node = nullptr;
	if (!IsNodeALeaf()) {		
		Node = SubNodes[GetSubNodeIndex(LookingPosition)];
	}
	else {
		Node = this;
	}
	return Node;
}

UOctreeNode * UOctreeNode::GetActualDeapestNodeRecursivelyForPos(FVector& LookingPosition) {
	UOctreeNode* Node = nullptr;
	if (!IsNodeALeaf()) {
		Node = SubNodes[GetSubNodeIndex(LookingPosition)];
		if (Node != nullptr && !Node->IsNodeALeaf()) {
			Node = Node->GetActualDeapestNodeRecursivelyForPos(LookingPosition);
		}
	}
	else {
		Node = this;
	}
	return Node;
}

TArray<UOctreeNode*>& UOctreeNode::GetLeafNodes(TArray<UOctreeNode*>& OutArray) {
	if (!IsNodeALeaf()) {
		for (auto& SubNode : SubNodes) {
			SubNode->GetLeafNodes(OutArray);
		}
	}
	else {
		OutArray.Add(this);
	}
	return OutArray;
}

void UOctreeNode::InsertValueForPosition(FVector& Position, float _Value) {

	const int SubPartIndex = GetSubNodeIndex(Position);
	if (!IsNodeAFinalLeaf() && NeedDeaperInsertForValue(SubPartIndex, _Value)) {
		if (!HasChildren()) {
			SubDivideNode();
		}
		SubNodes[SubPartIndex]->InsertValueForPosition(Position, _Value);
		if (CanCollapse()) {
			CollapseSubNodes(_Value);
		}
	}
	else {
		Value = _Value;
	/*	State = _Value > 0.5f ? ENodeState::Alive : ENodeState::Dead;
		ParentNode->UpdateStateToParentNode(State);*/
	}
}

void UOctreeNode::CollapseSubNodes(float NewValue) {
	Value = NewValue;	
	SubNodes.Empty();
}

void UOctreeNode::SubDivideNode() {
	if (!IsNodeAFinalLeaf() && !HasChildren()) {
		for (auto& Offset : UOctreeUtils::OctreeSizeOffsets) {
			UOctreeNode* Node = NewObject<UOctreeNode>();
			Node->Initialize(Size * 0.5, -1.0f, Depth + 1, NodePosition + Offset * Size);
			Node->ParentNode = this;
			SubNodes.Add(Node);
		}
	}
}

void UOctreeNode::SubDivideNodeRecursively() {
	SubDivideNode();
	for (auto SubNode : SubNodes) {
		SubDivideNodeRecursively();
	}
}

void UOctreeNode::DrawNode() {

	const float Thickness = 15 - Depth * 2;
	
	const FVector PostionLeftBackBottom = NodePosition - Size + Thickness;
	const FVector PostionRightBackBottom = FVector(NodePosition.X - Size + Thickness, NodePosition.Y + Size - Thickness, NodePosition.Z - Size + Thickness);
	const FVector PostionLeftForBottom = FVector(NodePosition.X + Size - Thickness, NodePosition.Y - Size + Thickness, NodePosition.Z - Size + Thickness);
	const FVector PostionRightForBottom = FVector(NodePosition.X + Size - Thickness, NodePosition.Y + Size - Thickness, NodePosition.Z - Size + Thickness);
	const FVector PostionLeftBackBackTop = FVector(NodePosition.X - Size + Thickness, NodePosition.Y - Size + Thickness, NodePosition.Z + Size - Thickness);
	const FVector PostionRightBackTop = FVector(NodePosition.X - Size + Thickness, NodePosition.Y + Size - Thickness, NodePosition.Z + Size - Thickness);
	const FVector PostionLeftForTop = FVector(NodePosition.X + Size - Thickness, NodePosition.Y - Size + Thickness, NodePosition.Z + Size - Thickness);
	const FVector PostionRightForTop = NodePosition + Size - Thickness;

	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBottom, PostionRightBackBottom, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBottom, PostionLeftForBottom, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionRightBackBottom, PostionRightForBottom, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftForBottom, PostionRightForBottom, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0,Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBackTop, PostionRightBackTop, UOctreeUtils::OctreeColorDepth[Depth -1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBackTop, PostionLeftForTop, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionRightBackTop, PostionRightForTop, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftForTop, PostionRightForTop, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftBackBottom, PostionLeftBackBackTop, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionRightBackBottom, PostionRightBackTop, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionLeftForBottom, PostionLeftForTop, UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);
	DrawDebugLine(UMapEditorUtils::GetActualWorld(), PostionRightForBottom, PostionRightForTop,  UOctreeUtils::OctreeColorDepth[Depth - 1], false, 60.0f, 0, Thickness);

	if (!IsNodeALeaf()) {
		DrawSubNodes();
	}
}

void UOctreeNode::DrawLeaf() {

	if (!IsNodeALeaf()) {
		for (auto SubNode : SubNodes) {
			SubNode->DrawLeaf();
		}
	}
	else {
		DrawNode();
	}
}


void UOctreeNode::DrawSubNodes() {
	for (auto SubNode : SubNodes) {
		SubNode->DrawNode();
	}
}
