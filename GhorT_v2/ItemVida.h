// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemVida.generated.h"

/**
 * 
 */
UCLASS()
class GHORT_V2_API AItemVida : public AItem
{
	GENERATED_BODY()
public:
	AItemVida();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Recover;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
