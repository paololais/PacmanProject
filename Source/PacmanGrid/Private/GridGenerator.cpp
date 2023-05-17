// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGenerator.h"

constexpr int MapSizeX = 30;
// la y contiene 28 elementi (+ il carattere di fine stringa)
constexpr int MapSizeY = 29;

// # - Wall
// B - Big Point
// N - Empty
// P - Player
//     Phantom:
//     - 1 - Blinky
//     - 2 - Pinky
//     - 3 - Inky
//     - 4 - Clyde
//    
// C - Cherry
// R - Ghost Respawn
// G - Ghost Area
// E - Ghost Exit

static char Map[MapSizeX][MapSizeY] = {
	"############################",
	"#000000000000##000000000000#",
	"#0####0#####0##0#####0####0#",
	"#B####0#####0##0#####0####B#",
	"#0####0#####0##0#####0####0#",
	"#00000000000000000000000000#",
	"#0####0##0########0##0####0#",
	"#0####0##0########0##0####0#",
	"#000000##0000##0000##000000#",
	"######0#####0##0#####0######",
	"######0#####0##0#####0######",
	"######0##0000000000##0######",
	"######0##0########0##0######",
	"######0##0#GGGGGG#0##0######",
	"T000000000#GGGGGG#000000000T",
	"######0##0#GGGGGG#0##0######",
	"######0##0###E####0##0######",
	"######0##0000000000##0######",
	"######0##0########0##0######",
	"######0##0########0##0######",
	"#000000000000##000000000000#",
	"#0####0#####0##0#####0####0#",
	"#0####0#####0##0#####0####0#",
	"#B00##0000000000000000##00B#",
	"###0##0##0########0##0##0###",
	"#000000##0########0##000000#",
	"#0#######0000##0000#######0#",
	"#0##########0##0##########0#",
	"#00000000000000000000000000#",
	"############################",
};

// Sets default values
AGridGenerator::AGridGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TileSize = 100.0f;
	SpawnOffset = FVector(TileSize);
	CountFood = 0;
}

// Called when the game starts or when spawned
void AGridGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateGrid();
}

// Called every frame
void AGridGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TMap<FVector2D, AGridBaseNode*> AGridGenerator::GetTileMAp()
{
	return TileMap;
}

TMap<FVector2D, AEatableEntity*> AGridGenerator::GetEatableEntityMap()
{
	return EatableEntityMap;
}

void AGridGenerator::GenerateGrid()
{
	AEatableEntity* Food = nullptr;

	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY - 1; y++)
		{
			const char MapTile = Map[x][y];

			FVector OffsetVector(x * SpawnOffset.X, y * SpawnOffset.Y, 0);
			// imposto le coordinate spaziali per la funzione di spawn della tile
			const FVector CurrentSpawnPosition = GetActorLocation() + OffsetVector;
			// questa funzione spawna una nuova tile
			const auto SpawnedNode = SpawnNodeActorById(MapTile, CurrentSpawnPosition);
			// assegna le coordinate di griglia alla tile
			SpawnedNode->TileGridPosition = (FVector2D(x, y));
			// assegna le coordinate spaziali alla tile
			SpawnedNode->TileCoordinatesPosition = CurrentSpawnPosition;
			// aggiungo alle strutture dati il riferimento alla tile creata
			Grid.Add(SpawnedNode);
			TileMap.Add(FVector2D(x, y), SpawnedNode);
			
			//Spawn pellet points and power
			if (MapTile == 'B') {
				FVector OffsetVectorZ(0, 0, 5.0f);
				FVector PositionShifted = CurrentSpawnPosition + OffsetVectorZ;
				Food = GetWorld()->SpawnActor<AEatableEntity>(PowerNode, PositionShifted, FRotator::ZeroRotator);
			}

			if (MapTile == '0') {
				FVector OffsetVectorZ(0, 0, +5.0f);
				FVector PositionShifted = CurrentSpawnPosition + OffsetVectorZ;
				Food = GetWorld()->SpawnActor<AEatableEntity>(PointNode, PositionShifted, FRotator::ZeroRotator);
			}

			if (Food != nullptr)
			{
				const auto SpawnedEatableEntity = Food;
				//conversione delle cordinate in 2D
				FVector2D Position2D = (x, y);
				//associo la posizione della griglia del oggetto spawnato
				SpawnedEatableEntity->EatableEntityPosition = (Position2D);
				//associo le coordinate spaziali
				SpawnedEatableEntity->EatableEntityCoordinatesPosition = CurrentSpawnPosition;
				
				//add reference to the eatable entity map
				EatableEntityMap.Add(FVector2D(x,y), SpawnedEatableEntity);

				//reassign null to food to reuse it
				Food = nullptr;
			}
		}
	}
	SetCountFood(EatableEntityMap.Num());
}

AGridBaseNode* AGridGenerator::GetNodeByCoords(const FVector2D Coords)
{
	if (Coords.X > MapSizeX - 1 || Coords.Y > MapSizeY - 2) return nullptr;
	if (Coords.X < 0 || Coords.Y < 0) return nullptr;
	return GetTileMAp()[FVector2D(Coords.X, Coords.Y)];
}

