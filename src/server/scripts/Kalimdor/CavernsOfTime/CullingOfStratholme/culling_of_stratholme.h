/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEF_CULLING_OF_STRATHOLME_H
#define DEF_CULLING_OF_STRATHOLME_H

#include "Creature.h"

#define DataHeader "CS"
#define CoSScriptName "instance_culling_of_stratholme"

enum ProgressStates : uint32
{
    JUST_STARTED            = 0x00001, // dungeon just started, crate count not visible yet; pending chromie interaction
    CRATES_IN_PROGRESS      = 0x00002, // freshly started dungeon, players are revealing scourge crates
    CRATES_DONE             = 0x00004, // all crates revealed, chromie spawns at Stratholme entrance; waiting for player input to begin first RP event
    UTHER_TALK              = 0x00008, // RP event in progress, Uther+Arthas talking
    PURGE_PENDING           = 0x00010, // RP event done, pending player input to start wave event
    PURGE_STARTING          = 0x00020, // Arthas entering Stratholme, RP sequence with Mal'ganis
    WAVES_IN_PROGRESS       = 0x00040, // first section is underway, players are battling waves
    WAVES_DONE              = 0x00080, // wave section completed; Arthas moving to take position in front of Stratholme Town Hall
    TOWN_HALL_PENDING       = 0x00100, // Arthas has reached the Town Hall; pending player input to begin escort section
    TOWN_HALL               = 0x00200, // now escorting Arthas through Stratholme Town Hall
    TOWN_HALL_COMPLETE      = 0x00400, // Town Hall event complete, third boss defeated; pending player input to begin gauntlet transition
    GAUNTLET_TRANSITION     = 0x00800, // Arthas is leading players through the secret passage from Town Hall to the gauntlet
    GAUNTLET_PENDING        = 0x01000, // Pending player input to begin escorting Arthas through the final gauntlet section
    GAUNTLET_IN_PROGRESS    = 0x02000, // Arthas is being escorted through the gauntlet section
    GAUNTLET_COMPLETE       = 0x04000, // Arthas has reached the end of the gauntlet section; player input pending to begin Mal'ganis encounter
    MALGANIS_IN_PROGRESS    = 0x08000, // Arthas has moved into the final square and Mal'ganis encounter begins
    COMPLETE                = 0x10000, // Mal'ganis encounter is completed; dungeon over

    ALL                     = 0x1FFFF
};

ProgressStates GetStableStateFor(ProgressStates const state); // defined by instance script
Position const& GetArthasSnapbackFor(ProgressStates state); // defined by arthas script

enum InstanceData
{
    DATA_MEATHOOK,
    DATA_SALRAMM,
    DATA_EPOCH,
    DATA_MAL_GANIS,
    DATA_INFINITE_CORRUPTOR,
    NUM_BOSS_ENCOUNTERS,

    DATA_INSTANCE_PROGRESS = NUM_BOSS_ENCOUNTERS, // GET only
    DATA_GM_RECALL,        // sent by chromie #1 in response to GM recall command (teleport all to arthas)
    DATA_GM_OVERRIDE,      // sent by chromie #1 in response to GM instance state change commands
    DATA_ARTHAS_DIED,      // failure signal, sent by arthas AI on death - regress instance
    DATA_CRATES_START,     // sent by chromie #1 creature script to initiate crate phase
    DATA_CRATE_REVEALED,   // sent by crate helper AI to trigger re-check of crate status
    DATA_UTHER_START,      // sent by chromie #2 creature script to initiate uther RP sequence
    DATA_UTHER_FINISHED,   // sent by arthas AI to signal transition to pre-purge
    DATA_SKIP_TO_PURGE,    // sent by chromie #1 creature script to skip straight to start of purge
    DATA_START_PURGE,      // sent by arthas creature script to begin pre-purge RP event
    DATA_START_WAVES,      // sent by arthas AI to begin wave event
    DATA_NOTIFY_DEATH,     // sent by wave mob AI to instance script on death
    DATA_REACH_TOWN_HALL,  // sent by arthas AI once he reaches stratholme town hall
    DATA_START_TOWN_HALL,  // sent by arthas creature script to begin town hall sequence
    DATA_TOWN_HALL_DONE,   // sent by arthas AI once Epoch is defeated
    DATA_TO_GAUNTLET,      // sent by arthas creature script to begin gauntlet transition
    DATA_GAUNTLET_REACHED, // sent by arthas AI once he arrives at the beginning of the gauntlet section
    DATA_START_GAUNTLET,   // sent by arthas creature script to begin gauntlet escort
    DATA_GAUNTLET_DONE,    // sent by arthas AI once he arrives at the end of the gauntlet section
    DATA_START_MALGANIS,   // sent by arthas creature script to begin mal'ganis event
    DATA_MALGANIS_DONE     // sent by arthas AI once mal'ganis outro is over
};

// these are sent by instance AI to creatures; they are passed as negative values to avoid conflicts with creature script specific actions
enum InstanceActions
{
    ACTION_PROGRESS_UPDATE = 1,
    ACTION_CORRUPTOR_LEAVE,
    ACTION_START_RP_EVENT1,   // Arthas/Uther chat in front of town
    ACTION_START_RP_EVENT2,   // Arthas/Mal'ganis chat at entrance
    ACTION_START_RP_EVENT3,   // Town Hall sequence
    ACTION_START_RP_EVENT4_1, // Bookcase transition sequence
    ACTION_START_RP_EVENT4_2, // Gauntlet escort phase
    ACTION_START_RP_EVENT5    // Mal'ganis encounter
};

enum InstanceEntries
{
    NPC_ARTHAS              =  26499,
    GO_HIDDEN_PASSAGE       = 188686,
    SPAWNGRP_CHROMIE_MID    =     52,
    SPAWNGRP_CRATE_HELPERS  =     53,
    SPAWNGRP_GAUNTLET_TRASH =     54,
    SPAWNGRP_UNDEAD_TRASH   =     55,
    SPAWNGRP_RESIDENTS      =     56
};

template <class AI, class T>
inline AI* GetCullingOfStratholmeAI(T* obj)
{
    return GetInstanceAI<AI>(obj, CoSScriptName);
}

#endif
