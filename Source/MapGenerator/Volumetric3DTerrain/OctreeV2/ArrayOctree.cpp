// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrayOctree.h"
#include "MapGenerator/Volumetric3DTerrain/Octree/OctreeUtils.h"
#include "MapGenerator/Volumetric3DTerrain/OctreeV2/ArrayOctreeNode.h"

void UArrayOctree::Initialize(float _Size, float _MinElemSpace, int MaxDepth, FVector _Position) {

	Position = _Position;
	Size = _Size;
	Depth = MaxDepth;
	MinElemSpace = _MinElemSpace;
	UOctreeUtils::InitStaticVariable(this);
	
	const int OctreeSize = GetOctreeNumOfNodes();
	Nodes.Reserve(OctreeSize);
	
	Nodes.Add(NewObject<UArrayOctreeNode>());
	Nodes[0]->Initialize(Size, -1.0f, 0, 0, Position);

	int CurrentDepth = 1;
	int StartIndexOfActualDepth = 1;
	int StartIndexOfPreviousDepth = 0;
	int NextDepthPass = 9;

	float ActualSize = Size * 0.5f;
	
	for (int i = 1; i < OctreeSize; i++) {

		if (i == NextDepthPass) { //Pretty dirty need to redo later
			StartIndexOfPreviousDepth += FMath::Pow(8, CurrentDepth - 1);
			CurrentDepth++;
			StartIndexOfActualDepth = NextDepthPass;
			NextDepthPass += FMath::Pow(8, CurrentDepth);
			ActualSize *= 0.5f;
		}
		
		UArrayOctreeNode* Node = NewObject<UArrayOctreeNode>();

		const int ParentNodeIndex = StartIndexOfPreviousDepth + (i - StartIndexOfActualDepth) / 8;
		const int IndexOffsetForChildPostion = (i - StartIndexOfActualDepth) % 8;
		
		Node->Initialize(ActualSize, -1.0f, CurrentDepth, i,Node[ParentNodeIndex].NodePosition + UOctreeUtils::OctreeSizeOffsets[IndexOffsetForChildPostion] * ActualSize);
		Nodes.Add(Node);
	}
}

void UArrayOctree::InsertDataAtAxisValue(int x, int y, int z, float Value) {
	const float HalfMinElemSpace = MinElemSpace * 0.5f;
	FVector _Position = FVector(x * MinElemSpace + HalfMinElemSpace, y * MinElemSpace + HalfMinElemSpace, z * MinElemSpace + HalfMinElemSpace);
	InsertDataAtPosition(_Position,0,0, Value);
}

void UArrayOctree::InsertDataAtPosition(FVector& LookingPosition, int PreviousSearchIndex, int ActualSearchIndex, float Value) {
	//TODO Check if need deeper insert
	if (Nodes[ActualSearchIndex]->Depth != Depth) {
		InsertDataAtPosition(LookingPosition, ActualSearchIndex, Nodes[ActualSearchIndex]->GetSubNodeIndex(LookingPosition), Value);
	}
	else {
		Nodes[ActualSearchIndex]->Value = Value;
		Nodes[ActualSearchIndex]->State = Value > 0.5f ? EState::Alive : EState::Dead;
		UpdateStateToParent(PreviousSearchIndex, Nodes[ActualSearchIndex]->State);
	}	
}

void UArrayOctree::UpdateStateToParent(int ParentIndex, TEnumAsByte<EState> ChildNewState) {

	const EState OriginalState = Nodes[ParentIndex]->State;
	if (Nodes[ParentIndex]->State == EState::Both) {
		if (ChildNewState != EState::Both) {
			bool NeedStateChange = true;
			const int StartSubNodeIndex = Nodes[ParentIndex]->GetFirstSubNodeIndex();
			for (int i = StartSubNodeIndex; i < StartSubNodeIndex + 8; i++) {
				if (Nodes[i]->State != ChildNewState) {
					NeedStateChange = false;
					break;
				}
			}
			if (NeedStateChange) {
				Nodes[ParentIndex]->State = ChildNewState;
			}
		}
	}
	else {
		if (Nodes[ParentIndex]->State != ChildNewState) {
			Nodes[ParentIndex]->State = EState::Both;
		}
	}
	if (ParentIndex != 0 && OriginalState != Nodes[ParentIndex]->State) {
		UpdateStateToParent(Nodes[ParentIndex]->GetParentIndex(), ChildNewState);
	}
	
}

int UArrayOctree::GetOctreeNumOfNodes() const{
	int NumNodes = 0;
	for (int i = 0; i <= Depth; i++) {
		NumNodes += FMath::Pow(8, i);
	}
	return NumNodes;
}


void UArrayOctree::Draw() {
	Nodes[0]->DrawNode();
}
