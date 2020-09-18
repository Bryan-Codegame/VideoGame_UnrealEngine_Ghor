// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerController.h"


// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create camera boom (pulls towards the player If there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; //Camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arm based on controller

	//Setsize for collision capsule
	GetCapsuleComponent()->SetCapsuleSize(60.f, 107.f);

	//Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//Attach the camera to the end of the the boom and let the boom adjust to match the controller oriantation.
	FollowCamera->bUsePawnControlRotation = false;

	//Set our turn rates for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	//Don´t rotate when the controller rotates.
	//Let that just affect the camera.
	//El personaje no gira cuando la cámara gira.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; //Personaje gira respecto a la dirección del input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 840.f, 0.0f); //Velocidad con la que gira
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//Items Values
	MaxHealth = 100.f;
	Health = 100.f;
	MaxEnergy = 100.f;
	Energy = 50.f;
	MaxLightitem = 10;
	Lightitem = 0;

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	//Press buttons or clics
	bShiftKeyDown = false; //Sprint
	bEquipDown = false; //EquipWeapon
	bLeftClickDown = false; //Attack
	//bESCDown = false; //Pause

	//Initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	EnergyStatus = EEnergyStatus::EES_Normal;

	EnergyDrainRate = 5.f;
	EnergyRecoveryRate = 0.6f;
	MinSprintEnergy = 33.f;
	MaxSprintEnergy = 66.f;

	//Combo Contador
	Count = 0;

}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaEnergy = EnergyDrainRate * DeltaTime;
	float DeltaEnergyRecovery = EnergyRecoveryRate * DeltaTime;

	//EnergyBarStatus
	switch (EnergyStatus)
	{
	case EEnergyStatus::EES_Normal:
		if (bShiftKeyDown)
		{
			if (Energy - DeltaEnergy <= MinSprintEnergy)
			{
				SetEnergyStatus(EEnergyStatus::EES_BelowMinimum);
				Energy -= DeltaEnergy;
			}
			else
			{
				Energy -= DeltaEnergy;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else //Shift key up
		{
			if (Energy + DeltaEnergy >= MaxSprintEnergy)
			{
				SetEnergyStatus(EEnergyStatus::EES_AboveMaximum);
				Energy += DeltaEnergyRecovery;
			}
			else
			{
				Energy += DeltaEnergyRecovery;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	//Below Minimum
	case EEnergyStatus::EES_BelowMinimum:
		if (bShiftKeyDown)
		{
			if (Energy - DeltaEnergy <= 0)
			{
				
				SetMovementStatus(EMovementStatus::EMS_Normal);
				Energy += DeltaEnergyRecovery;
			}
			else
			{
				Energy -= DeltaEnergy;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else //Shift key up
		{
			if (Energy + DeltaEnergy >= MinSprintEnergy)
			{
				SetEnergyStatus(EEnergyStatus::EES_Normal);
				Energy += DeltaEnergyRecovery;
			}
			else
			{
				Energy += DeltaEnergyRecovery;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	//Above Maximum
	case EEnergyStatus::EES_AboveMaximum:
		if (bShiftKeyDown)
		{
			if (Energy - DeltaEnergy <= MaxSprintEnergy)
			{
				SetEnergyStatus(EEnergyStatus::EES_Normal);
				Energy -= DeltaEnergy;
			}
			else
			{
				Energy -= DeltaEnergy;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else //Shift key up
		{
			if (Energy + DeltaEnergy >= MaxEnergy)
			{
				Energy = MaxEnergy;
			}
			else
			{
				Energy += DeltaEnergyRecovery;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EEnergyStatus::EES_Exhausted:
		
		/**if (Energy <= 0)
		{
			Energy = 1.f;
			Energy += DeltaEnergyRecovery;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);*/
		break;

	default:
		;
	}
	

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AMain::EquipDown);
	PlayerInputComponent->BindAction("Equip", IE_Released, this, &AMain::EquipUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LeftClickDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LeftClickUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);
}

void  AMain::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0)) // && (!bAttacking)
	{	
		//Find out wich way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}


void AMain::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0)) //&& (!bAttacking)
	{
		//Find out wich way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


//Decremente Energia del bebé
void AMain::DecrementEnergy(float Amount)
{
	if (Energy - Amount <= 0.f)
	{
		Energy = 1.f;
	}
	else
	{
		Energy -= Amount;
	}
}
//Decrementa vida
void AMain::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

//Incrementa vida
void AMain::IncrementHealth(float Amount)
{
	if (Health + Amount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += Amount;
	}
}

//Incrementa contador de items recogidos
void AMain::IncrementItemLight()
{
	if (Lightitem + 1 >= MaxLightitem)
	{
		Lightitem = MaxLightitem;
	}
	else
	{
		Lightitem++;
	}
	
}


void AMain::IncrementItemEnergy(float Amount)
{
	if (Energy + Amount >= MaxEnergy)
	{
		Energy = MaxEnergy;
	}
	else
	{
		Energy += Amount;
	}
}
void AMain::Die()
{

}

//Movement define if character is sprinting or not
void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

//Press shift
void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

//Press E

void AMain::EquipDown()
{
	bEquipDown = true;
	
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);

		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	
}

void AMain::EquipUp()
{
	bEquipDown = false;
}

//Attack
void AMain::LeftClickDown()
{
	bLeftClickDown = true;

	UE_LOG(LogTemp, Warning, TEXT("LeftClick"));

	if (EquippedWeapon)
	{
		Count++;
		Attack();
	}
	
	
}
void AMain::LeftClickUp()
{
	bLeftClickDown = false;
}

//Attack
void AMain::Attack()
{
	if (!bAttacking)
	{
		bAttacking = true;

		if (bAttacking)
		{
			DecrementEnergy(2.f);
		}
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			

			if (Count == 5)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("AtaqueGhor3"), CombatMontage);
				Count = 0;

			}
			else 
			{
				int32 Section = FMath::RandRange(0, 1);

				switch (Section)
				{
				case 0:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("AtaqueGhor1"), CombatMontage);
					break;
				case 1:
					AnimInstance->Montage_Play(CombatMontage, 1.20f);
					AnimInstance->Montage_JumpToSection(FName("AtaqueGhor2"), CombatMontage);
					break;
				}
			}			
		}
	}
}

void AMain::AttackEnd()
{
	bAttacking = false;

	if (bLeftClickDown)
	{
		Attack();
	}
}

//Press to pause
/*void AMain::ESCDown()
{
	bESCDown = true;
	
	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}
void AMain::ESCUp()
{
	bESCDown = false;
}

//Destroy the equipped weapon when the main picks up another weapon 
/*void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = WeaponToSet;
} */