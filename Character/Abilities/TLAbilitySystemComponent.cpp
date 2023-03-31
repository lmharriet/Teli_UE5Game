// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/TLAbilitySystemComponent.h"

UTLAbilitySystemComponent::UTLAbilitySystemComponent()
	:CharacterAbilitiesGiven(false)
	,StartupEffectApplied(false)
{

}


void UTLAbilitySystemComponent::ReceiveDamage(UTLAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}