// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TLGameCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/TLCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/TLAIController.h"
#include "Framework/TLPlayerState.h"

#include "Abilities/TLAbilitySystemComponent.h"


ATLGameCharacter::ATLGameCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.Get().SetDefaultSubobjectClass<UTLCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region Base

	//// Don't rotate when the controller rotates. Let that just affect the camera.
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	//// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	//// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	//// instead of recompiling to adjust them
	//GetCharacterMovement()->JumpZVelocity = 700.f;
	//GetCharacterMovement()->AirControl = 0.35f;
	//GetCharacterMovement()->MaxWalkSpeed = 500.f;
	//GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;


	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 500.0f;
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//FollowCam = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCam->SetupAttachment(CameraBoom);
	//FollowCam->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

#pragma endregion

#pragma region GAS
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0, 0, 70));


	FollowCam = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCam->SetupAttachment(CameraBoom);
	//FollowCam->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCam->FieldOfView = 80.0f;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	AIControllerClass = ATLAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

#pragma endregion

}

void ATLGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	StartingCameraBoomArmLength = CameraBoom->TargetArmLength;
	StartingCameraBoomLocation = CameraBoom->GetRelativeLocation();
}

void ATLGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ATLGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATLGameCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &ATLGameCharacter::MoveLeftRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);*/


#pragma region GAS
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATLGameCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &ATLGameCharacter::MoveLeftRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ATLGameCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATLGameCharacter::TurnRate);

	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ATLGameCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATLGameCharacter::LookUpRate);
#pragma endregion

	BindASCInput();

}


void ATLGameCharacter::PossessedBy(AController* NowController)
{
	Super::PossessedBy(NowController);

	TObjectPtr<ATLPlayerState> PS = GetPlayerState<ATLPlayerState>();
	if (PS != nullptr && PS->IsValidLowLevel() == true)
	{
		InitStartingValues(PS);

		AddStartupEffects();

		AddCharacterAbilities();
	}
	
}

TObjectPtr<USpringArmComponent> ATLGameCharacter::GetCameraBoom()
{
	return CameraBoom;
}

TObjectPtr<UCameraComponent> ATLGameCharacter::GetFollowCamera()
{
	return FollowCam;
}

float ATLGameCharacter::GetStartingCameraBoomArmLength()
{
	return StartingCameraBoomArmLength;
}

FVector ATLGameCharacter::GetStartingCameraBoomLocation()
{
	return StartingCameraBoomLocation;
}

const ECharacterType ATLGameCharacter::GetCharacterType()
{
	return ECharacterType::Player;
}

void ATLGameCharacter::InitStartingValues(TObjectPtr<class ATLPlayerState> InPlayerState)
{
	if (InPlayerState != nullptr && InPlayerState->IsValidLowLevel() == true)
	{
		AbilitySystemComp = Cast<UTLAbilitySystemComponent>(InPlayerState->GetAbilitySystemComponent());

		InPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(InPlayerState, this);

		AttributeSetBase = InPlayerState->GetAttributeSetBase();

		AbilitySystemComp->SetTagMapCount(DeadTag, 0);

		InitializeAttributes();
		SetHealth(GetMaxHealth());
		SetMana(GetMana());
	}

}

void ATLGameCharacter::LookUp(float Value)
{
	if (IsAlive() == true)
	{
		AddControllerPitchInput(Value);
	}
}

void ATLGameCharacter::LookUpRate(float Value)
{
	if (IsAlive() == true)
	{
		AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
	}
}

void ATLGameCharacter::Turn(float Value)
{
	if (IsAlive() == true)
	{
		AddControllerYawInput(Value);
	}
}

void ATLGameCharacter::TurnRate(float Value)
{
	if (IsAlive() == true)
	{
		AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
	}
}

void ATLGameCharacter::MoveForwardBack(float Value)
{
	if (IsAlive() == false)
	{
		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATLGameCharacter::MoveLeftRight(float Value)
{
	if (IsAlive() == false)
	{
		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATLGameCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	TObjectPtr<ATLPlayerState> PS = GetPlayerState<ATLPlayerState>();
	if (PS != nullptr && PS->IsValidLowLevel() == true)
	{
		InitStartingValues(PS);

		BindASCInput();

		//InitializeAttributes();
	}
}

void ATLGameCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComp.IsValid() && IsValid(InputComponent))
	{
		// TODO : find Reason why it (below code) does not work
		
		//AbilitySystemComp->BindAbilityActivationToInputComponent(
		//	InputComponent, FGameplayAbilityInputBinds(
		//					FString("ConfirmTarget")
		//					, FString("CancelTarget")
		//					, FString("DemoAbilityID")
		//					, static_cast<int32>(EDemoAbilityID::Confirm)
		//					, static_cast<int32>(EDemoAbilityID::Cancel)));

		ASCInputBound = true;
		
	}
}