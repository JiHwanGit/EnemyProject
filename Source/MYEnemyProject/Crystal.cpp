// Fill out your copyright notice in the Description page of Project Settings.


#include "Crystal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACrystal::ACrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(20, 20, 20));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MyContent/Blueprints/Crystal/source/Fantasy_crystal_stone.Fantasy_crystal_stone'"));

	if (tempMesh.Succeeded())
	{
		Body->SetStaticMesh(tempMesh.Object);
		Body->SetWorldScale3D(FVector(0.1, 0.1, 0.1));
	}	

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void ACrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

