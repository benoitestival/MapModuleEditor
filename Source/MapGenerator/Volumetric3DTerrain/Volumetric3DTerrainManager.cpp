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
	/*if (!Initialized) {
		Initialized = true;*/
		AVolumetricChunk* Chunk = UMapEditorUtils::GetActualWorld()->SpawnActor<AVolumetricChunk>(AVolumetricChunk::StaticClass(), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		Chunk->Initialize(this);
		Chunks.Add(Chunk);
	//}
	
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
