// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extend = SpawningBox->GetScaledBoxExtent(); //Se obtiene el tamaño de la caja
	FVector Origin = SpawningBox->GetComponentLocation(); //Origen de la caja

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extend);  //Con el origen y tamaño de la caja genera una ubicación aleatoria

	return Point;
}

void ASpawnVolume::SpawnOurPawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			//World->SpawnActor<ACritter>(ToSpawn, Location, FRotator(0.f));
		}
	}
}