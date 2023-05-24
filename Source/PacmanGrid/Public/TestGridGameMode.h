// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGenerator.h"
#include "GridPawn.h"
#include "Blinky.h"
#include "Inky.h"
#include "Pinky.h"
#include "Clyde.h"
#include "Fruit.h"
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

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AFruit> FruitClass;

	UPROPERTY(VisibleAnywhere)
		AFruit* Fruit;

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

	//called when pacman dies or when ghosts die
	FTimerHandle StopTimer;
	void StopMovement(float StopTime);
	void ResumeMovement();

	//global counter - used by ghosts to leave their house after pacman loses a life
	//pinky 7, inky 17
	//se clyde è nella casa quando globalcounter = 32, 
	//allora il contatore verrà azzerato e disattivato
	int GlobalCounter = 0;
};
