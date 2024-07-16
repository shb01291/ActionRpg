// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Rpg_Echo.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/RpgHUD.h"
#include "HUD/RpgOverlay.h"
#include "Components/AttributeComponent.h"
#include "Items/Soul.h"
#include "Items/Treasures/Treasure.h"

// Sets default values
ARpg_Echo::ARpg_Echo()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 250.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");


	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ARpg_Echo::Tick(float DeltaTime)
{
	if (Attributes && RpgOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		RpgOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void ARpg_Echo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ARpg_Echo::Move);//Move를 MoveAction에 바인딩
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARpg_Echo::Look);//Move를 MoveAction에 바인딩
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARpg_Echo::Jump);//Move를 MoveAction에 바인딩
		EnhancedInputComponent->BindAction(EkeyAction, ETriggerEvent::Triggered, this, &ARpg_Echo::EkeyPressed);
		EnhancedInputComponent->BindAction(LeftMouseAction, ETriggerEvent::Triggered, this, &ARpg_Echo::Attack);
		EnhancedInputComponent->BindAction(MouseWheelAction, ETriggerEvent::Triggered, this, &ARpg_Echo::MouseWheelScroll);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ARpg_Echo::Dodge);
	}

}

float ARpg_Echo::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ARpg_Echo::SetHUDHealth()
{
	if (RpgOverlay && Attributes)
	{
		RpgOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void ARpg_Echo::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

bool ARpg_Echo::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ARpg_Echo::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	// 타격 중간에 공격 받았을 시 disable boxcollision 실행안될 때 대비
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ARpg_Echo::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ARpg_Echo::AddSouls(ASoul* Soul)
{
	if (Attributes && RpgOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		RpgOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ARpg_Echo::AddGold(ATreasure* Treasure)
{
	if (Attributes && RpgOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		RpgOverlay->SetGold(Attributes->GetGold());
	}
}

// Called when the game starts or when spawned
void ARpg_Echo::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast< APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterContext, 0);
		}
	}

	Tags.Add(FName("EngageableTarget"));
	InitializeRpgOverlay();
}

void ARpg_Echo::InitializeRpgOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ARpgHUD* RpgHUD = Cast<ARpgHUD>(PlayerController->GetHUD());
		if (RpgHUD)
		{
			RpgOverlay = RpgHUD->GetRpgOverlay();
			if (RpgOverlay && Attributes)
			{
				RpgOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				RpgOverlay->SetStaminaBarPercent(1.f);
				RpgOverlay->SetGold(0.f);
				RpgOverlay->SetSouls(0.f);

			}
		}
	}
}

void ARpg_Echo::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ARpg_Echo::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxis.Y);
	AddControllerYawInput(LookAxis.X);

}

void ARpg_Echo::EkeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			DisArm();
		}
		else if (CanArm()) 
		{
			Arm();
		}
	}
}

void ARpg_Echo::Attack()
{
	Super::Attack();

	if (CanAttack()) 
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ARpg_Echo::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina(Attributes->GetDodgeCost())) return;
	
	PlayDodgeMontage();

	ActionState = EActionState::EAS_Dodge;
	if (Attributes && RpgOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		RpgOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

bool ARpg_Echo::HasEnoughStamina(int32 Cost)
{
	return Attributes->GetStaminaPercent() > Cost;
}

bool ARpg_Echo::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void ARpg_Echo::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void ARpg_Echo::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ARpg_Echo::MouseWheelScroll(const FInputActionValue& Value)
{
	if(Value.Get<float>() > 0)
	{
		if (CameraBoom->TargetArmLength > 20)
		{
		
		float NewSpringArmLength = FMath::Clamp(CameraBoom->TargetArmLength - ZoomSpeed, 20, 700);
		CameraBoom->TargetArmLength = NewSpringArmLength;


		}
	}

	else if (Value.Get<float>() < 0)
	{
		if (CameraBoom->TargetArmLength < 700)
		{
			float NewSpringArmLength = FMath::Clamp(CameraBoom->TargetArmLength + ZoomSpeed, 20, 700);
			CameraBoom->TargetArmLength = NewSpringArmLength;
		}
	}
}



bool ARpg_Echo::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void ARpg_Echo::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

void ARpg_Echo::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
}

bool ARpg_Echo::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState != ECharacterState::ECS_Unequipped;
}

bool ARpg_Echo::CanArm()
{
	return ActionState==EActionState::EAS_Unoccupied
		&& CharacterState == ECharacterState::ECS_Unequipped
		&& EquippedWeapon;
}

void ARpg_Echo::DisArm()
{
	PlayEquipMontage(FName("UnEquip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ARpg_Echo::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ARpg_Echo::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ARpg_Echo::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ARpg_Echo::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ARpg_Echo::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ARpg_Echo::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
		
	}
}


