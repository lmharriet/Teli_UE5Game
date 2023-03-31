// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TLCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Character/Abilities/TLCharacterGameplayAbility.h"
#include "Abilities/AttributeSets/TLAttributeSetBase.h"
#include "Abilities/TLAbilitySystemComponent.h"
#include "Character/TLCharacterMovementComponent.h"


// Sets default values
ATLCharacter::ATLCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UTLCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	, CharacterType(ECharacterType::None)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));

}

// Called when the game starts or when spawned
void ATLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATLCharacter::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority
		|| AbilitySystemComp.IsValid() == false
		|| AbilitySystemComp->CharacterAbilitiesGiven ==false)
	{
		return;
	}

	for (TSubclassOf<UTLCharacterGameplayAbility>& StartUpAbility : CharacterAbilities)
	{
		AbilitySystemComp->GiveAbility(
			FGameplayAbilitySpec(StartUpAbility
					, GetAbilityLevel(StartUpAbility.GetDefaultObject()->AbilityID)
					, static_cast<int32>(StartUpAbility.GetDefaultObject()->AbilityInputID)
					, this));

		AbilitySystemComp->CharacterAbilitiesGiven = true;
	}
}

void ATLCharacter::InitializeAttributes()
{
	if (AbilitySystemComp.IsValid() == false)
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."),*FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComp->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGeHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComp.Get());
	}

}



void ATLCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority
		|| AbilitySystemComp.IsValid() == false
		|| AbilitySystemComp->StartupEffectApplied == true)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GamePlayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComp->MakeOutgoingSpec(GamePlayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGeHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComp.Get());
		}
	}

	AbilitySystemComp->StartupEffectApplied = true;

}

void ATLCharacter::SetHealth(float InHealth)
{
	if (AttributeSetBase.IsValid() == true)
	{
		AttributeSetBase->SetHealth(InHealth);
	}
}

void ATLCharacter::SetMana(float InMana)
{
	if (AttributeSetBase.IsValid() == true)
	{
		AttributeSetBase->SetMana(InMana);
	}
}


// Called every frame
void ATLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ATLCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp.Get();
}


bool ATLCharacter::IsAlive() const
{
	return GetHealth() > 0.f;
}


int32 ATLCharacter::GetAbilityLevel(EDemoAbilityID InAbilityID) const
{
	// @ TODO : Change Logic
	return static_cast<int32>(InAbilityID);
}

void ATLCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority 
		|| AbilitySystemComp.IsValid() == false
		|| AbilitySystemComp->CharacterAbilitiesGiven == false)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;

	for (FGameplayAbilitySpec& Spec : AbilitySystemComp->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && (CharacterAbilities.Contains(Spec.Ability->GetClass()) ==true))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i)
	{
		AbilitySystemComp->ClearAbility(AbilitiesToRemove[i]);
	}
	
	AbilitySystemComp->CharacterAbilitiesGiven = false;
	
}

void ATLCharacter::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComp.IsValid() == true)
	{
		AbilitySystemComp->CancelAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);

		int32 NumEffectsEremoved = AbilitySystemComp->RemoveActiveEffectsWithTags(EffectTagsToRemove);
		AbilitySystemComp->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
}

void ATLCharacter::FinishDying()
{
	Destroy();
}


int32 ATLCharacter::GetCharacterLevel() const
{
	if (AttributeSetBase.IsValid() == true)
	{
		return AttributeSetBase->GetLevel();
	}

	return 1;
}

float ATLCharacter::GetHealth() const
{
	if (AttributeSetBase.IsValid() ==true)
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.f;
}


float ATLCharacter::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid() == true)
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.f;
}

float ATLCharacter::GetMana() const
{
	if (AttributeSetBase.IsValid() == true)
	{
		return AttributeSetBase->GetMana();
	}

	return 0.f;
}

float ATLCharacter::GetMaxMana() const
{
	if (AttributeSetBase.IsValid() == true)
	{
		return AttributeSetBase->GetMaxMana();
	}
	return 0.f;
}





// Called to bind functionality to input
void ATLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATLCharacter::SetCharacterType(const ECharacterType InCharacterType)
{
	CharacterType = InCharacterType;
}

void ATLCharacter::SetMoveStance(const EMoveStance InMoveStance)
{
	MoveStance = InMoveStance;
}

const ECharacterType ATLCharacter::GetCharacterType()
{
	
	return CharacterType;
}

const EMoveStance& ATLCharacter::GetMoveStance()
{
	return MoveStance;
}