// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrayOctree.h"
#include "MapGenerator/Volumetric3DTerrain/Octree/OctreeUtils.h"
#include "MapGenerator/Volumetric3DTerrain/OctreeV2/ArrayOctreeNode.h"
#include "MapGenerator/Volumetric3DTerrain/Components/MarchingCubes.h"
#include "MapGenerator/Volumetric3DTerrain/Components/NoiseComponent.h"

bool UArrayOctree::IsPositionInOctree(FVector& _Position) const{
	const float OctreeSize = (Position - (Position - Size)).Size();
	const float DistanceSize = (Position - _Position).Size();
	return OctreeSize > DistanceSize;
}

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

	float ActualSize = Size;
	
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
		const FVector NodePos = Nodes[ParentNodeIndex]->NodePosition + UOctreeUtils::OctreeSizeOffsets[IndexOffsetForChildPostion] * ActualSize * 0.5f;
		
		Node->Initialize(ActualSize * 0.5f, -1.0f, CurrentDepth, i, NodePos);
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
				//TODO Need To Change the value too
				//temporary need to use trilinear interpollation
				Nodes[ParentIndex]->Value = Nodes[StartSubNodeIndex]->Value;
			}
		}
	}
	else {
		if (Nodes[ParentIndex]->State != ChildNewState) {
			Nodes[ParentIndex]->State = EState::Both;
			Nodes[ParentIndex]->Value = -1.0f;
		}
	}
	if (ParentIndex != 0 && OriginalState != Nodes[ParentIndex]->State) {
		UpdateStateToParent(Nodes[ParentIndex]->GetParentIndex(), ChildNewState);
	}
	
}

UArrayOctreeNode * UArrayOctree::GetOctreeLeafNodeAtPosition(FVector& _Position) {

	UArrayOctreeNode* Node = Nodes[0];
	while (Node->Depth != Depth) {
		Node = Nodes[Node->GetSubNodeIndex(_Position)];
	}
	return Node;
}

