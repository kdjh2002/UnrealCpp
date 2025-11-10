// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ActionCharactor.generated.h"	//ㅁㅈㄱ 마지ㅏㅁㄱ


class UInputAction;

UCLASS()
class UNREALCPP_API AActionCharactor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharactor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = 100.0f;


protected:

	void OnMoveInput(const FInputActionValue& InValue);


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player| Camera")
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player| Camera")
	TObjectPtr<class UCameraComponent> PlayerCamera = nullptr;
	//TObjectPtr<USpringArmComponent> a = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Move;
};
