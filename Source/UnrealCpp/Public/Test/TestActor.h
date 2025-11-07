// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

//generated.h는 모든 헤더 중 마지막에 있어야한다.
#include "TestActor.generated.h"	//생성된 헤더

//UCLASS()
//	- C++ 클래스를 언리얼 엔진의 객체 시스템에 등록(=UObject)
// - 이 메크로가 붙은 클래스만이 언리얼 엔진의 객체로 인식되고 관리된다.
// - 주요기능
//		- 가비지 콜랙팅
//		- 블루프린트 연동
//		- 에디터 노출


//UFUNCTION()
//- 블루프린트에서 함수를 호출, 구현, 재정의 등을 할 떄 UFUCTION 사용
//- 델리게이트에 바인딩 할 경우
//- 다이나믹 델리게이트에 바인딩 할 경우 UFUNCTION 필수
//- 블루프린트에서 사용될 경우 반드시 필요하다.

//UPROPERTY()
//	- UCLASS나 USTRUCT내의 멤버 변수를 언리얼 엔진의 리플랙션 시스템에 등록
//	- 주요기능
//		- 에디터에서 노출 및 편집을 가능하게 한다.
//		- 블루프린트 접근
//		- 가비지 컬랙션이 추적을 가능하게 함
//		- 직렬화(변수의 값이 레벨이나 에셋에 함께 저장) //메모리에
//		- 네트워크 복제(Replicated)		//내 HP가 다른사람한테도 보여지게
//		-

UCLASS()
class UNREALCPP_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Test_NormalFunction();

	//BlueprintCall : //C++로 작성한 함수를 블루프린트에서 호출할 수 있다.
	UFUNCTION(BlueprintCallable, Category = "내함수")
	void Test_UFunction();

	//BlueprintImplementableEvent : //C++로 작성한 함수를 블루프린트에서 호출할 수 있다.
	UFUNCTION(BlueprintImplementableEvent, Category = "내 함수")
	void Test_ImplementableFunction();

	//BlueprintNative
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "내함수")
	void Test_NativeEventFunction();

	//위가 함수
	//아래가 변수
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 


protected:
	int32 Data1 = 10;

	UPROPERTY(VisibleAnyWhere, Category = "Test변수")
	int32 Data2 = 20;

	UPROPERTY(VisibleDefaultsOnly, Category = "Test변수")
	int32 Data3 = 30;

	UPROPERTY(VisibleDefaultsOnly, Category = "Test변수")
	int32 Data4 = 40;

	UPROPERTY(EditAnyWhere, Category = "Test변수")
	int32 Data5 = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Test변수")
	int32 Data6 = 60;

	UPROPERTY(EditDefaultsOnly, Category = "Test변수") // 맵에 배치된 상태에서만 가능
	int32 Data7 = 70;

	UPROPERTY(BlueprintReadOnly, Category = "Test변수")	//블루프린트 쪽에서 읽기만 간으
	int32 Data8 = 80;

	UPROPERTY(BlueprintReadWrite, Category = "Test변수")		//블루프린트 쪽에서 읽고 쓰기 기능
	int32 Data9 = 90;

	UPROPERTY(VisibleAnywhere, Category = "Test변수")		//확인은 어디서든 할 수 있고 블루프린트에서 읽고 쓰기
	int32 Data10 = 100;
};
