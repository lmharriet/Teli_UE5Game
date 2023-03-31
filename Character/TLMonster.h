// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TLCharacter.h"
#include "TLMonster.generated.h"

/**
 * 
 */
UCLASS()
class TELIHOLD_API ATLMonster : public ATLCharacter
{
	GENERATED_BODY()

public:
	ATLMonster(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetGrade(const int32 InGrade);
public:
	virtual const ECharacterType GetCharacterType() override;
	const int32& GetGrade();

private:

	int32 Grade;
};
