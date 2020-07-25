// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h" 


class SMapEditorMainWidget;

class MAPEDITORMODULE_API MapEditorToolKit : public FModeToolkit{
public:
	MapEditorToolKit();
	~MapEditorToolKit();

	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	virtual TSharedPtr<SWidget> GetInlineContent() const override;

	TSharedPtr<SMapEditorMainWidget> GetEditorWidget() const;

private:

	TSharedPtr<SMapEditorMainWidget> EditorMainWidget;
};
