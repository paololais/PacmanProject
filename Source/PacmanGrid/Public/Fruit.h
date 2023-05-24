// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EatableEntity.h"
#include "Fruit.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API AFruit : public AEatableEntity
{
	GENERATED_BODY()
public:
	AFruit();

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// set up collider to use for PowerNode and PointNode
	UPROPERTY(EditAnywhere)
		UBoxComponent* Collider;
};
