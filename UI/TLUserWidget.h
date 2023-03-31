// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TLUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELIHOLD_API UTLUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init();
	virtual void ShutDown();

public:
	const FName& GetWidgetName();
	const void SetWidgetName(const FName& InWidgetName);
private:

	FName WidgetName;
};
