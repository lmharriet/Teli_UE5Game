// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Defines/TLCharacterDefines.h"

#include "TLCharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TELIHOLD_API UTLCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UTLCharacterGameplayAbility();

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EDemoAbilityID AbilityInputID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EDemoAbilityID AbilityID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted;
};
