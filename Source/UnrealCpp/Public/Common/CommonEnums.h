// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
//class UNREALCPP_API WeaponEnum
//{
//public:
//	WeaponEnum();
//	~WeaponEnum();
//};

UENUM(BlueprintType)
enum class EItemCode : uint8
{
	BasicWeapon		UMETA(DisplayName = "기본무기(도끼)"), 
	Sword			UMETA(DisplayName = "대검"),
	Hammer			UMETA(DisplayName = "망치"),
};

//비트플래그용 enum, 에디터에서 편집가능
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditer = "true"))
enum class EBitFlag : uint8
{
	None = 0,
	Test1 = 1 << 0,
	Test2 = 1 << 1,
	Test3 = 1 << 2,
	Test4 = 1 << 3,
	Test5 = 1 << 4
};