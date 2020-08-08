// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeManager.h"
#include "Engine/World.h"
#include "MapGenerator/HeightMapLandscape/LandscapeChunk.h"
#include "MapGenerator/Objects/BaseLandscapeComponent.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "UObject/ConstructorHelpers.h" 
#include "Kismet/GameplayStatics.h"

// Sets default values

ULandscapeManager::ULandscapeManager() {
	MapType = EMapType::ProceduralLandscape;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TerrainMaterial(TEXT("Material'/Game/Material/TerrainMaterial.TerrainMaterial'"));
	if (TerrainMaterial.Succeeded()) {
		//Material = TerrainMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FalloffCurve(TEXT("CurveFloat'/Game/Helpers/FallofCurve.FallofCurve'"));
	if (TerrainMaterial.Succeeded()) {
		Fallof = FalloffCurve.Object;
	}

}


void ULandscapeManager::Enter() {
	UE_LOG(LogTemp, Warning, TEXT("Landscape : Enter"));

	if (!Initialized) {

		Initialized = true;
		const float XSize = ChunkSizeX * CellResolution;
		const float YSize = ChunkSizeY * CellResolution;

		Cache = { NbrChunkX, NbrChunkY };

		Chunks.Empty();
		Chunks.Reserve(NbrChunkX * NbrChunkY);

		for (int x = 0; x < NbrChunkX; x++) {
			for (int y = 0; y < NbrChunkY; y++) {

				FVector Location = FVector(x * XSize, y * YSize, 0.0f);
				ALandscapeChunk* LandscapeChunk = UMapEditorUtils::GetActualWorld()->SpawnActor<ALandscapeChunk>(ALandscapeChunk::StaticClass(), Location, FRotator(0.0f,0.0f,0.0f));
				LandscapeChunk->ChunkX = x;
				LandscapeChunk->ChunkY = y;
				LandscapeChunk->Init();
				Chunks.Add(LandscapeChunk);
			}
		}
	}
}

void ULandscapeManager::Leave() {
	
	UE_LOG(LogTemp, Warning, TEXT("Landscape : Leave"));
	if (Chunks.Num() > 0) {
		for (auto Chunk : Chunks) {
			UMapEditorUtils::GetActualWorld()->DestroyActor(Chunk); //TODO Find Another way to get world
		}
		Chunks.Empty();
	}
	Initialized = false;
	//Super::Leave();
}

void ULandscapeManager::Construct() {
	
	UE_LOG(LogTemp, Warning, TEXT("Landscape : Construct"));

	TArray<UBaseLandscapeComponent*> Temp;
	for (auto Component : ComponentsCache) {
		if (!MapComponents.Contains(Component)) {
			if (Component != nullptr) {
				Component->Remove(this);
				Temp.Add(Component);
			}
		}
	}
	for (auto Component : Temp) {
		ComponentsCache.Remove(Component);
	}
	for (auto Component : MapComponents) {
		if (!ComponentsCache.Contains(Component)) {
			ComponentsCache.Add(Component);
		}
	}
	
	const int DesiredNumberOfChunks = NbrChunkX * NbrChunkY;
	if (DesiredNumberOfChunks == Chunks.Num()) {
		//Other property change need to refresh all map
		for (auto Chunk : Chunks) {
			Chunk->Init();
		}
	}
	else {
		//The number of chunk has changed only change impacted chunk
		TArray<ALandscapeChunk*> NewChunksArray;
		NewChunksArray.Reserve(NbrChunkX * NbrChunkY);

		const int CopyX = NbrChunkX > Cache.NbrX ? Cache.NbrX : NbrChunkX;
		const int CopyY = NbrChunkY > Cache.NbrY ? Cache.NbrY : NbrChunkY;

		const float XSize = ChunkSizeX * CellResolution;
		const float YSize = ChunkSizeY * CellResolution;

		for (int x = 0; x < CopyX; x++) {
			//Copy All chunks within CopyX and Copy scope
			for (int y = 0; y < CopyY; y++) {
				const int CopyIndex = x * Cache.NbrY + y;
				NewChunksArray.Add(Chunks[CopyIndex]);
			}
			//If the new array is bigger in Y size need to add missing elements (possibly 0 iteration in this under scope)
			for (int y = CopyY; y < NbrChunkY; y++) {
				FVector Location = FVector(x * XSize, y * YSize, 0.0f);
				ALandscapeChunk* LandscapeChunk = UMapEditorUtils::GetActualWorld()->SpawnActor<ALandscapeChunk>(ALandscapeChunk::StaticClass(), Location, FRotator(0.0f, 0.0f, 0.0f));
				LandscapeChunk->ChunkX = x;
				LandscapeChunk->ChunkY = y;
				LandscapeChunk->Init();
				NewChunksArray.Add(LandscapeChunk);
			}
			//If the new array is shorter in Y need to delete chunks(possibly 0 iteration in this under scope)
			for (int y = CopyY; y < Cache.NbrY; y++) {
				const int DeleteIndex = x * Cache.NbrY + y;
				UMapEditorUtils::GetActualWorld()->DestroyActor(Chunks[DeleteIndex]);
			}
		}

		//If the new array is bigger in X size need to add missing elements (possibly 0 iteration in this under scope)
		for (int x = CopyX; x < NbrChunkX; x++) {
			for (int y = 0; y < NbrChunkY; y++) {
				FVector Location = FVector(x * XSize, y * YSize, 0.0f);
				ALandscapeChunk* LandscapeChunk = UMapEditorUtils::GetActualWorld()->SpawnActor<ALandscapeChunk>(ALandscapeChunk::StaticClass(), Location, FRotator(0.0f, 0.0f, 0.0f));
				LandscapeChunk->ChunkX = x;
				LandscapeChunk->ChunkY = y;
				LandscapeChunk->Init();
				NewChunksArray.Add(LandscapeChunk);
			}
		}

		//If the new array is shorter in X need to free memory of unusaged cells (possibly 0 iteration in this under scope)
		for (int x = CopyX; x < Cache.NbrX; x++) {
			for (int y = 0; y < Cache.NbrY; y++) {
				const int DeleteIndex = x * Cache.NbrY + y;
				UMapEditorUtils::GetActualWorld()->DestroyActor(Chunks[DeleteIndex]);
			}
		}

		Chunks = NewChunksArray;
		Cache.NbrX = NbrChunkX;
		Cache.NbrY = NbrChunkY;

	}
}

void ULandscapeManager::Load() {

	Initialized = true;
	TArray<AActor*> Out = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(UMapEditorUtils::GetActualWorld(), ALandscapeChunk::StaticClass(), Out);
	if (Out.Num() > 0) {
		int MaxX = 0;
		int MaxY = 0;
		TArray<ALandscapeChunk*> ChunksTemp;
		ChunksTemp.Reserve(10 * 10);//Random reserve
		for (auto Actor : Out) {
			ALandscapeChunk* Chunk = Cast<ALandscapeChunk>(Actor);
			if (Chunk->ChunkX > MaxX) {
				MaxX = Chunk->ChunkX;
			}
			if (Chunk->ChunkY > MaxY) {
				MaxY = Chunk->ChunkY;
			}
			ChunksTemp.Add(Chunk);
		}			
		NbrChunkX = MaxX + 1;
		NbrChunkY = MaxY + 1;
		Cache.NbrX = NbrChunkX;
		Cache.NbrY = NbrChunkY;
		
		Chunks.Init(nullptr,NbrChunkX * NbrChunkY);
		for (auto Chunk : ChunksTemp) {
			const int index = Chunk->ChunkX * NbrChunkY + Chunk->ChunkY;
			Chunks[index] = Chunk;
		}
		ChunksTemp.Empty();
		
		ChunkSizeX = Chunks[0]->ChunkSizeX;
		ChunkSizeY = Chunks[0]->ChunkSizeY;
		//AddNormals = Chunks[0]->UseNormals;
	}
}

FVector2D ULandscapeManager::FindSizeXYOfChunkArray(TArray<ALandscapeChunk*>& _Chunks) {
	return FVector2D();
}


//void ULandscapeManager::ErodeTerrain() {
//	
//}
