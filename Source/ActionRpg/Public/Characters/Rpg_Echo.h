// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickUpInterface.h"
#include "Rpg_Echo.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;
class URpgOverlay;
class ASoul;
class ATreasure;

UCLASS()
class ACTIONRPG_API ARpg_Echo : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARpg_Echo();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Jump() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	Input
	*/
	 
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* CharacterContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EkeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LeftMouseAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MouseWheelAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;

	/*
	Callback function for Input
	*/

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EkeyPressed();
	virtual void Attack() override;
	void MouseWheelScroll(const FInputActionValue& Value);
	void Dodge();

	
	/*
	* Animation Montage
	*/

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	

	/* 
	* Combat	
	*/
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual void DodgeEnd() override;
	virtual void Die() override;
	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();
	void DisArm();
	void Arm(); 
	bool IsOccupied();
	bool HasEnoughStamina(int32 Cost);
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();



private:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* EyeBrows;
	
	UPROPERTY(VisibleInstanceOnly) // detail 패널에서만 볼 수 있음
	AItem* OverlappingItem;

	float ZoomSpeed = 20.f;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	URpgOverlay* RpgOverlay;

	void InitializeRpgOverlay();
	void SetHUDHealth();
	bool IsUnoccupied();

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

};
