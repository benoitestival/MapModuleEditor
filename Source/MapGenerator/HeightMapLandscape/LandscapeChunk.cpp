// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeChunk.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "Providers/RuntimeMeshProviderStatic.h"
#include "MapGenerator/Objects/BaseLandscapeComponent.h"
#include "MapGenerator/HeightMapLandscape/LandscapeManager.h"

// Sets default values
ALandscapeChunk::ALandscapeChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALandscapeChunk::Init() {
	NoiseSettings.NoiseModule.SetNoiseType(NoiseType::Perlin);
	Manager = UMapEditorUtils::TryGetLandscapeManager();
	NoiseSettings.LoadNoiseSettings(ChunkX, ChunkY, Manager);
	ChunkSizeX = Manager->ChunkSizeX;
	ChunkSizeY = Manager->ChunkSizeY;
	UseElevation = Manager->UseElevationScale;
	for (auto Component : Manager->MapComponents) {
		if(Component != nullptr) {
			Component->Initialize(this);
		}
	}
	GenerateNoiseMap();
	GenerateMesh(false);
}

void ALandscapeChunk::GenerateMesh(bool UseRawHeightMap) {

	URuntimeMeshProviderStatic* StaticProvider = NewObject<URuntimeMeshProviderStatic>(this);
	if (StaticProvider != nullptr) {
		//Vertices are added in local and not in world

		GetRuntimeMeshComponent()->Initialize(StaticProvider);

		const bool UseNormals = Manager->AddNormals;
		const bool UseUvs = Manager->AddUVs;

		//const bool UseElevationScale = Manager->UseElevationScale;
		
		const float CellResolution = Manager->CellResolution;
		
		TArray<FVector> Vertices;
		Vertices.Reserve((ChunkSizeX + 1) * (ChunkSizeY + 1));
		TArray<int> Triangles;
		Triangles.Reserve((ChunkSizeX + 1) * (ChunkSizeY + 1) * 3 - (ChunkSizeY + 1));
		TArray<FColor> Colors;//No reserve because not using for now;
		TArray<FVector> Normals;
		Normals.Init(FVector(), (ChunkSizeX + 1) * (ChunkSizeY + 1));
		TArray<FVector2D> UVs;
		UVs.Reserve((ChunkSizeX + 1) * (ChunkSizeY + 1));
		TArray<FRuntimeMeshTangent> Tangents;//No reserve because not using for now;

		TArray<int> VerticesBottomLineCache;
		VerticesBottomLineCache.Reserve(ChunkSizeY + 1);
		TArray<int> VerticesTopLineCache;
		VerticesBottomLineCache.Reserve(ChunkSizeY + 1);
		
		for (int y = 0; y < ChunkSizeY + 1; y++) {
			const FVector Vertice = FVector(0, y * CellResolution, UseRawHeightMap ? HeightMap[y] : ApplyHeightMultiplicatorByCoord(0, y));
			VerticesBottomLineCache.Add(Vertices.Add(Vertice));
			if (UseUvs) {
				UVs.Add(FVector2D(0.0f, (float)y / (float)ChunkSizeY));
			}
		}

		for (int x = 0; x < ChunkSizeX; x++) {
			
			const FVector FirstTopLineVert = FVector((x + 1) * CellResolution, 0, UseRawHeightMap ? HeightMap[x * (ChunkSizeY + 1)] : ApplyHeightMultiplicatorByCoord(x + 1, 0));
			if (UseUvs) {
				UVs.Add(FVector2D((float)(x + 1) / (float)ChunkSizeX, 0.0f));
			}
			VerticesTopLineCache.Add(Vertices.Add(FirstTopLineVert));
			
			for (int y = 0; y < ChunkSizeY; y++) {
				const FVector VerticeTopToAdd = FVector((x + 1) * CellResolution, (y + 1) * CellResolution, UseRawHeightMap ? HeightMap[x * (ChunkSizeY + 1) + y] : ApplyHeightMultiplicatorByCoord(x + 1,y + 1) );
				if (UseUvs) {
					UVs.Add(FVector2D((float)(x + 1) / (float)ChunkSizeX, (float)(y + 1) / (float)ChunkSizeY));
				}
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

		StaticProvider->SetupMaterialSlot(0, TEXT("TriMat"), Manager->Material);
		StaticProvider->CreateSectionFromComponents(0,0,0,Vertices, Triangles, Normals, UVs, Colors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
		
	}	
}

void ALandscapeChunk::GenerateNoiseMap() {
	
	const bool ShowNoiseLog = Manager->ShowNoiseLog;
	HeightMap.Empty();
	HeightMap.Reserve((ChunkSizeX + 1) * (ChunkSizeY + 1));

	if (NoiseSettings.NumOctaves == 0) {
		NoiseSettings.NumOctaves = 1;
	}

	if (NoiseSettings.Scale == 0) {
		NoiseSettings.Scale = 0.001f;
	}

	for (int x = 0; x < ChunkSizeX + 1; x++) {
		for (int y = 0; y < ChunkSizeY + 1; y++) {

			float NoiseValue = 0.0f;
			float Frequency = 1.0f;
			float Amplitude = 1.0f;

			for (int o = 0; o < NoiseSettings.NumOctaves; o++) {
				const float NewX = ((float)x + (float)NoiseSettings.XOffset) / NoiseSettings.Scale * Frequency;
				const float NewY = ((float)y + (float)NoiseSettings.YOffset) / NoiseSettings.Scale * Frequency;

				const float Perlin = NoiseSettings.NoiseModule.GetPerlin(NewX, NewY);
				if (ShowNoiseLog) {
					UE_LOG(LogTemp, Warning, TEXT("Original Perlin Value : %f, for x: %d and y: %d"), Perlin, x, y);
				}
				
				NoiseValue += Perlin * Amplitude;
				FMath::Clamp(NoiseValue, -1.0f, 1.0f);
				Amplitude *= NoiseSettings.Persistance;
				Frequency *= NoiseSettings.Lacunarity;
			}
			
			NoiseValue = (NoiseValue + 1) * 0.5;			
			HeightMap.Add(NoiseValue);

		}
	}

	NoiseValueCalculated.Broadcast(HeightMap, this);

	
}


float ALandscapeChunk::ApplyHeightMultiplicatorByCoord(int x, int y) {

	const int index = x * (ChunkSizeY + 1) + y;
	float NoiseValue = HeightMap[index];
	return ApplyHeightMultiplicator(NoiseValue);
}


float ALandscapeChunk::ApplyHeightMultiplicator(float Value){

	
	if (NoiseSettings.Fallof != nullptr) {
		const float FallofInfluence = NoiseSettings.Fallof->GetFloatValue(Value);
		Value = Value * (NoiseSettings.HeightMultiplicator * FallofInfluence);
	}
	else {
		Value = Value * NoiseSettings.HeightMultiplicator;
		if (Value < NoiseSettings.SeaLevel) {
			Value = NoiseSettings.SeaLevel;
		}
	}
	
	return Value;
}




float ALandscapeChunk::CalculateFallofMap(int x, int y) {

	const int NewX = (x + NoiseSettings.XOffset)/* / NoiseSettings.IslandSize*/;
	const int NewY = (y + NoiseSettings.YOffset)/* / NoiseSettings.IslandSize*/;

	const float XVal = NewX / (float)(NoiseSettings.IslandSize * ChunkSizeX) * 2 - 1;
	const float YVal = NewY / (float)(NoiseSettings.IslandSize * ChunkSizeY) * 2 - 1;

	const float FallofValue = FMath::Max(FMath::Abs(XVal), FMath::Abs(YVal));

	const float a = 3.0f;
	const float b = 2.2f;
	
	return FMath::Pow(FallofValue, a) / (FMath::Pow(FallofValue, a) + FMath::Pow(b - b * FallofValue, a));
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

