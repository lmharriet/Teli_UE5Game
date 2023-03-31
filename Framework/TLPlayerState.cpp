// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TLPlayerState.h"

#include "Character/Abilities/TLAbilitySystemComponent.h"

ATLPlayerState::ATLPlayerState()
{
	AbilitySystemComp = CreateDefaultSubobject<UTLAbilitySystemComponent>(TEXT("AbilitySystemComps"));
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);


	AttributeSetBase = CreateDefaultSubobject<UTLAttributeSetBase>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

}

UAbilitySystemComponent* ATLPlayerState::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComp);
}

UTLAttributeSetBase* ATLPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}


bool ATLPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}


void ATLPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	// @TODO : implement HUD later
}

float ATLPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ATLPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ATLPlayerState::GetMana() const
{
	return AttributeSetBase->GetMana();
}

float ATLPlayerState::GetMaxMana() const
{
	return AttributeSetBase->GetMaxMana();
}

int32 ATLPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetLevel();
}

void ATLPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComp)
	{
		HealthChangedDelegateHandle = AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject
									(this, &ATLPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject
									(this, &ATLPlayerState::MaxHealthChanged);
		ManaChangedDelegateHandle = AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaAttribute()).AddUObject
									(this, &ATLPlayerState::ManaChanged);
		MaxManaChangedDelegateHandle = AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxManaAttribute()).AddUObject
									(this, &ATLPlayerState::MaxManaChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetLevelAttribute()).AddUObject
									(this, &ATLPlayerState::CharacterLevelChanged);

		AbilitySystemComp->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"),EGameplayTagEventType::NewOrRemoved)).AddUObject
							(this,&ATLPlayerState::StunTagChanged);
	}


}

void ATLPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("HealthChanged!"));
}

void ATLPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("MaxHealthChanged!"));
}

void ATLPlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("ManaChanged!"));
}

void ATLPlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("MaxManaChanged!"));
}

void ATLPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterLevelChanged!"));
}

void ATLPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagToCancel;
		AbilityTagToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagToIgnore;
		AbilityTagToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComp->CancelAbilities(&AbilityTagToCancel, &AbilityTagToIgnore);
	}
}
