// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GridBaseNode.generated.h"

UENUM()
enum EWalkableId { NotWalkable, Walkable };

UCLASS()
class PACMANGRID_API AGridBaseNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridBaseNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EWalkableId> EWalkableId = Walkable;

	// set the (x, y) position
	void SetGridPosition(const double InX, const double InY);

	// get the (x, y) position
	FVector2D GetGridPosition() const;

	// (x, y) position of the tile
	UPROPERTY(EditAnywhere)
		FVector2D TileGridPosition;

	// FVector position of the tile
	UPROPERTY(EditAnywhere)
		FVector TileCoordinatesPosition;

	FVector GetTileCoordinates();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Collider;

};
