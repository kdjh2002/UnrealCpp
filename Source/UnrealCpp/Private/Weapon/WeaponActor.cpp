// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "Player/ActionCharacter.h"
#include "Player/StatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetupAttachment(root);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	WeaponSlashEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	WeaponSlashEffect->SetupAttachment(WeaponMesh);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AWeaponActor::OnWeaponBeginOverlap);

	
}


void AWeaponActor::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	float finalDamage = Damage;
	AController* instigator = nullptr;
	if (WeaponOwner.IsValid())
	{
		if (WeaponOwner == OtherActor)	//내가 오버랩될 떄는 무시
			return;

		if (WeaponOwner->GetStatusComponent() != nullptr)	//스테이터스 
		{
			finalDamage += WeaponOwner->GetStatusComponent()->GetAttackPower();
		}
		instigator = WeaponOwner->GetController();
	}
	UE_LOG(LogTemp, Log, TEXT("Overlapped : %s"), *OtherActor->GetName());
	UGameplayStatics::ApplyDamage(OtherActor, finalDamage, instigator, this, DamageType);
}

void AWeaponActor::WeaponActivate(bool bActivate)
{
	//SetActorHiddenInGame(!bActivate);	// 무기는 비지빌리티만 수정하면 된다.
	if (bActivate)
	{
		// 무기 활성화
		AttachToComponent(
			WeaponOwner->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("hand_rSocket"));		// 플레이어의 손에 붙이기
		SetActorHiddenInGame(false);	// 게임에서 보이게 하기
		//SetActorEnableCollision(true);
		//SetActorTickEnabled(true);
		OnWeaponActivate();
	}
	else
	{
		// 무기 비활성화
		SetActorHiddenInGame(true);
		AttachToComponent(
			WeaponOwner->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("root"));
		SetActorRelativeLocation(FVector(0.0f, 0.0f, -10000.0f));	// 안보이는 곳에 배치
		//SetActorEnableCollision(false);
		//SetActorTickEnabled(false);

		//컬리전과 트레일도 끄기
		AttackEnable(false);
		TrailEnable(false);
		
		OnWeaponDeactivate();
	}
		//보일것이냐 말거냐- 비지빌리티 - ㅇ
		//충돌할거냐 말거냐 - 충돌 - ㄴ
		//액터의 틱 돌릴거냐 말거냐 - 액터의 틱 - ㄴ
		//액터의 컴포넌트들의 틱을 돌릴거냐 말거냐 - 액터의 컴포넌트 틱 - ㄴ
		//물리 시뮬레이션 켤꺼냐 말꺼냐 - ㄴ
		//타이머 켤꺼냐말거냐 - ㄴ
		//오디오 켤거냐말거냐 - ㄴ
		//파티클 - ㄴ
		//애니메이션 - ㄴ
		//movement컴포넌트 - ㄴ
		//TSet<UActorComponent*>Components = GetComponents();
		//for (auto comp : components)
		//{
		//	//자식 컴포넌트 전부 끄기
		//}

}

void AWeaponActor::AttackEnable(bool bEnable)
{
	if (bEnable)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeaponActor::TrailEnable(bool bEnable)
{
	if (bEnable)	//활성화됨
	{
		//재시작을 해야하는경우는
		WeaponSlashEffect->Activate(true);		//나이아가라 처음부터 재시작
	}
	else 
	{	//루프드레이션을 이어서 할수있는 버그생김
		WeaponSlashEffect->Deactivate();		//재생중이던 나이아가라 정지
	}
}

void AWeaponActor::OnWeaponPickuped(int InCount)
{
	// 무기 획득 시 처리할 기본 로직
	//UE_LOG(LogTemp, Log, TEXT("Weapon [%s] Pickuped! Count = %d"), *GetName(), InCount);
}


void AWeaponActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//CDO(Class Default Object)의 설정대로 초기화 된 이후(= overlapOnlyPawn 설정 이후)
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

