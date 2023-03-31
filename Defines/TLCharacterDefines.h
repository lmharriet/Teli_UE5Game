// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TLCharacterDefines.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDemoAbilityID : uint8
{
	None		UMETA(DisplayName = "None"),
	Confirm		UMETA(DisplayName = "Confirm"),
	Cancel		UMETA(DisplayName = "Cancel"),
	// and so on...
};

UENUM(Blueprintable)
enum class ECharacterType : uint8
{
	None,
	Player,
	Monster,
	Npc_Static,
	Npc_Movable
};

UENUM(Blueprintable)
enum class EMoveStance : uint8
{
	None,
	Jog,
	Run,
	Dash,
	Roll,
	Fly,
	Think,
	Attack,
};

UCLASS()
class TELIHOLD_API UTLCharacterDefines : public UObject
{
	GENERATED_BODY()
	
};
