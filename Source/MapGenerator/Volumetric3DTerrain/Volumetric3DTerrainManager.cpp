// Fill out your copyright notice in the Description page of Project Settings.


#include "Volumetric3DTerrainManager.h"
#include "Engine/World.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "MapGenerator/Volumetric3DTerrain/VolumetricChunk.h"

UVolumetric3DTerrainManager::UVolumetric3DTerrainManager() {
	MapType = EMapType::Volumetric3DLandscape;
}


void UVolumetric3DTerrainManager::Enter() {
	Super::Enter();
	UE_LOG(LogTemp, Warning, TEXT("Volumetric Terrain : Enter"));

	const float ElementAxisNum = FMath::Pow(2, MaxDepth);
	
	const float XSize = ElementAxisNum * Elementize;
	const float YSize = ElementAxisNum * Elementize;
	const float ZSize = ElementAxisNum * Elementize;

	for (int x = 0; x < NbrChunkX; x++) {
		for (int y = 0; y < NbrChunkY; y++) {
			for (int z = 0; z < NbrChunkZ; z++) {

				FVector Location = FVector(x * XSize, y * YSize, z * ZSize);
				AVolumetricChunk* Chunk = UMapEditorUtils::GetActualWorld()->SpawnActor<AVolumetricChunk>(AVolumetricChunk::StaticClass(), Location, FRotator(0.0f, 0.0f, 0.0f));
				Chunk->ChunkX = x;
				Chunk->ChunkY = y;
				Chunk->ChunkZ = z;
				Chunk->Initialize(this);
				Chunks.Add(Chunk);
			}
		}
	}
	
}

void UVolumetric3DTerrainManager::Leave() {
	UE_LOG(LogTemp, Warning, TEXT("Volumetric Terrain : Leave"));
	//Super::Leave();
	if (Chunks.Num() > 0) {
		for (auto Chunk : Chunks) {
			UMapEditorUtils::GetActualWorld()->DestroyActor(Chunk); //TODO Find Another way to get world
		}
		Chunks.Empty();
	}
	Initialized = false;
}

void UVolumetric3DTerrainManager::Construct() {
	UE_LOG(LogTemp, Warning, TEXT("Volumetric Terrain : Construct"));
	for (auto Chunk : Chunks) {
		Chunk->Initialize(this);
	}
	
}
