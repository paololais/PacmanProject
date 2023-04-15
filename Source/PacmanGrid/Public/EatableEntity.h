// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EatableEntity.generated.h"

UENUM()
enum EEatId { NotEaten, Eaten };

UCLASS()
class PACMANGRID_API AEatableEntity : public AActor
{
	GENERATED_BODY()

public:

	// x,y position of the Point
	UPROPERTY(EditAnywhere)
		FVector2D EatableEntityPosition;

	// FVector position of the Point
	UPROPERTY(EditAnywhere)
		FVector EatableEntityCoordinatesPosition;

	UPROPERTY(EditAnywhere)
		int32 pointValue;

	// Sets default values for this actor's properties
	AEatableEntity();

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EEatId> EEatId = NotEaten;

	// get the (x, y) position
	FVector2D getPosition();

	// get position of the Point 
	FVector GetTileCoordinates();

	// set the (x, y) position
	void SetFruitPosition(const double InX, const double InY);

	//utili per gestione delle proprietà
	void setEaten();
	int getPointValue();
	bool CheckNotEaten();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
