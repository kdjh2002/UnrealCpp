// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonEnums.h"
#include "Data/WeaponDataAsset.h"
#include "WeaponManagerComponent.generated.h"

// This class does not need to be modified.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALCPP_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	// Sets default values for this component's properties
	UWeaponManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//시작시 데이터가 잘 들어있는지 체크해서 문제가 있으면 로그를 출력하는 함수
	void ValidateWeaponDatabase();

protected:
	//이 컴포넌트가 관리하는 무기 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Database")
	TMap<EItemCode, TObjectPtr<UWeaponDataAsset>> WeaponDatabase;

	TMap<EItemCode, TObjectPtr<AWeaponActor>> WeaponInstance;
};
