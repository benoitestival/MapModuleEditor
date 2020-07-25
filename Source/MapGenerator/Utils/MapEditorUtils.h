// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MapGenerator/Utils/EnumsUtils.h"
#include "MapEditorUtils.generated.h"

class UEditorGameMode;
class UBaseMapManager;
class UVolumetric3DTerrainManager;
class ULandscapeManager;
class AMapActor;

UCLASS()
class MAPGENERATOR_API UMapEditorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static TEnumAsByte<EMapType> DefaultMapType;
	static TEnumAsByte<EMapType> ActualMapType;
	static TMap<EMapType, UClass*> ManagerRegistry;
	static TMap<EMapType, FString> ModeName;
	static bool IsRunning;
public:
	static void InitMapEditor();
	static void DestroyMapEditor();

	static bool IsEditorRunning();
	static AMapActor* GetMapActor();
	static UBaseMapManager* GetActualMapManager();
	static ULandscapeManager* TryGetLandscapeManager();
	static UVolumetric3DTerrainManager* tryGetVolumetricTerrainManager();

	static UWorld* GetActualWorld();
	
private:
	
	static UEditorGameMode* GetMapGameMode();
};
