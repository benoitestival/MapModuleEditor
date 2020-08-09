// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Volumetric3DTerrain/Octree/Octree.h"
#include "MapGenerator/Volumetric3DTerrain/OctreeV2/ArrayOctree.h"

/**
 * 
 */
class MAPGENERATOR_API UOctreeUtils
{

public:
	static TArray<FVector> OctreeSizeOffsets;
	static TArray<FColor> OctreeColorDepth;
	static bool InsertBitCommon[2];
	static bool InsertBitNotCommon[2];
	
private:
	static int OctreeMaxDepth;
	static float Size;
	static FVector OctreePos;
	static UArrayOctree* Octree;

	
public:
	
	static void InitStaticVariable(UOctree* _Octree);
	static void InitStaticVariable(UArrayOctree* _Octree);
	static FVector GetOctreePosition();
	static int GetOctreeMaxDepth();
	static float GetOctreeSize();
	static UArrayOctree* GetOctree();
	
	
};
