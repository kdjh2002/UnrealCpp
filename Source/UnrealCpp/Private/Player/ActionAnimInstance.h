// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UActionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")//보이는건 어디서든
		float Speed = 0.0f; //매번 업데이트해줘야함 //블프에서EVENTbluprintupdate에서 활용

private: 
	//
	TWeakObjectPtr <const UPawnMovementComponent> OwnerMovementComponent = nullptr;

};


