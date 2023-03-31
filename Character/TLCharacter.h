// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Defines/TLCharacterDefines.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "TLCharacter.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate , ATLCharacter*);

UCLASS()
class TELIHOLD_API ATLCharacter : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATLCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	virtual void SetHealth(float InHealth);
	virtual void SetMana(float InMana);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:

	void SetCharacterType(const ECharacterType InCharacterType);
	void SetMoveStance(const EMoveStance InMoveStance);
	
public:

	virtual const ECharacterType GetCharacterType();
	const EMoveStance& GetMoveStance();
	
	//UPROPERTY(BlueprintAssignable, Category = "TL|Character")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category = "TL|Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "TL|Character")
	virtual int32 GetAbilityLevel(EDemoAbilityID InAbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable , Category = "TL|Character")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "TL|Character|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "TL|Character|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "TL|Character|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "TL|Character|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "TL|Character|Attributes")
	float GetMaxMana() const;


protected:
	TWeakObjectPtr<class UTLAbilitySystemComponent> AbilitySystemComp;
	TWeakObjectPtr<class UTLAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TL|Character")
	FText CharacterName;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TL|Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TL|Attributes")
	TArray<TSubclassOf<class UTLCharacterGameplayAbility>> CharacterAbilities;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TL|Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TL|Attributes")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

private:
	ECharacterType CharacterType;
	EMoveStance MoveStance;

};
