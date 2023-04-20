// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleporter.generated.h"

class USoundCue;

UCLASS()
class PACMANGRID_API ATeleporter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeleporter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TeleportToTarget(AActor* Actor);

	UPROPERTY(EditAnywhere)
		ATeleporter* Target = nullptr;

	UPROPERTY(EditAnywhere)
		USoundCue* TeleportSound;

	UFUNCTION()
		void OnOverlapBegin(AActor* Teleporter, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		FVector2D TeleporterPosition;

	// FVector position of the Point
	UPROPERTY(EditAnywhere)
		FVector TeleporterCoordinatesPosition;

	// get the (x, y) position
	FVector2D getPosition();

	// get position of the Point 
	FVector GetTeleporterCoordinates();
};
