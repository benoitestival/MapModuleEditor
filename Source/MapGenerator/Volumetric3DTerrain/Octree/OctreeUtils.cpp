// Fill out your copyright notice in the Description page of Project Settings.


#include "OctreeUtils.h"

FVector UOctreeUtils::OctreePos = FVector();
float UOctreeUtils::Size = 0.0f;
int UOctreeUtils::OctreeMaxDepth = 0;
UArrayOctree* UOctreeUtils::Octree = nullptr;
TArray<FVector> UOctreeUtils::OctreeSizeOffsets = {
	FVector(-0.5f,-0.5f,-0.5f),
	FVector(-0.5f,0.5f,-0.5f),
	FVector(0.5f,-0.5f,-0.5f),
	FVector(0.5f,0.5f,-0.5f),
	FVector(-0.5f,-0.5f,0.5f),
	FVector(-0.5f,0.5f,0.5f),
	FVector(0.5f,-0.5f,0.5f),
	FVector(0.5f,0.5f,0.5f)
};

TArray<FColor> UOctreeUtils::OctreeColorDepth = {};

bool UOctreeUtils::InsertBitCommon[2] = {
	false,
	true
};

bool UOctreeUtils::InsertBitNotCommon[2] = {
	true,
	false
};

void UOctreeUtils::InitStaticVariable(UOctree * _Octree) {
	if (_Octree != nullptr) {
		OctreePos = _Octree->Position;
		Size = _Octree->Size;
		OctreeMaxDepth = _Octree->Depth;
	}
	OctreeColorDepth.Empty();
	for (int i = 0; i < OctreeMaxDepth; i++) {
		OctreeColorDepth.Add(FColor::MakeRandomColor());
	}
}

void UOctreeUtils::InitStaticVariable(UArrayOctree* _Octree) {
	if (_Octree != nullptr) {
		OctreePos = _Octree->Position;
		Size = _Octree->Size;
		OctreeMaxDepth = _Octree->Depth;
		Octree = _Octree;
	}
	OctreeColorDepth.Empty();
	OctreeColorDepth.Add(FColor::Blue);
	OctreeColorDepth.Add(FColor::Red);
	OctreeColorDepth.Add(FColor::Green);
	OctreeColorDepth.Add(FColor::Cyan);
	OctreeColorDepth.Add(FColor::Yellow);
	OctreeColorDepth.Add(FColor::Orange);
	/*for (int i = 0; i <= OctreeMaxDepth; i++) {
		OctreeColorDepth.Add(FColor::MakeRandomColor());
	}*/
}


FVector UOctreeUtils::GetOctreePosition() {
	return OctreePos;
}

int UOctreeUtils::GetOctreeMaxDepth() {
	return OctreeMaxDepth;
}

float UOctreeUtils::GetOctreeSize() {
	return Size;
}

UArrayOctree* UOctreeUtils::GetOctree() {
	return Octree;
}
