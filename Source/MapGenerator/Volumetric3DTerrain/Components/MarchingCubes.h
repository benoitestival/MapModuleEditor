// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGenerator/Volumetric3DTerrain/OctreeV2/ArrayOctree.h"

class UArrayOctreeNode;

class MAPGENERATOR_API MarchingCubes
{
public:

	static FVector CellOffset[8];
	static int edgeTable[256];
	static int triTable[256][16];

public:

	static int GetCellIndex(TArray<FCell>& Cell);
	
	static void MarchCell(TArray<FVector>& Vertices, TArray<int>& Triangles, TArray<FVector>& Normals, TArray<FCell>& Cell);

	static FVector CalculateInterp(FCell& Node1, FCell& Node2, float IsoValue);
	
};
