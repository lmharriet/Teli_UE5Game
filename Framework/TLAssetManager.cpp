// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TLAssetManager.h"
#include "AbilitySystemGlobals.h"
void UTLAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();

	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
}