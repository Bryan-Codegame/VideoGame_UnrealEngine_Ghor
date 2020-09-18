// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EEnergyStatus : uint8
{
	EES_AboveMaximum UMETA(DisplayName = "AboveMaximum"),
	EES_Normal UMETA(DisplayName = "Normal"),
	EES_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	EES_Exhausted UMETA(DisplayName = "Exhausted"),
	EES_MAX UMETA(DisplayName = "DefaultMax"),
	EES_Recovering UMETA(DisplayName = "Recovering")
};
UCLASS()
class GHORT_V2_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	//class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EEnergyStatus EnergyStatus;

	FORCEINLINE void SetEnergyStatus(EEnergyStatus Status) { EnergyStatus = Status; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float EnergyDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float EnergyRecoveryRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintEnergy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSprintEnergy;
	
	//Set MovementStatus and running speed
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
    float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	float SprintingSpeed;

	
	
	/** Camera boon potitioning the camera behind the player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rates to scale turning functions for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;


	//Combo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	int32 Count;


	/* Player HUD*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player HUD")
	float MaxHealth; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player HUD")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player HUD")
	float MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player HUD")
	float Energy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player HUD")
	int32 MaxLightitem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player HUD")
	int32 Lightitem;

	void DecrementHealth(float Amount);

	void DecrementEnergy(float Amount);

	void IncrementHealth(float Amount);

	void IncrementItemLight();

	void IncrementItemEnergy(float Amount);

	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Called for forwards/backwards input*/
	void MoveForward(float Value);

	/* Called for side to sode input*/
	void MoveRight(float Value);

	/** Called via input to turn at a given rate
	* @param Rate this is a normalize rate, i.e 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/** Called via input to look up/down at a given rate
	* @param Rate this is a normalize rate, i.e 1.0 means 100% of desired look up/down rate
	*/
	void LookUpAtRate(float Rate);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//Press E to Equip weapon
	bool bEquipDown;
	void EquipDown();
	void EquipUp();

	//Press LeftMouseButton to Attack
	bool bLeftClickDown;
	void LeftClickDown();
	void LeftClickUp();

	//Press Shift to Sprint
	bool bShiftKeyDown;
	void ShiftKeyDown();
	void ShiftKeyUp();

	//Press P or Exit to Pause
	//bool bESCDown;
	//void ESCDown();
	//void ESCUp();

	//Attack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	//Equip weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;

		// Main can active the overlap  when it press a key   
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetEquippedWeapon(AWeapon* WeaponToSet) { EquippedWeapon = WeaponToSet; }
	//FORCEINLINE AWeapon* GetEquipedWeapon() { return EquippedWeapon; }
	
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }
}; 