void UArrayOctree::MarchNode(TArray<FVector>& Vertices, TArray<int>& Triangles, TArray<FVector>& Normals, UArrayOctreeNode* Node) {

	if (Node->State == EState::Both) {
		const int StartChildIndex = Node->GetFirstSubNodeIndex();
		for (int i = StartChildIndex; i < StartChildIndex + 8; i++) {
			MarchNode(Vertices, Triangles, Normals, Nodes[i]);
		}
	}
	else {

		const int DiffSize = Node->Size / MinElemSpace;

		//Handle X and Z side
		const FVector BasePosition = Node->NodePosition + FVector(-Node->Size * 0.5f + MinElemSpace * 0.5f, Node->Size * 0.5f - MinElemSpace * 0.5f, -Node->Size * 0.5f - MinElemSpace * 0.5f) ;
		for (int i = 0; i < DiffSize - 1; i++) {
			for (int j = 0; j < DiffSize; j++) {
				
				FVector _Position = BasePosition + FVector(i * MinElemSpace, 0.0f, j * MinElemSpace);
				TArray<FCell> Cells;
				Cells.Reserve(8);

				for (int k = 0; k < 8; k++) {
					FVector __Position = Node->NodePosition + MarchingCubes::CellOffset[k] * MinElemSpace;
					if (IsPositionInOctree(__Position)) {
						FCell Cell = { GetOctreeLeafNodeAtPosition(_Position), __Position };
						Cells.Add(Cell);
					}
					else {
						UArrayOctreeNode* TempNode = NewObject<UArrayOctreeNode>();
						TempNode->Size = Node->Size;
						TempNode->NodePosition = __Position;
						TempNode->Value = UNoiseComponent::GetPerlin(__Position.X / MinElemSpace, __Position.Y / MinElemSpace, __Position.Z / MinElemSpace);
						TempNode->Depth = Node->Depth;
						TempNode->State = EState::Dead;
						FCell Cell = { TempNode, __Position };
						Cells.Add(Cell);
					}
				}

				MarchingCubes::MarchCell(Vertices, Triangles, Normals, Cells);
				
			}
		}

		//Handle Y and Z side
		const FVector BasePositionV = Node->NodePosition + FVector(Node->Size * 0.5f - MinElemSpace * 0.5f, - Node->Size * 0.5f + MinElemSpace * 0.5f, - Node->Size * 0.5f - MinElemSpace * 0.5f) ;
		for (int i = 0; i < DiffSize - 1; i++) {
			for (int j = 0; j < DiffSize; j++) {

				FVector _Position = BasePositionV + FVector(0.0f, i * MinElemSpace, j * MinElemSpace);
				TArray<FCell> Cells;
				Cells.Reserve(8);
				for (int k = 0; k < 8; k++) {
					FVector __Position = Node->NodePosition + MarchingCubes::CellOffset[k] * MinElemSpace;
					if (IsPositionInOctree(__Position)) {
						FCell Cell = { GetOctreeLeafNodeAtPosition(_Position), __Position };
						Cells.Add(Cell);
					}
					else {
						UArrayOctreeNode* TempNode = NewObject<UArrayOctreeNode>();
						TempNode->Size = Node->Size;
						TempNode->NodePosition = __Position;
						TempNode->Value = UNoiseComponent::GetPerlin(__Position.X / MinElemSpace, __Position.Y / MinElemSpace, __Position.Z / MinElemSpace);
						TempNode->Depth = Node->Depth;
						TempNode->State = EState::Dead;
						FCell Cell = { TempNode, __Position };
						Cells.Add(Cell);
					}
				}

				MarchingCubes::MarchCell(Vertices, Triangles, Normals, Cells);

			}
		}

		//Handle all Z in corner
		const FVector LastPosition = Node->NodePosition + FVector(Node->Size * 0.5f, Node->Size * 0.5f, -Node->Size * 0.5f) - MinElemSpace * 0.5f;
		for (int i = 0; i < DiffSize; i++) {
			FVector _Position = BasePositionV + FVector(0.0f, 0.0f, i * MinElemSpace);
			TArray<FCell> Cells;
			Cells.Reserve(8);
			for (int k = 0; k < 8; k++) {
				FVector __Position = Node->NodePosition + MarchingCubes::CellOffset[k] * MinElemSpace;
				if (IsPositionInOctree(__Position)) {
					FCell Cell = { GetOctreeLeafNodeAtPosition(__Position), __Position };
					Cells.Add(Cell);
				}
				else {
					UArrayOctreeNode* TempNode = NewObject<UArrayOctreeNode>();
					TempNode->Size = Node->Size;
					TempNode->NodePosition = __Position;
					TempNode->Value = UNoiseComponent::GetPerlin(__Position.X / MinElemSpace, __Position.Y / MinElemSpace, __Position.Z / MinElemSpace);
					TempNode->Depth = Node->Depth;
					TempNode->State = EState::Dead;
					FCell Cell = { TempNode, __Position };
					Cells.Add(Cell);
				}
			}

			MarchingCubes::MarchCell(Vertices, Triangles, Normals, Cells);
			
		}
		
		//if (Node->Depth == Depth) {
		//	//Final Leaf, need to find the friends of this leaf
		//	TArray<UArrayOctreeNode*> CellNodes;
		//	CellNodes.Reserve(8);
		//	//CellNodes.Add(Node);
		//	for (int i = 0; i < 8; i++) {
		//		FVector _Position = Node->NodePosition + MarchingCubes::CellOffset[i] * Node->Size;
		//		if (IsPositionInOctree(_Position)) {
		//			CellNodes.Add(GetOctreeLeafNodeAtPosition(_Position));
		//		}
		//		else {
		//			UArrayOctreeNode* TempNode = NewObject<UArrayOctreeNode>();
		//			TempNode->Size = Node->Size;
		//			TempNode->NodePosition = _Position;
		//			TempNode->Value = UNoiseComponent::GetPerlin(_Position.X / MinElemSpace, _Position.Y / MinElemSpace , _Position.Z / MinElemSpace);
		//			TempNode->Depth = Node->Depth;
		//			TempNode->State = EState::Dead;
		//			CellNodes.Add(TempNode);
		//		}
		//	}
		//}
		//else {
		//	
		//}
	}
	
}

void UArrayOctree::AlternativeMarchNode(TArray<FVector>& Vertices, TArray<int>& Triangles, TArray<FVector>& Normals, UArrayOctreeNode * Node) {

	TArray<FCell> CellNodes;
	CellNodes.Reserve(8);
	for (int i = 0; i < 8; i++) {
		FVector _Position = Node->NodePosition + MarchingCubes::CellOffset[i] * Node->Size;
		if (IsPositionInOctree(_Position)) {
			CellNodes.Add({ GetOctreeLeafNodeAtPosition(_Position), _Position });
		}
		else {
			UArrayOctreeNode* TempNode = NewObject<UArrayOctreeNode>();
			TempNode->Size = Node->Size;
			TempNode->NodePosition = _Position;
			TempNode->Value = UNoiseComponent::GetPerlin(_Position.X / MinElemSpace, _Position.Y / MinElemSpace, _Position.Z / MinElemSpace);
			TempNode->Depth = Node->Depth;
			TempNode->State = EState::Dead;
			CellNodes.Add({ TempNode, _Position});
		}
	}
	MarchingCubes::MarchCell(Vertices, Triangles, Normals, CellNodes);
	
}

void UArrayOctree::MarchNodes(TArray<FVector>& Vertices, TArray<int>& Triangles, TArray<FVector>& Normals) {
	MarchNode(Vertices, Triangles, Normals, Nodes[0]);
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
