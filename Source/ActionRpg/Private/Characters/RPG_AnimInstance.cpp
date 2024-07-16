// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPG_AnimInstance.h"
#include "Characters/Rpg_Echo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URPG_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EchoCharacter = Cast<ARpg_Echo>(TryGetPawnOwner());

	if (EchoCharacter)
	{
		EchoCharacterMovement = EchoCharacter->GetCharacterMovement();
	}
}

void URPG_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (EchoCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(EchoCharacterMovement->Velocity);
		IsFalling = EchoCharacterMovement->IsFalling();
		CharacterState = EchoCharacter->GetCharacterState();
		ActionState = EchoCharacter->GetActionState();
		DeathPose = EchoCharacter->GetDeathPose();
	}
}
