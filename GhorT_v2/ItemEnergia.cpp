// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEnergia.h"
#include "Main.h"

AItemEnergia::AItemEnergia()
{
	RecoverEnergy = 15.f;
}

void AItemEnergia::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("ItemEnergia::OnOverlapBegin()"));

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->IncrementItemEnergy(RecoverEnergy);
			Destroy();
		}
	}

}

void AItemEnergia::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("ItemEnergia::OnOverlapEnd()"));
}