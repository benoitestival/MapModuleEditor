// Fill out your copyright notice in the Description page of Project Settings.


#include "SMapEditorMainWidget.h"
#include "MapEditorModule/MapEditorMode.h"
#include "MapEditorModule/Utils/MapEditorModuleUtils.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "MapGenerator/MapActor.h"


void SMapEditorMainWidget::Construct(const FArguments& InArgs) {
	
	FMapEditorMode* MapEditorMode = MapEditorModuleUtils::GetMapEditorMode();

	/*StateChangedRegistry[0] = &SMapEditorMainWidget::StateChangedToProceduralLandscape;
	StateChangedRegistry[1] = &SMapEditorMainWidget::StateChangedToVolumetriqueTerrain;*/
	
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea);

	DetailsPanel = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	if (MapEditorMode != nullptr) {

		AMapActor* MapActor = UMapEditorUtils::GetMapActor();
		if (MapActor != nullptr) {
			ModeChanged.AddUObject(MapActor, &AMapActor::SwitchMapMode);
			DetailsPanel->SetObject(MapActor);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MapActor is nullptr"));
		}
	}

	const FMargin Padding(0.0f, 4.0f, 0.0f, 4.0f);

	const TSharedRef<SGridPanel> RadioButtonPanelMenu = CreateRadioButtonMenuPanel(2);
	
	ChildSlot[
		SNew(SScrollBox) + SScrollBox::Slot().Padding(0.0f)[
			SNew(SVerticalBox) + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)[
				RadioButtonPanelMenu
			] + SVerticalBox::Slot().AutoHeight().Padding(3.0f)[
				SNew(SSeparator).Orientation(Orient_Horizontal)
			] 
			+ SVerticalBox::Slot().AutoHeight()[
				DetailsPanel.ToSharedRef()
			]
		]
	];
	
}

void SMapEditorMainWidget::SetDetailPanelObject(UObject* Object) {
	if (Object != nullptr) {
		DetailsPanel->SetObject(Object);
	}
}

TSharedRef<SGridPanel> SMapEditorMainWidget::CreateRadioButtonMenuPanel(int NumberOfColumns) {
	
	const TSharedRef<SGridPanel> RadioButtonGridPanel = SNew(SGridPanel);

	int it = 0;
	for (auto& MapType : UEnumsUtils::DifferentMapTypes) {
		FText Text = FText::FromString(*UMapEditorUtils::ModeName.Find(MapType));
		TSharedRef<SCheckBox> CheckBox = SNew(SCheckBox).Style(FEditorStyle::Get(), "RadioButton").Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f)).OnCheckStateChanged(this, &SMapEditorMainWidget::OnCheckBoxStateChanged, MapType);
		RadioButtonGridPanel->AddSlot(it % NumberOfColumns, it / NumberOfColumns).Padding(FMargin(20.0f, 5.0f))[
			CheckBox
		];
		it++;
		RadioButtonGridPanel->AddSlot(it % NumberOfColumns, it / NumberOfColumns).Padding(FMargin(20.0f, 5.0f))[
			SNew(STextBlock).Text(Text)
		];
		it++;
		CheckBoxes.Add({MapType}, {CheckBox});
	}
	TSharedRef<SCheckBox> CheckBox = *CheckBoxes.Find(UMapEditorUtils::ActualMapType);
	CheckBox->SetIsChecked(ECheckBoxState::Checked);
	
	return RadioButtonGridPanel;
}

void SMapEditorMainWidget::OnCheckBoxStateChanged(ECheckBoxState CheckBoxState, EMapType MapTypeImpacted) {

	UE_LOG(LogTemp, Warning, TEXT("Mode change asked"));

	if (CheckBoxState == ECheckBoxState::Checked) {
		if (MapTypeImpacted != UMapEditorUtils::ActualMapType) {
			UE_LOG(LogTemp, Warning, TEXT("Mode will changed"));
			TSharedRef<SCheckBox> CheckBox = *CheckBoxes.Find(UMapEditorUtils::ActualMapType);
			CheckBox->SetIsChecked(ECheckBoxState::Unchecked);
			ModeChanged.Broadcast(MapTypeImpacted);
		}
	}
	else if (CheckBoxState == ECheckBoxState::Unchecked) {
		if (MapTypeImpacted == UMapEditorUtils::ActualMapType) {
			UE_LOG(LogTemp, Warning, TEXT("Mode will not changed"));
			TSharedRef<SCheckBox> CheckBox = *CheckBoxes.Find(MapTypeImpacted);
			CheckBox->SetIsChecked(ECheckBoxState::Checked);
		}
	}
	
}


