// Fill out your copyright notice in the Description page of Project Settings.


#include "SOWStructTypes.h"

bool FPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

