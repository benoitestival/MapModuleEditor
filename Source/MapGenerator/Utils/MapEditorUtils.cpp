// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditorUtils.h"
#include "MapGenerator/GameMode/EditorGameMode.h"
#include "MapGenerator/MapActor.h"
#include "MapGenerator/Objects/BaseMapManager.h"
#include "MapGenerator/HeightMapLandscape/LandscapeManager.h"
#include "MapGenerator/Volumetric3DTerrain/Volumetric3DTerrainManager.h"
#include "MapGenerator/HeightMapPlanet/HeightMapPlanetManager.h"

TEnumAsByte<EMapType> UMapEditorUtils::DefaultMapType = EMapType::ProceduralLandscape;
TEnumAsByte<EMapType> UMapEditorUtils::ActualMapType = UMapEditorUtils::DefaultMapType;

TMap<EMapType, UClass*> UMapEditorUtils::ManagerRegistry = TMap<EMapType, UClass*>();
TMap<EMapType, FString> UMapEditorUtils::ModeName = TMap<EMapType, FString>();

bool UMapEditorUtils::IsRunning = false;

void UMapEditorUtils::InitMapEditor() {
	ManagerRegistry.Empty();//Just in case
	ManagerRegistry.Add({EMapType::ProceduralLandscape}, {ULandscapeManager::StaticClass()});
	ManagerRegistry.Add({EMapType::ProceduralHMPlanet}, {UHeightMapPlanetManager::StaticClass()});
	ManagerRegistry.Add({EMapType::Volumetric3DLandscape}, {UVolumetric3DTerrainManager::StaticClass()});

	ModeName.Add({ EMapType::ProceduralLandscape }, { "Landscape procedural" });
	ModeName.Add({ EMapType::ProceduralHMPlanet }, { "HM Planet procedural" });
	ModeName.Add({ EMapType::Volumetric3DLandscape }, { "Terrain Volumetric" });
	IsRunning = true;
	UEditorGameMode::Get();
}

void UMapEditorUtils::DestroyMapEditor() {
	UEditorGameMode::Destroy();
	ManagerRegistry.Empty();
	IsRunning = false;
}

bool UMapEditorUtils::IsEditorRunning() {
	return IsRunning;
}


AMapActor* UMapEditorUtils::GetMapActor() {
	return UMapEditorUtils::GetMapGameMode()->GetMapActor();
}

UBaseMapManager* UMapEditorUtils::GetActualMapManager() {
	return UMapEditorUtils::GetMapActor()->MapManager;
}

ULandscapeManager* UMapEditorUtils::TryGetLandscapeManager() {
	return Cast<ULandscapeManager>(UMapEditorUtils::GetActualMapManager());
}

UVolumetric3DTerrainManager* UMapEditorUtils::tryGetVolumetricTerrainManager() {
	return Cast<UVolumetric3DTerrainManager>(UMapEditorUtils::GetActualMapManager());
}

UHeightMapPlanetManager * UMapEditorUtils::TryGetHeightMapPlanetManager() {
	return Cast<UHeightMapPlanetManager>(UMapEditorUtils::GetActualMapManager());
}

UWorld* UMapEditorUtils::GetActualWorld() {
	return UMapEditorUtils::GetMapGameMode()->GetWorld();
}

UEditorGameMode* UMapEditorUtils::GetMapGameMode() {
	return UEditorGameMode::Get();
}
