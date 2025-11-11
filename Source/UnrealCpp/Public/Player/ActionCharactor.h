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
	//입력 방향 입력 받기
	void OnMoveInput(const FInputActionValue& InValue);

	//구르기 입력 받기
	void OnRollInput(const FInputActionValue& InValue);

	//달리기 모드 설정
	void SetSprintMode();

	//걷기 모드 설정
	void SetWalkMode();


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player| Camera")
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player| Camera")
	TObjectPtr<class UCameraComponent> PlayerCamera = nullptr;
	//TObjectPtr<USpringArmComponent> a = nullptr;

	//인풋 액션들 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Sprint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Roll = nullptr;


	//달리기 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float SprintSpeed = 1200.0f;//BS_Move에서 그래프 최대 길이로 맞추기

	//걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float WalkSpeed = 600.0f;//BS_Move에서 그래프 최대 길이로 맞추기

	//구르기 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	//UAnimMontage* //이것도 가능
	TObjectPtr<UAnimMontage>RollMontage = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance>AnimInstance = nullptr;
 
};
