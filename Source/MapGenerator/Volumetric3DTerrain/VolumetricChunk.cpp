// Fill out your copyright notice in the Description page of Project Settings.


#include "VolumetricChunk.h"
#include "MapGenerator/Volumetric3DTerrain/Volumetric3DTerrainManager.h"
#include "MapGenerator/Volumetric3DTerrain/Octree/Octree.h"
#include "MapGenerator/Volumetric3DTerrain/OctreeV2/ArrayOctree.h"
#include "MapGenerator/Volumetric3DTerrain/Components/NoiseComponent.h"

// Sets default values
AVolumetricChunk::AVolumetricChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVolumetricChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVolumetricChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVolumetricChunk::Initialize(UVolumetric3DTerrainManager* _Manager) {
	//MaxDepth = _Manager->MaxDepth;
	//Elementize = _Manager->Elementize;
	//Manager = _Manager;
	//NumElementsSide = FMath::Pow(2, MaxDepth - 1);
	//Octree = NewObject<UOctree>();
	//const float HalfSize = NumElementsSide * Elementize * 0.5;
	//const FVector Position = GetActorLocation() + HalfSize;
	//Octree->Initialize(HalfSize, MaxDepth, Position);
	//
	////Generate NoiseMap and insert in octree
	//GenerateAndInsertNoiseMap();


	MaxDepth = _Manager->MaxDepth;
	Elementize = _Manager->Elementize;
	Manager = _Manager;
	NumElementsSide = FMath::Pow(2, MaxDepth);
	Octree = NewObject<UArrayOctree>();
	const float Axissize = NumElementsSide * Elementize;
	const float HalfSize = Axissize * 0.5;
	const FVector Position = FVector(ChunkX * Axissize, ChunkY * Axissize, ChunkZ * Axissize) + HalfSize;
	Octree->Initialize(HalfSize, Elementize, MaxDepth, Position);

	//Generate NoiseMap and insert in octree
	GenerateAndInsertNoiseMap();
	
	//TODO Generate mesh
}

void AVolumetricChunk::GenerateAndInsertNoiseMap(){


	if (Octree != nullptr) {
		UNoiseComponent::Init();

		const int StartX = ChunkX * NumElementsSide;
		const int StartY = ChunkY * NumElementsSide;
		const int StartZ = ChunkZ * NumElementsSide;
		
		for (int x = StartX; x < NumElementsSide + StartX; x++) {
			for (int y = StartY; y < NumElementsSide + StartY; y++) {
				for (int z = StartZ; z < NumElementsSide + StartZ; z++) {
					Octree->InsertDataAtAxisValue(x, y, z, UNoiseComponent::GetPerlin(x, y, z));
				}
			}
		}
	}
	if (Manager->DrawOctree) {
		Octree->Draw();
	}
	
	//if (Octree != nullptr) {

	//	UNoiseComponent::Init();

	//	//float Values[8] = {1,-1, 1, 1, -1, 1, 1 , 1};
	//	
	//	//TODO Load NoiseSettings
	//	const float HalfSize = Elementize * 0.5;
	//	int it = 0;
	//	for (int x = 0; x < NumElementsSide; x++) {
	//		for (int y = 0; y < NumElementsSide; y++) {
	//			for (int z = 0; z < NumElementsSide; z++) {

	//				FVector Position = FVector(x * Elementize + HalfSize, y * Elementize + HalfSize, z * Elementize + HalfSize);
	//				Octree->InsertValueAtPosition(Position, UNoiseComponent::GetPerlin(x, y, z) /*Values[it]*/);
	//				//it++;
	//			}
	//		}
	//	}
	//}

	//if (Manager->DrawOctree) {
	//	if (Manager->DrawLeaf) {
	//		Octree->DrawLeaf();
	//	}
	//	else {
	//		Octree->Draw();
	//	}
	//}
	
}

