// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "ActionPlayerController.generated.h"


class UInputMappingContext; //UInput~ 라는 클래스가 있다고 알려주는 역할
/**
 * 
 */
UCLASS()
class UNREALCPP_API AActionPlayerController : public APlayerController
{
	GENERATED_BODY()

	//UInputMappingContext a; //전방 선언한 것은 헤더가 없으면 인스턴스를 만들 수 없다.
	//UInputMappingContext* b; //전방 선언한 것의 주소는 저장 할 수 있다.


protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")		//가비지컬렉팅 or 블루프린트에서 사용
		TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

private:
	int32 GameInputPriority = 1;

};
