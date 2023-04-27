// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EatableEntity.h"
#include "GridPawn.h"
#include "PowerNode.generated.h"


UCLASS()
class PACMANGRID_API APowerNode : public AEatableEntity
{
	GENERATED_BODY()
	
public:
	APowerNode();

	// gestione del tempo 
	FTimerHandle PowerModeTimer;

	/*
		definisco una funzione in grado di gestire le interazioni tra collider di diversi
		oggetti, riuscendo a tenere traccia dei singoli eventi
	*/
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	UFUNCTION()
		virtual void BeginPlay() override;

	// set up collider to use for PowerNode and PointNode
	UPROPERTY(EditAnywhere)
		UBoxComponent* Collider;

protected:
	UPROPERTY(VisibleAnywhere)
		class ATestGridGameMode* GameMode;
};
