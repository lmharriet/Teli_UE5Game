// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TELIHOLD_API ATLPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
protected:
	virtual void OnPossess(APawn* aPawn) override;
};
