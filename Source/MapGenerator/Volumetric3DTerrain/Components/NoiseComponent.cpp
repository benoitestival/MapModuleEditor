// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseComponent.h"
#include "MapGenerator/Volumetric3DTerrain/Volumetric3DTerrainManager.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "MapGenerator/Volumetric3DTerrain/Octree/OctreeUtils.h"

FastNoise UNoiseComponent::NoiseModule = FastNoise();
float UNoiseComponent::Height = 0.0f;
float UNoiseComponent::Scale = 0.0001f;
bool UNoiseComponent::ApplyZ = true;
bool UNoiseComponent::Planet = false;
float UNoiseComponent::PlanetRadius = 2000.0f;
float UNoiseComponent::MinElemSize = 80.0f;
UCurveFloat* UNoiseComponent::Curve = nullptr;
int UNoiseComponent::NumOctaves = 3;
float UNoiseComponent::Persistence = 0.1f;
float UNoiseComponent::Lacunarity = 5.0f;
float UNoiseComponent::DistGroundCave = 5.0f;
float UNoiseComponent::HoleRadiusMax = 0.0f;
float UNoiseComponent::HoleRadiusMin = 0.0f;
TArray<FHole> UNoiseComponent::Holes = {};

void UNoiseComponent::Init() {
	NoiseModule.SetNoiseType(NoiseType::Perlin);
	UVolumetric3DTerrainManager* Manager = UMapEditorUtils::tryGetVolumetricTerrainManager();
	if (Manager != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Noise Settings Load"));
		Scale = Manager->NoiseScale;
		ApplyZ = Manager->ApplyZToFunc;
		Height = Manager->HeightMultiplicator;
		Planet = Manager->Planet;
		Curve = Manager->Fallof;
		NumOctaves = Manager->NumOctaves;
		
		if (Planet) {
			PlanetRadius = Manager->PlanetRadius;
			MinElemSize = Manager->Elementize;
			DistGroundCave = Manager->DistGroundToCave;
			if (PlanetRadius > UOctreeUtils::GetOctreeSize()) {
				PlanetRadius = UOctreeUtils::GetOctreeSize() * 0.5 - Manager->Elementize * 2;
			}
			HoleRadiusMax = Manager->MaxSizeHole;
			HoleRadiusMin = Manager->MinSizeHole;
			Holes.Empty();
			for (int i = 0; i < Manager->NumOfHoles; i++) {
				FHole NewHole = { FMath::VRand() * PlanetRadius + UOctreeUtils::GetOctreePosition() , FMath::FRandRange(HoleRadiusMin, HoleRadiusMax)};
				Holes.Add(NewHole);
			}
		}
		
	}
}


float UNoiseComponent::GetPerlin(float x, float y, float z) {

	if (Scale == 0.0f) {
		Scale = 0.00001f;
	}

	float NewX = x / Scale;
	float NewY = y / Scale;
	float NewZ = z / Scale;

	float NoiseValue = 0.0f;

	if (Planet) {
		const float HalfSize = MinElemSize * 0.5f;
		const FVector PointPos = FVector(x * MinElemSize + HalfSize, y * MinElemSize + HalfSize, z * MinElemSize + HalfSize);
		const float DistCenterToPos = FVector::Dist(PointPos, UOctreeUtils::GetOctreePosition());

		if (DistCenterToPos > PlanetRadius - DistGroundCave) {
			NoiseValue = PlanetRadius - DistCenterToPos;
		}
		else {
			NoiseValue = 0.0f;
		}

		if (DistCenterToPos < PlanetRadius + DistGroundCave && DistCenterToPos > PlanetRadius - DistGroundCave) {
			for (auto& Hole : Holes) {
				const float Dist = FVector::Dist(PointPos, Hole.Position);
				if (Dist < Hole.Radius) {
					NoiseValue = -100.0f;
				}
			}
		}
		
	}
	else {
		NoiseValue = -NewZ;
	}

	float Frequency = 1.0f;
	float Amplitude = 1.0f;
	float PerlinValue = 0.0f;
	
	for (int i = 0; i < NumOctaves; i++) {

		NewX *= Frequency;
		NewY *= Frequency;
		NewZ *= Frequency;
		float PerlinNoise = 0.0f;
		if(ApplyZ) {
			PerlinNoise = NoiseModule.GetNoise(NewX, NewY, NewZ);
		}
		else {
			PerlinNoise = NoiseModule.GetNoise(NewX, NewY, 0.0f);
		}
		PerlinValue += PerlinNoise * Amplitude;
		FMath::Clamp(NoiseValue, -1.0f, 1.0f);
		Amplitude *= Persistence;
		Frequency *= Lacunarity;
		
	}
	
	NoiseValue += PerlinValue * Height;

	NoiseValue = FMath::Clamp(NoiseValue, -Height, Height);	
	
	NoiseValue /= Height;

	//Rescale Bettween 0 and 1;
	NoiseValue = (NoiseValue + 1.0f) * 0.5f;


	return NoiseValue;
}
