// Fill out your copyright notice in the Description page of Project Settings.


#include "PUBG_gamemode.h"
#include "Custom_player.h"
#include "Sports_car.h"
#include "Custom_player_controller.h"

APUBG_gamemode::APUBG_gamemode()
{
    //DefaultPawnClass      = ACustom_player::StaticClass();
    DefaultPawnClass = ASports_car::StaticClass();
    PlayerControllerClass = ACustom_player_controller::StaticClass();
}