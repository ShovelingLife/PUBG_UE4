// Fill out your copyright notice in the Description page of Project Settings.


#include "PUBG_gamemode.h"
#include "Custom_player.h"
#include "Custom_player_controller.h"

APUBG_gamemode::APUBG_gamemode()
{
    DefaultPawnClass      = ACustom_player::StaticClass();
    PlayerControllerClass = ACustom_player_controller::StaticClass();
}