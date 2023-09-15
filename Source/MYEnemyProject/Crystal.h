// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crystal.generated.h"

class UBoxComponent;

UCLASS()
class MYENEMYPROJECT_API ACrystal : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Box;
};
