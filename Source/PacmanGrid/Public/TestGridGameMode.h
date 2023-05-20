// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGenerator.h"
#include "GridPawn.h"
#include "Blinky.h"
#include "Inky.h"
#include "Pinky.h"
#include "Clyde.h"
#include "GameFramework/GameMode.h"
#include "TestGridGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API ATestGridGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGridGenerator> GridGeneratorClass;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGridGenerator* GField;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APacmanPawn> PacmanClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABlinky> BlinkyClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AInky> InkyClass;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APinky> PinkyClass;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AClyde> ClydeClass;

	UPROPERTY(VisibleAnywhere)
		APacmanPawn* PacmanPawn;

	UPROPERTY(VisibleAnywhere)
	ABlinky* BlinkyPawn;

	UPROPERTY(VisibleAnywhere)
	AInky* InkyPawn;

	UPROPERTY(VisibleAnywhere)
		APinky* PinkyPawn;

	UPROPERTY(VisibleAnywhere)
		AClyde* ClydePawn;

	UPROPERTY(EditAnywhere, Category="Class Types")
		TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
		class UGameOverWIdget* GameOverWidget;

	UPROPERTY(EditAnywhere, Category = "Class Types")
		TSubclassOf<UUserWidget> GameWinWidgetClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UGameWinWidget* GameWinWidget;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundCue* GameOverSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundCue* GameWinSound;

	void ShowGameOverScreen();

	void ShowGameWinScreen();

	ATestGridGameMode();

	~ATestGridGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//tracks if the game is over
	bool IsGameOver;

	void GameOver();

	//tracks if level is win - use only if you implement other levels
	//bool IsLevelWin;
	//void SetLevelWin();

	void GameWin();

	void RespawnPositions();
};
