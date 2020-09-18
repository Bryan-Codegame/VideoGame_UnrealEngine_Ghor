// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemEnergia.generated.h"

/**
 * 
 */
UCLASS()
class GHORT_V2_API AItemEnergia : public AItem
{
	GENERATED_BODY()

public:
	AItemEnergia();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float RecoverEnergy;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
