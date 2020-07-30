// Fill out your copyright notice in the Description page of Project Settings.


#include "FallOffMapComponent.h"
#include "MapGenerator/HeightMapLandscape/LandscapeChunk.h"

void UFallOffMapComponent::Initialize(ALandscapeChunk* Chunk) {

	if (!Chunks.Contains(Chunk)) {
		if (Chunk != nullptr) {
			Chunks.Add(Chunk);
			Chunk->NoiseValueCalculated.AddDynamic(this, &UFallOffMapComponent::CalculateFalloff);
		}
	}
}

void UFallOffMapComponent::Remove(ULandscapeManager* Manager) {

	for (auto Chunk : Chunks) {
		Chunk->NoiseValueCalculated.RemoveDynamic(this, &UFallOffMapComponent::CalculateFalloff);
	}
	Chunks.Empty();
}


void UFallOffMapComponent::CalculateFalloff(TArray<float>& HeightMap, ALandscapeChunk* Chunk) {

	if (FallOffType == EFalloffType::Square) {

		for (int x = 0; x < Chunk->ChunkSizeX + 1; x++) {
			for (int y = 0; y < Chunk->ChunkSizeY + 1; y++) {
				
				const int Index = x * (Chunk->ChunkSizeY + 1) + y;
				float NoiseValue = HeightMap[Index];
				
				const int NewX = (x + Chunk->NoiseSettings.XOffset)/* / NoiseSettings.IslandSize*/;
				const int NewY = (y + Chunk->NoiseSettings.YOffset)/* / NoiseSettings.IslandSize*/;

				const float XVal = NewX / (float)(ChunkFallofScale * Chunk->ChunkSizeX) * 2 - 1;
				const float YVal = NewY / (float)(ChunkFallofScale * Chunk->ChunkSizeY) * 2 - 1;

				const float FallofValue = FMath::Max(FMath::Abs(XVal), FMath::Abs(YVal));

				
				NoiseValue -= FMath::Pow(FallofValue, A) / (FMath::Pow(FallofValue, A) + FMath::Pow(B - B * FallofValue, A));
				NoiseValue = FMath::Clamp(NoiseValue, 0.0f, 1.0f);

				HeightMap[Index] = NoiseValue;
				
			}
		}
	}
	else {

		
	}
	
}
