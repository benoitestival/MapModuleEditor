// Fill out your copyright notice in the Description page of Project Settings.


#include "Volumetric3DTerrainManager.h"

UVolumetric3DTerrainManager::UVolumetric3DTerrainManager() {
	MapType = EMapType::Volumetric3DLandscape;
}


void UVolumetric3DTerrainManager::Enter() {
	Super::Enter();
	UE_LOG(LogTemp, Warning, TEXT("Volumetric Terrain : Enter"));
}

void UVolumetric3DTerrainManager::Leave() {
	UE_LOG(LogTemp, Warning, TEXT("Volumetric Terrain : Leave"));
	Super::Leave();
}

void UVolumetric3DTerrainManager::Construct() {
	UE_LOG(LogTemp, Warning, TEXT("Volumetric Terrain : Construct"));
}
