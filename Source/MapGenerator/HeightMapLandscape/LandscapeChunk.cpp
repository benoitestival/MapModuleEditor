// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeChunk.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "Providers/RuntimeMeshProviderStatic.h"
#include "MapGenerator/HeightMapLandscape/LandscapeManager.h"

// Sets default values
ALandscapeChunk::ALandscapeChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALandscapeChunk::Generate() {

	NoiseModule.SetNoiseType(FastNoise::Perlin);
	
	Manager = UMapEditorUtils::TryGetLandscapeManager();
	NoiseSettings.LoadNoiseSettings(ChunkX, ChunkY, Manager);
	URuntimeMeshProviderStatic* StaticProvider = NewObject<URuntimeMeshProviderStatic>(this);
	if (StaticProvider != nullptr) {
		//Vertices are added in local and not in world

		GetRuntimeMeshComponent()->Initialize(StaticProvider);

		ChunkSizeX = Manager->ChunkSizeX;
		ChunkSizeY = Manager->ChunkSizeY;

		UseNormals = Manager->AddNormals;
		ShowNoiseLog = Manager->ShowNoiseLog;
		/*const int MeshSizeX = SizeX + 1;
		const int MeshSizeY = SizeY + 1;
		*/
		const float CellResolution = Manager->CellResolution;
		
		TArray<FVector> Vertices;
		Vertices.Reserve(ChunkSizeX * ChunkSizeY);
		TArray<int> Triangles;
		Triangles.Reserve(ChunkSizeX * ChunkSizeY * 3 - ChunkSizeY);
		TArray<FColor> Colors;//No reserve because not using for now;
		TArray<FVector> Normals;
		Normals.Init(FVector(), (ChunkSizeX + 1) * (ChunkSizeY + 1));
		TArray<FVector2D> UVs;
		UVs.Reserve(ChunkSizeX * ChunkSizeY);
		TArray<FRuntimeMeshTangent> Tangents;//No reserve because not using for now;

		TArray<int> VerticesBottomLineCache;
		VerticesBottomLineCache.Reserve(ChunkSizeY);
		TArray<int> VerticesTopLineCache;
		VerticesBottomLineCache.Reserve(ChunkSizeY);
		
		for (int y = 0; y < ChunkSizeY + 1; y++) {
			const FVector Vertice = FVector(0, y * CellResolution, GetNoiseValue(0, y));
			VerticesBottomLineCache.Add(Vertices.Add(Vertice));
		}

		for (int x = 0; x < ChunkSizeX; x++) {
			
			const FVector FirstTopLineVert = FVector((x + 1) * CellResolution, 0, GetNoiseValue(x + 1, 0));
			VerticesTopLineCache.Add(Vertices.Add(FirstTopLineVert));
			
			for (int y = 0; y < ChunkSizeY; y++) {
				const FVector VerticeTopToAdd = FVector((x + 1) * CellResolution, (y + 1) * CellResolution, GetNoiseValue(x + 1, y + 1));
				VerticesTopLineCache.Add(Vertices.Add(VerticeTopToAdd));

				Triangles.Add(VerticesBottomLineCache[y]);
				Triangles.Add(VerticesBottomLineCache[y + 1]);
				Triangles.Add(VerticesTopLineCache[y]);

				Triangles.Add(VerticesBottomLineCache[y + 1]);
				Triangles.Add(VerticesTopLineCache[y + 1]);
				Triangles.Add(VerticesTopLineCache[y]);

				//Adding normal
				if (UseNormals) {
					const FVector AB = Vertices[VerticesTopLineCache[y]] - Vertices[VerticesBottomLineCache[y]];
					const FVector AC = Vertices[VerticesBottomLineCache[y + 1]] - Vertices[VerticesBottomLineCache[y]];
					const FVector NormalForBottom = FVector::CrossProduct(AB, AC);
					Normals[VerticesBottomLineCache[y]] = NormalForBottom.GetSafeNormal();
				}
				
				
			}

			//Adding the extra normal at the end of the line
			if (UseNormals) {
				const FVector AB = Vertices[VerticesTopLineCache[ChunkSizeY - 1]] - Vertices[VerticesBottomLineCache[ChunkSizeY]];
				const FVector AC = Vertices[VerticesTopLineCache[ChunkSizeY]] - Vertices[VerticesBottomLineCache[ChunkSizeY]];
				const FVector NormalForBottom = FVector::CrossProduct(AB, AC);
				Normals[VerticesBottomLineCache[ChunkSizeY]] = NormalForBottom.GetSafeNormal();
			}
	

			VerticesBottomLineCache = VerticesTopLineCache;
			VerticesTopLineCache.Empty();
		}

		//Adding normals for last line
		//FVector OffsetDown = FVector(-CellResolution, 0.0f, 0.0f);
		//for (int y = 0; y < SizeY ; y++) {
		//	OffsetDown.Z = GetNoiseValue(SizeX - 1, y);
		//	const FVector AB = Vertices[VerticesBottomLineCache[y + 1]] - Vertices[VerticesBottomLineCache[y]];
		//	const FVector AC = FVector(Vertices[VerticesBottomLineCache[y + 1]] + OffsetDown) - Vertices[VerticesBottomLineCache[y]];
		//	const FVector Normal = FVector::CrossProduct(AB, AC);
		//	Normals[VerticesBottomLineCache[y]] = Normal.GetSafeNormal();
		//}
		////
		////Adding last normal
		//OffsetDown.Z = GetNoiseValue(SizeX - 1, SizeY - 1);
		//const FVector AB = Vertices[VerticesBottomLineCache[SizeY - 2]] - Vertices[VerticesBottomLineCache[SizeY - 1]];
		//const FVector AC = FVector(Vertices[VerticesBottomLineCache[SizeY - 1]] - OffsetDown) - Vertices[VerticesBottomLineCache[SizeY - 1]];
		//const FVector Normal = FVector::CrossProduct(AB, AC);
		//Normals[SizeY * SizeX - 1] = Normal.GetSafeNormal();
		
		StaticProvider->CreateSectionFromComponents(0,0,0,Vertices, Triangles, Normals, UVs, Colors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
		
	}
	
}

float ALandscapeChunk::GetNoiseValue(int x, int y) {

	if (NoiseSettings.NumOctaves == 0) {
		NoiseSettings.NumOctaves = 1;
	}

	if (NoiseSettings.Scale == 0) {
		NoiseSettings.Scale = 0.001f;
	}
	
	float NoiseValue = 0.0f;
	float Frequency = 1.0f;
	float Amplitude = 1.0f;
	
	for (int o = 0; o < NoiseSettings.NumOctaves; o++) {

		const int NewX = (x + NoiseSettings.XOffset) / NoiseSettings.Scale * Frequency;
		const int NewY = (y + NoiseSettings.YOffset) / NoiseSettings.Scale * Frequency;
		
		const float Perlin = NoiseModule.GetPerlin(NewX, NewY);
		if (ShowNoiseLog) {
			UE_LOG(LogTemp, Warning, TEXT("Original Perlin Value : %f, for x: %d and y: %d"), Perlin, x, y);
		}
		NoiseValue += Perlin * Amplitude;
		Amplitude *= NoiseSettings.Persistance;
		Frequency *= NoiseSettings.Lacunarity;
	}
	
	if (NoiseSettings.UseFallof) {
		if(NoiseSettings.Fallof != nullptr) {
			const float FallofInfluence = NoiseSettings.Fallof->GetFloatValue(NoiseValue);
			NoiseValue = NoiseValue * (NoiseSettings.HeightMultiplicator * FallofInfluence);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Select a curve"));
		}
	}
	else {
		NoiseValue = NoiseValue * NoiseSettings.HeightMultiplicator;
		if (NoiseValue < NoiseSettings.SeaLevel) {
			NoiseValue = NoiseSettings.SeaLevel;
		}
	}
	
	/*if (ShowNoiseLog) {
		UE_LOG(LogTemp, Warning, TEXT("End Perlin Value : %f, for x: %d and y: %d"), NoiseValue, x, y);
	}*/
	return NoiseValue ;
}


// Called when the game starts or when spawned
void ALandscapeChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALandscapeChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

