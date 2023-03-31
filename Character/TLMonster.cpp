// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TLMonster.h"

ATLMonster::ATLMonster(const FObjectInitializer& ObjectInitializer)
	:Grade(1)
{

}

void ATLMonster::BeginPlay()
{
	Super::BeginPlay();
}

void ATLMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATLMonster::SetGrade(const int32 InGrade)
{
	Grade = InGrade;
}

const ECharacterType ATLMonster::GetCharacterType()
{
	return ECharacterType::Monster; 
}

const int32& ATLMonster::GetGrade()
{
	return Grade;
}