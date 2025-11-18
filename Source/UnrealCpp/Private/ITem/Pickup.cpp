// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "NiagaraComponent.h"
#include "Player/InventoryOwner.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//루트 : 컬리젼 추가(box)
	//무기 외형 : 스켈레탈 메시
	//오버랩 영역 : 스피어 컬리전
	//이팩트 : 그대로 

	BaseRoot = CreateDefaultSubobject<USphereComponent>(TEXT("BaseRoot"));
	SetRootComponent(BaseRoot);
	BaseRoot->InitSphereRadius(1.0f);
	BaseRoot->SetSimulatePhysics(true);
	BaseRoot->BodyInstance.bLockXRotation = true;
	BaseRoot->BodyInstance.bLockYRotation = true;
	BaseRoot->SetCollisionProfileName(TEXT("BlockAllDynamic"));


	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BaseRoot);
	//bSimulatephysics //시뮬레이션 피직스 켬
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->AddRelativeRotation(FRotator(0, 0, -10.0f));


	PickupOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap"));
	PickupOverlap->SetupAttachment(BaseRoot);
	PickupOverlap->SetSphereRadius(100.0f);
	PickupOverlap->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(BaseRoot);

	PickupTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeline"));
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	//StartLocation = GetActorLocation();

	if (PickupOverlap)
	{
		PickupOverlap->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnPickupBeginOverlap);
	}

	if (PickupTimeline)
	{
		if(ScaleCurve)
		{
		FOnTimelineFloat UpdateDelegate;
		UpdateDelegate.BindUFunction(this, FName("OnTimelineUpdate"));
		PickupTimeline->AddInterpFloat(DistanceCurve, UpdateDelegate);

		FOnTimelineEvent FinishDelegate;
		FinishDelegate.BindUFunction(this, FName("OnTimelineFinished"));
		PickupTimeline->SetTimelineFinishedFunc(FinishDelegate);
		}

	PickupTimeline->SetPlayRate(1 / Duration);
	}
	bPickuped = false;
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddWorldRotation(FRotator(0, RotateSpeed*DeltaTime, 0));
}

//먹는처리
void APickup::OnPickup_Implementation(AActor* Target)
{
	if (!bPickuped)
	{
		//UE_LOG(LogTemp, Log, TEXT("OnPickup_Implementation 실행"));
		bPickuped = true;
		PickupOwner = Target;
		PickupStartLocation = Mesh->GetRelativeLocation() + GetActorLocation();	//mesh의 월드 위
		SetActorEnableCollision(false);	//이 액터와 포함하는 모든 컴포넌트의 충돌 정지
	/*	StartLocation = GetActorLocation();
		TargetLocation = Target->GetActorLocation() + FVector(0, 0, 50);*/

		BaseRoot->SetSimulatePhysics(false);
		PickupTimeline->PlayFromStart();	// 타임라인 시작
	}
}

void APickup::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Pickup Overlap"));
}

void APickup::OnTimelineUpdate(float Value)
{
	//타임라인 진행 시간(0~1)
	float CurrentTime = PickupTimeline->GetPlaybackPosition();

	//커브의 현재 값 받아오기
	float distanceValue = Value;
	//DistanceCurve ? DistanceCurve->GetFloatValue(CurrentTime);
	float heightValue = HeightCurve ? HeightCurve->GetFloatValue(CurrentTime): 0.0f;
	float ScaleValue = ScaleCurve ? ScaleCurve->GetFloatValue(CurrentTime): 1.0f;

	/*FVector NewLocation = FMath::Lerp(TargetLocation, StartLocation, Value);
	SetActorLocation(NewLocation);*/

	//커브값을 기준으로 새 위치와 스케일 계산
	FVector NewLocation = FMath::Lerp(PickupStartLocation, PickupOwner.Get()->GetActorLocation(), distanceValue);
	NewLocation += heightValue * PickupHeight * FVector::UpVector;
	Mesh->SetWorldLocation(NewLocation);

	FVector NewScale = FVector::One() * ScaleValue;
	SetActorScale3D(NewScale);

}

void APickup::OnTimelineFinished()
{
	// 자신을 먹은 대상에게 자기가 가지고 있는 무기를 알려줘야 함
	if (PickupOwner.IsValid() && PickupOwner->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(PickupOwner.Get(), PickupItem);
	}
	//Destroy();	//자기자신 삭제
}
