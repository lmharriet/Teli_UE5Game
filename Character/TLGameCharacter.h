// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TLCharacter.h"
#include "TLGameCharacter.generated.h"

/**
 * 
 */

class UCameraComponent;
class USpringArmComponent;
class ATLPlayerState;

UCLASS()
class TELIHOLD_API ATLGameCharacter : public ATLCharacter
{
	GENERATED_BODY()
public:
		ATLGameCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NowController) override;
	
	TObjectPtr<USpringArmComponent> GetCameraBoom();
	TObjectPtr<UCameraComponent> GetFollowCamera();

	UFUNCTION(BlueprintCallable,Category = "TL|Camera")
	float GetStartingCameraBoomArmLength();
	UFUNCTION(BlueprintCallable, Category = "TL|Camera")
	FVector GetStartingCameraBoomLocation();
	
public:

	virtual const ECharacterType GetCharacterType() override;
	
private:
	void InitStartingValues(TObjectPtr<ATLPlayerState> InPlayerState);
	
private:
	void LookUp(float Value);
	void LookUpRate(float Value);
	void Turn(float Value);
	void TurnRate(float Value);
	void MoveForwardBack(float Value);
	void MoveLeftRight(float Value);

	virtual void OnRep_PlayerState() override;

	void BindASCInput();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "TL|Camera")
	float BaseTurnRate = 45.0f;

	UPROPERTY(BlueprintReadOnly, Category = "TL|Camera")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(BlueprintReadOnly, Category = "TL|Camera")
	float StartingCameraBoomArmLength;

	UPROPERTY(BlueprintReadOnly, Category = "TL|Camera")
	FVector StartingCameraBoomLocation;
	
private:
	UPROPERTY(Category = "TL|Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCameraComponent> FollowCam;
	UPROPERTY(Category = "TL|Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<USpringArmComponent> CameraBoom;

	bool ASCInputBound = false;

	FGameplayTag DeadTag;
};
