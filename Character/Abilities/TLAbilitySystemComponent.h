// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TLAbilitySystemComponent.generated.h"

/**
 * 
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UTLAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

UCLASS()
class TELIHOLD_API UTLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UTLAbilitySystemComponent();
public:

	bool CharacterAbilitiesGiven;
	bool StartupEffectApplied;


	FReceivedDamageDelegate ReceivedDamage;
	virtual void ReceiveDamage(UTLAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
