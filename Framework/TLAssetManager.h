// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TLAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TELIHOLD_API UTLAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	virtual void StartInitialLoading() override;
};
