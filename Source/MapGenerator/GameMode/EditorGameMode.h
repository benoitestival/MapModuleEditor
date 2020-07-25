// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AMapActor;

class MAPGENERATOR_API UEditorGameMode
{
public:

	int RayDistance = 15000;

private:

	AMapActor* MapActor;
	
	static UEditorGameMode* EditorGameMode;
	
public:

	bool IsInEditor();
	void Init();
	void Delete();
	AMapActor* FindMapActorInMap() const;
	AMapActor* GetMapActor();

	static UWorld* GetWorld();
	
	static UEditorGameMode* Get();
	static void Destroy();
	
private:
	UEditorGameMode();
	~UEditorGameMode();
};
