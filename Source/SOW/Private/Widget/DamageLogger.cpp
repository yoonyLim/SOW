// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DamageLogger.h"

UDamageLogger::UDamageLogger()
{
	BaseDamage = 0;
	FinalDamage = 0;
}

UDamageLogger::UDamageLogger(float b, float f)
{
	BaseDamage = b;
	FinalDamage = f;
}

void UDamageLogger::SetLoggerValue(float b, float f)
{
	BaseDamage = b;
	FinalDamage = f;
}

void UDamageLogger::GetLoggerValue(float& OutBase, float& OutFinal) const
{
	OutBase = BaseDamage;
	OutFinal = FinalDamage;
}
