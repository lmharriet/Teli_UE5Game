// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TLPlayerController.h"
#include "Framework/TLPlayerState.h"
#include "Abilities/TLAbilitySystemComponent.h"

void ATLPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	ATLPlayerState* PS = GetPlayerState<ATLPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, aPawn);
	}
}

// @TODO : Add HUD Stuff