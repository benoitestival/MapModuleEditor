// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/EnumsUtils.h"
#include "MapActor.generated.h"

class UBaseMapManager;

UCLASS()
class MAPGENERATOR_API AMapActor : public AActor
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Instanced, Export, VisibleAnywhere, BlueprintReadWrite)
	UBaseMapManager* MapManager = nullptr;

public:

	AMapActor();

	
	void Init();
	
	void SwitchMapMode(UBaseMapManager* NewManager);

	UFUNCTION()
	void SwitchMapMode(TEnumAsByte<EMapType> MapType);

	virtual void OnConstruction(const FTransform& Transform) override;

};