bool AGridGenerator::IsNodeReachableAndNextToCurrentPosition(const FVector2D CurrentCoordinates, const FVector2D TargetCoords)
{
	const float DistX = FMath::Abs(CurrentCoordinates.X - TargetCoords.X);
	const float DistY = FMath::Abs(CurrentCoordinates.Y - TargetCoords.Y);
	if (DistX > 1 || DistY > 1) return false;
	AGridBaseNode* const N = GetNodeByCoords(TargetCoords);
	if (N && N->EWalkableId == NotWalkable) return false;
	return true;
}

AGridBaseNode* AGridGenerator::GetClosestNodeFromMyCoordsToTargetCoords(const FVector2D StartCoords, const FVector2D TargetCoords, FVector IgnoredDir)
{
	//Get all neighbours
	const TArray<FDirNode> Neighbours = GetNodeNeighbours(GetNodeByCoords(StartCoords));
	float Dist = FLT_MAX;
	AGridBaseNode* ReturnNode = nullptr;
	for (FDirNode FDirNode : Neighbours)
	{
		if (FDirNode.Node == nullptr || FDirNode.Dir == IgnoredDir || FDirNode.Node->EWalkableId == NotWalkable)
			continue;
		
		const float TempDist = FVector2D::Distance(FDirNode.Node->GetGridPosition(), TargetCoords);
		if (TempDist < Dist)
		{
			Dist = TempDist;
			ReturnNode = FDirNode.Node;
		}
	}
	
	return ReturnNode;
}

TArray<FDirNode> AGridGenerator::GetNodeNeighbours(const AGridBaseNode* Node)
{
	TArray<FDirNode> Vec;
	if (Node)
	{	Vec.Add(FDirNode(GetNextNode(Node->GetGridPosition(), FVector::ForwardVector), FVector::ForwardVector));
		Vec.Add(FDirNode(GetNextNode(Node->GetGridPosition(), FVector::RightVector), FVector::RightVector));
		Vec.Add(FDirNode(GetNextNode(Node->GetGridPosition(), -FVector::ForwardVector), -FVector::ForwardVector));
		Vec.Add(FDirNode(GetNextNode(Node->GetGridPosition(), FVector::LeftVector), FVector::LeftVector));
	}
	return Vec;
}


int AGridGenerator::GetCountFood()
{
	return CountFood;
}

void AGridGenerator::SetCountFood(int count)
{
	this->CountFood = count;
}

AGridBaseNode* AGridGenerator::SpawnNodeActorById(char CharId, FVector Position) const
{
	AGridBaseNode* Node;
	TSubclassOf<AGridBaseNode> ClassToSpawn = AGridBaseNode::StaticClass();
	AEatableEntity* Food = nullptr;

	if (CharId == '#')
	{
		ClassToSpawn = WallNode;
	}
	else if (CharId == 'B')
	{
		ClassToSpawn = PowerN;
	}
	else if (CharId == 'N')
	{
		ClassToSpawn = NullNode;
	}
	else if (CharId == 'T')
	{
		ClassToSpawn = TeleportNode;
	}
	else if (CharId == 'G')
	{
		ClassToSpawn = GhostAreaNode;
	}
	else if (CharId == 'E')
	{
		ClassToSpawn = GhostExitNode;
	}
	else
	{
		ClassToSpawn = LabyrinthNode;
	}
	Node = GetWorld()->SpawnActor<AGridBaseNode>(ClassToSpawn, Position, FRotator::ZeroRotator);
	return Node;
}


bool AGridGenerator::IsNodeValidForWalk(AGridBaseNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->EWalkableId == NotWalkable) return false;

	return true;
}

AGridBaseNode* AGridGenerator::GetNextNode(const FVector2D StartCoords, FVector InputDir)
{

	const float RequestedX = StartCoords.X + InputDir.X;
	const float RequestedY = StartCoords.Y + InputDir.Y;
	// la funzione clamp ritorna un numero compreso tra min e max se nel range
	//se più piccolo del min ritorna il min
	//se più grande del max ritorna il max
	const float ClampedX = FMath::Clamp(RequestedX, 0.f, MapSizeX - 1);
	const float ClampedY = FMath::Clamp(RequestedY, 0.f, MapSizeY - 2);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), ClampedX, ClampedY));
	AGridBaseNode* PossibleNode = GetTileMAp()[FVector2D(ClampedX, ClampedY)];

	if (RequestedX > ClampedX || RequestedX < 0)
	{
		//X overflow
		PossibleNode = nullptr;
	}
	if (RequestedY > ClampedY || RequestedY < 0)
	{
		//Y overflow
		PossibleNode = nullptr;
	}
	return PossibleNode;
}

FVector2D AGridGenerator::GetPosition(const FHitResult& Hit)
{
	return Cast<AGridBaseNode>(Hit.GetActor())->GetGridPosition();
}

TArray<AGridBaseNode*>& AGridGenerator::GetTileArray()
{
	return Grid;
}

FVector AGridGenerator::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY)
{
	return  TileSize * FVector(InX, InY, 0);
}

FVector2D AGridGenerator::GetXYPositionByRelativeLocation(const FVector& Location)
{
	double x = floor(Location[0] / (TileSize));
	double y = floor(Location[1] / (TileSize));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}

FVector2D AGridGenerator::GetTwoDOfVector(FVector DDDVector)
{
	return FVector2D(DDDVector.X, DDDVector.Y);
}

FVector AGridGenerator::GetThreeDOfTwoDVector(FVector2D DDDVector)
{
	return FVector(DDDVector.X, DDDVector.Y, 0);
}