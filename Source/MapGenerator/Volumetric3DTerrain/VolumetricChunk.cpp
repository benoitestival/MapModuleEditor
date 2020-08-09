// Fill out your copyright notice in the Description page of Project Settings.


#include "VolumetricChunk.h"
#include "MapGenerator/Volumetric3DTerrain/Volumetric3DTerrainManager.h"
#include "MapGenerator/Volumetric3DTerrain/Octree/Octree.h"
#include "MapGenerator/Volumetric3DTerrain/OctreeV2/ArrayOctree.h"
#include "MapGenerator/Volumetric3DTerrain/Components/NoiseComponent.h"
#include "Providers/RuntimeMeshProviderStatic.h"


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
	Octree->Initialize(Axissize, Elementize, MaxDepth, Position);

	//Generate NoiseMap and insert in octree
	GenerateAndInsertNoiseMap();

	if (Manager->StandardMarch) {
		AlternativeMarchOctree();
	}
	else {
		//MarchOctree();
	}
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
					//Octree->InsertDataAtAxisValue(x, y, z, 0);
				}
			}
		}
	}
	FVector Position = Octree->Position + 40;
	Octree->InsertDataAtPosition(Position, 0, 0, 1);
	if (Manager->DrawOctree) {
		Octree->Draw();
	}
	
}

void AVolumetricChunk::MarchOctree() {

	URuntimeMeshProviderStatic* StaticProvider = NewObject<URuntimeMeshProviderStatic>(this);
	if (StaticProvider != nullptr) {

		GetRuntimeMeshComponent()->Initialize(StaticProvider);
		
		TArray<FVector> Vertices;
		Vertices.Reserve(10000);
		TArray<int> Triangles;
		Triangles.Reserve(20000);
		TArray<FColor> Colors;//No reserve because not using for now;
		TArray<FVector> Normals;
		Normals.Reserve(10000);
		TArray<FVector2D> UVs;
		//UVs.Reserve((ChunkSizeX + 1) * (ChunkSizeY + 1));
		TArray<FRuntimeMeshTangent> Tangents;//No reserve because not using for now;
		
		Octree->MarchNodes(Vertices, Triangles, Normals);
		StaticProvider->CreateSectionFromComponents(0, 0, 0, Vertices, Triangles, Normals, UVs, Colors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);

	}	
}

void AVolumetricChunk::AlternativeMarchOctree() {
	
	URuntimeMeshProviderStatic* StaticProvider = NewObject<URuntimeMeshProviderStatic>(this);

	if (Octree != nullptr && StaticProvider != nullptr) {
		UNoiseComponent::Init();

		const int StartX = ChunkX * NumElementsSide;
		const int StartY = ChunkY * NumElementsSide;
		const int StartZ = ChunkZ * NumElementsSide;

		GetRuntimeMeshComponent()->Initialize(StaticProvider);

		TArray<FVector> Vertices;
		Vertices.Reserve(10000);
		TArray<int> Triangles;
		Triangles.Reserve(20000);
		TArray<FColor> Colors;//No reserve because not using for now;
		TArray<FVector> Normals;
		Normals.Reserve(10000);
		TArray<FVector2D> UVs;
		//UVs.Reserve((ChunkSizeX + 1) * (ChunkSizeY + 1));
		TArray<FRuntimeMeshTangent> Tangents;//No reserve because not using for now;

		const float HalfMinElemSize = Elementize * 0.5f;
		for (int x = StartX; x < NumElementsSide + StartX; x++) {
			for (int y = StartY; y < NumElementsSide + StartY; y++) {
				for (int z = StartZ; z < NumElementsSide + StartZ; z++) {

					FVector _Position = FVector(x * Elementize + HalfMinElemSize, y * Elementize + HalfMinElemSize, z * Elementize + HalfMinElemSize);
					Octree->AlternativeMarchNode(Vertices, Triangles, Normals, Octree->GetOctreeLeafNodeAtPosition(_Position));

				}
			}
		}

		StaticProvider->CreateSectionFromComponents(0, 0, 0, Vertices, Triangles, Normals, UVs, Colors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);

	}
}
