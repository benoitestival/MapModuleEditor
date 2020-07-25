// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "MapEditorModule/ToolKits/MapEditorToolKit.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "MapGenerator/Utils/EnumsUtils.h"
#include "IDetailsView.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FModeChanged, TEnumAsByte<EMapType>);

class MAPEDITORMODULE_API SMapEditorMainWidget : public SCompoundWidget{

public:

	SLATE_BEGIN_ARGS(SMapEditorMainWidget) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

	void OnCheckBoxStateChanged(ECheckBoxState CheckBoxState, EMapType MapTypeImpacted);

	void SetDetailPanelObject(UObject* Object);
	
	TSharedRef<SGridPanel> CreateRadioButtonMenuPanel(int NumberOfColumns);

	TMap<EMapType, TSharedRef<SCheckBox>> CheckBoxes;
	
	FModeChanged ModeChanged;

	
private:
	
	TSharedPtr<IDetailsView> DetailsPanel;
	TSharedPtr<IDetailsView> OptionalDetailsPanel;

	//typedef void(UClickPaintTool::*PaintMode)(ULevelCell*);
};
