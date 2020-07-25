// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditorToolKit.h"
#include "MapEditorModule/Widgets/SMapEditorMainWidget.h"

MapEditorToolKit::MapEditorToolKit()
{
}

MapEditorToolKit::~MapEditorToolKit()
{
}

void MapEditorToolKit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) {
	EditorMainWidget = SNew(SMapEditorMainWidget);
	FModeToolkit::Init(InitToolkitHost);
}

TSharedPtr<SWidget> MapEditorToolKit::GetInlineContent() const {
	return EditorMainWidget;
}

TSharedPtr<SMapEditorMainWidget> MapEditorToolKit::GetEditorWidget() const {
	return EditorMainWidget;
}
