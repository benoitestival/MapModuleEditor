// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditorModuleUtils.h"
#include "EditorModeManager.h"
#include "MapEditorModule/MapEditorMode.h"


FMapEditorMode* MapEditorModuleUtils::GetMapEditorMode() {
	return (FMapEditorMode*)GLevelEditorModeTools().GetActiveMode(FMapEditorMode::EM_MapEditorID);
}
