// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Abilities/AttributeSets/TLAttributeSetBase.h"
#include "TLPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class TELIHOLD_API ATLPlayerState : public APlayerState ,public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ATLPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	class UTLAttributeSetBase* GetAttributeSetBase() const;
	
	UFUNCTION(BlueprintCallable , Category = "TL|PlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "TL|PlayerState|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "TL|PlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "TL|PlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "TL|PlayerState|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "TL|PlayerState|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "TL|PlayerState|Attributes")
	int32 GetCharacterLevel() const;

protected:

	UPROPERTY()
	class UTLAbilitySystemComponent* AbilitySystemComp;
	
	UPROPERTY()
	class UTLAttributeSetBase* AttributeSetBase;
	
	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
