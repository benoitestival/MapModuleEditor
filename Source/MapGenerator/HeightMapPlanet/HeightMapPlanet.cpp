// Fill out your copyright notice in the Description page of Project Settings.


#include "HeightMapPlanet.h"
#include "Engine/World.h"
#include "MapGenerator/HeightMapPlanet/HeightMapPlanetManager.h"
#include "HeightMapPlanetChunk.h"
#include "MapGenerator/Utils/MapEditorUtils.h"

TArray<FVector> AHeightMapPlanet::ChunksOrientation = {
};


// Sets default values
AHeightMapPlanet::AHeightMapPlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHeightMapPlanet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeightMapPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHeightMapPlanet::Initialize() {

	const float Height = 1;
	
	ChunksOrientation.Add(FVector(Height,0.0f,0.0f)),
	ChunksOrientation.Add(FVector(0.0f,Height,0.0f)),
	ChunksOrientation.Add(FVector(0.0f,0.0f,Height)),
	ChunksOrientation.Add(FVector(-Height,0.0f,0.0f)),
	ChunksOrientation.Add(FVector(0.0f,-Height,0.0f)),
	ChunksOrientation.Add(FVector(0.0f,0.0f,-Height)),


	
	Manager = UMapEditorUtils::TryGetHeightMapPlanetManager();
	if (Manager != nullptr) {

		ElemSize = Manager->ElemSize;
		ChunkSize = Manager->ChunkSize;
		PlanetCenter = Manager->PlanetCenter;
		
		if (Chunks.Num() == 0) {
			Chunks.Reserve(6);

			for (auto Orient : ChunksOrientation) {
				FVector Location = PlanetCenter;
				FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
				AHeightMapPlanetChunk* Chunk = UMapEditorUtils::GetActualWorld()->SpawnActor<AHeightMapPlanetChunk>(AHeightMapPlanetChunk::StaticClass(), Location, Rotation);
				Chunk->Initialize(this, Orient);
				Chunks.Add(Chunk);
			}
			
		}

		for (int i = 0; i< Chunks.Num(); i++) {
			if (Manager->Offsets.IsValidIndex(i)) {
				Chunks[i]->Offset = Manager->Offsets[i];
			}
			Chunks[i]->GenerateChunk();
		}
		
	}
}

void AHeightMapPlanet::Remove() {
	for (auto Chunk : Chunks) {
		UMapEditorUtils::GetActualWorld()->DestroyActor(Chunk);
	}
	Chunks.Empty();
}
