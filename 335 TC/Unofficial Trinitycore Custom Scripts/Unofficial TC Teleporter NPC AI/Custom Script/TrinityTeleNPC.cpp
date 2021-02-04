/*
Trinitycore Teleport NPC
Based loosely on custom npcs from the single player project
Re-Writen for Trinitycore Custom 335 by
SPP Dev MDic
 */

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameEventMgr.h"
#include "Player.h"
#include "WorldSession.h"

//INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `spell_school_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES (66666, 0, 0, 0, 0, 0, 16941, 0, 0, 0, 'Teleporter', 'Trinitycore', NULL, 0, 60, 60, 0, 35, 1, 1, 0.99206, 1, 0, 0, 2000, 2000, 1, 1, 1, 768, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 66, 'TC_TeleNPC', 12340);

class TC_TeleNPC : public CreatureScript
{
public:
    TC_TeleNPC() : CreatureScript("TC_TeleNPC") { }

    struct TC_TeleNPCAI : public ScriptedAI
    {
        TC_TeleNPCAI(Creature* creature) : ScriptedAI(creature) { }

        bool OnGossipHello(Player* player) override
        {
            if (player)
            {
                return OnGossipHello(player, me);
            }
            else
            {
            return false;
            }
        }

        static bool OnGossipHello(Player* player, Creature* creature)
        {
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport Menu", GOSSIP_SENDER_MAIN, 2);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            return OnGossipSelect(player, me, sender, action);
        }

        static bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
        {
            ClearGossipMenuFor(player);

            switch (action)
            {
            case 2: //Second menu (TELEPORT_MAIN) and it's cases
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface\\icons\\Spell_arcane_portalthunderbluff:25|t Cities", GOSSIP_SENDER_MAIN, 21);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface\\icons\\Spell_arcane_portalundercity:25|t Dungeons", GOSSIP_SENDER_MAIN, 22);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface\\icons\\Spell_arcane_portalorgrimmar:25|t Raids", GOSSIP_SENDER_MAIN, 23);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface\\icons\\Spell_arcane_portaldarnassus:25|t PvP Zones", GOSSIP_SENDER_MAIN, 24);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface\\icons\\Spell_arcane_portalstormwind:25|t Interesting places", GOSSIP_SENDER_MAIN, 25);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 21: //Second menu (TELEPORT_CITIES) and it's cases
                if (player->GetTeam() == ALLIANCE)
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Stormwind", GOSSIP_SENDER_MAIN, 211);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Ironforge", GOSSIP_SENDER_MAIN, 212);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleportation to Darnassus", GOSSIP_SENDER_MAIN, 213);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to the Exodar", GOSSIP_SENDER_MAIN, 214);
                }
                else
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Orgrimmar", GOSSIP_SENDER_MAIN, 215);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Thunder Bluff", GOSSIP_SENDER_MAIN, 216);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Undercity", GOSSIP_SENDER_MAIN, 217);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Silvermoon City", GOSSIP_SENDER_MAIN, 218);
                }
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Ratchet", GOSSIP_SENDER_MAIN, 219);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Everlook", GOSSIP_SENDER_MAIN, 2110);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Gadgetzan", GOSSIP_SENDER_MAIN, 2111);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Booty Bay", GOSSIP_SENDER_MAIN, 2112);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Shattrah", GOSSIP_SENDER_MAIN, 2113);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Argent Tournament", GOSSIP_SENDER_MAIN, 2114);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Teleport to Dalaran", GOSSIP_SENDER_MAIN, 2115);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 2);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 211: //Stormwind
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -8827.53f, 625.9f, 93.91f, 0.0f);
                break;
            case 212: //Ironforge
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -4924.07f, -951.95f, 501.55f, 5.40f);
                break;
            case 213: //Darnassus
                CloseGossipMenuFor(player);
                player->TeleportTo(1, 9947.52f, 2482.73f, 1316.21f, 0.0f);
                break;
            case 214: //Exodar
                CloseGossipMenuFor(player);
                player->TeleportTo(530, -3965.6f, -11653.5f, -138.8f, 0.0f);
                break;
            case 215: //Orgrimmar
                CloseGossipMenuFor(player);
                player->TeleportTo(1, 1552.5f, -4420.66f, 8.94802f, 0.0f);
                break;
            case 216: //Thunderbluff
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -1280.19f, 127.21f, 131.35f, 5.16f);
                break;
            case 217: //Undercity
                CloseGossipMenuFor(player);
                player->TeleportTo(0, 1560.05f, 240.213f, -43.1025f, 0.0f);
                break;
            case 218: //Silvermoon
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 9487.690f, -7279.2f, 14.2866f, 0.0f);
                break;
            case 219: //Ratchet
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -956.664f, -3754.709f, 5.3323f, 0.99f);
                break;
            case 2110: //Everlook
                CloseGossipMenuFor(player);
                player->TeleportTo(1, 6725.504f, -4621.73f, 720.843f, 4.78f);
                break;
            case 2111: //Gadgetzan
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -7177.149f, -3785.34f, 8.369f, 6.1f);
                break;
            case 2112: //Booty bay
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -14297.2f, 530.992f, 8.779f, 3.98f);
                break;
            case 2113: //Shattrah
                CloseGossipMenuFor(player);
                player->TeleportTo(530, -1838.16f, 5301.79f, -12.428f, 5.9517f);
                break;
            case 2114: //Argent Tournament
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 8515.89f, 629.25f, 547.396f, 1.5747f);
                break;
            case 2115: //Dalaran
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 5804.15f, 624.771f, 647.767f, 1.64f);
                break;
            case 22: //Second menu (TELEPORT_DUNGEON_INSTANCES) and it's cases
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Classic Dungeons", GOSSIP_SENDER_MAIN, 221);
                if (player->GetLevel() >= 58)
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Burning Crusade Dungeons", GOSSIP_SENDER_MAIN, 222);
                }
                if (player->GetLevel() >= 68)
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Wraith of the Lich King Dungeons", GOSSIP_SENDER_MAIN, 223);
                }
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 2);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 221: //Second menu (TELEPORT_DUNGEON_INSTANCES_CLASSIC)
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Dead Mines", GOSSIP_SENDER_MAIN, 2211);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Wailing Caverns", GOSSIP_SENDER_MAIN, 2212);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Blackfathom Deeps", GOSSIP_SENDER_MAIN, 2213);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Gnomregan", GOSSIP_SENDER_MAIN, 2214);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Razorfen Downs", GOSSIP_SENDER_MAIN, 2215);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Uldaman", GOSSIP_SENDER_MAIN, 2216);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Maraudon", GOSSIP_SENDER_MAIN, 2217);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Blackrock Depths", GOSSIP_SENDER_MAIN, 2218);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Dire Maul", GOSSIP_SENDER_MAIN, 2219);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Stratholme", GOSSIP_SENDER_MAIN, 22110);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Ragefire Chasm", GOSSIP_SENDER_MAIN, 22111);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Shadowfang Keep", GOSSIP_SENDER_MAIN, 22112);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Stormwind Stockade", GOSSIP_SENDER_MAIN, 22113);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Razorfen Kraul", GOSSIP_SENDER_MAIN, 22114);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Scarlet Monastery", GOSSIP_SENDER_MAIN, 22115);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Zul'Farrak", GOSSIP_SENDER_MAIN, 22116);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Sunken Temple", GOSSIP_SENDER_MAIN, 22117);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Lower Blackrock Spire", GOSSIP_SENDER_MAIN, 22118);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Scholomance", GOSSIP_SENDER_MAIN, 22119);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 22);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2211: //Deadmines
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -11212.04f, 1658.58f, 25.67f, 1.45f);
                break;
            case 2212: //Wailing caverns
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -722.53f, -2226.30f, 16.94f, 2.71f);
                break;
            case 2213: //Blackfathom Deeps
                CloseGossipMenuFor(player);
                player->TeleportTo(1, 4254.58f, 664.74f, -29.04f, 1.97f);
                break;
            case 2214: //Gnomeregan
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -5163.43f, 660.40f, 348.28f, 4.65f);
                break;
            case 2215: //Razorfen Downs
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -4645.08f, -2470.85f, 85.53f, 4.39f);
                break;
            case 2216: //Uldaman
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -6119.70f, -2957.30f, 204.11f, 0.03f);
                break;
            case 2217: //Maraudon
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -1433.33f, 2955.34f, 96.21f, 4.82f);
                break;
            case 2218: //Blackrock Depths
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -7301.03f, -913.19f, 165.37f, 0.08f);
                break;
            case 2219: //Dire Maul
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -3982.47f, 1127.79f, 161.02f, 0.05f);
                break;
            case 22110: //Stratholme
                CloseGossipMenuFor(player);
                player->TeleportTo(0, 3263.54f, -3379.46f, 143.59f, 0.00f);
                break;
            case 22111: //Ragefire Chasm
                if (player->GetTeam() == ALLIANCE)
                {
                    CloseGossipMenuFor(player);
                    player->Whisper("Take it easy, friend. You're Alliance, and I won't let you teleport to Orgrimmar.", LANG_UNIVERSAL, NULL);
                }
                else
                {
                    CloseGossipMenuFor(player);
                    player->TeleportTo(1, 1800.53f, -4394.68f, -17.93f, 5.49f);
                }
                break;
            case 22112: //Shadowfang Keep
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -254.47f, 1524.68f, 76.89f, 1.56f);
                break;
            case 22113: //Stormwind Stockade
                if (player->GetTeam() == ALLIANCE)
                {
                    CloseGossipMenuFor(player);
                    player->TeleportTo(0, -8769.76f, 813.08f, 97.63f, 2.26f);
                }
                else
                {
                    CloseGossipMenuFor(player);
                    player->Whisper("Easy, friend. You are a Horde, and I will not have you teleported to Stormwind.", LANG_UNIVERSAL, NULL);
                }
                break;
            case 22114: //Razorfen Kraul
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -4484.04f, -1739.40f, 86.47f, 1.23f);
                break;
            case 22115: //Scarlet Monastery
                CloseGossipMenuFor(player);
                player->TeleportTo(0, 2843.89f, -693.74f, 139.32f, 5.11f);
                break;
            case 22116: //Zul'Farrak
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -6839.39f, -2911.03f, 8.87f, 0.41f);
                break;
            case 22117: //Sunken Temple
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -10346.92f, -3851.90f, -43.41f, 6.09f);
                break;
            case 22118: //Lower Blackrock Spire
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -7535.43f, -1212.04f, 285.45f, 5.29f);
                break;
            case 22119: //Scholomance
                CloseGossipMenuFor(player);
                player->TeleportTo(0, 1219.01f, -2604.66f, 85.61f, 0.50f);
                break;
            case 222: //Second menu (TELEPORT_DUNGEON_INSTANCES_BC)
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Shattered Halls, Hellfire Ramparts, The Blood Furnace", GOSSIP_SENDER_MAIN, 2221);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Steamvault, Underbog, The Slave Pens", GOSSIP_SENDER_MAIN, 2222);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Mana-Tombs, Sethekk Halls, Shadow Labyrinth, Auchenai Crypts", GOSSIP_SENDER_MAIN, 2223);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Mechanar", GOSSIP_SENDER_MAIN, 2224);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Botanica", GOSSIP_SENDER_MAIN, 2225);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Arcatraz", GOSSIP_SENDER_MAIN, 2226);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Magister's Terrace", GOSSIP_SENDER_MAIN, 2227);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Opening of the Dark Portal, The Escape From Durnholde", GOSSIP_SENDER_MAIN, 2228);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 22);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2221: //The Shattered Halls, Hellfire Ramparts, The Blood Furnace +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, -305.816223f, 3056.401611f, -2.473183f, 2.01f);
                break;
            case 2222: //The Steamvault, Underbog, The Slave Pens +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 734.39f, 6866.09f, -69.72f, 0.00f);
                break;
            case 2223: //Mana-Tombs, Sethekk Halls, Shadow Labyrinth, Auchenai Crypts +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, -3322.89f, 4941.024f, -101.21f, 3.72f);
                break;
            case 2224: //The Mechanar +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 2873.38f, 1554.49f, 252.159f, 3.82f);
                break;
            case 2225: //The Botanica +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 3403.67f, 1491.30f, 182.543f, 5.59f);
                break;
            case 2226: //The Arcatraz +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 3307.53f, 1343.1f, 505.56f, 5.08f);
                break;
            case 2227: //Magister's Terrace +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 12888.501f, -7317.7f, 65.508f, 4.82f);
                break;
            case 2228: //Opening of the Dark Portal, The Escape From Durnholde +
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -8484.37f, -4491.85f, -212.96f, 1.61f);
                break;
            case 223: //Second menu (TELEPORT_DUNGEON_INSTANCES_LK)
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Utgarde Keep", GOSSIP_SENDER_MAIN, 2231);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Drak'Tharon Keep", GOSSIP_SENDER_MAIN, 2232);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Ahn'kahet: The Old Kingdom", GOSSIP_SENDER_MAIN, 2233);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Violet Hold", GOSSIP_SENDER_MAIN, 2234);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Halls of Stone", GOSSIP_SENDER_MAIN, 2235);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Oculus, The Nexus", GOSSIP_SENDER_MAIN, 2236);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Azjol-Nerub", GOSSIP_SENDER_MAIN, 2237);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Culling of Stratholme", GOSSIP_SENDER_MAIN, 2238);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Gundrak", GOSSIP_SENDER_MAIN, 2239);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Halls of Lightning", GOSSIP_SENDER_MAIN, 22310);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Utgarde Pinnacle", GOSSIP_SENDER_MAIN, 22311);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Trial of the Champion", GOSSIP_SENDER_MAIN, 22312);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Forge of Souls, Pit of Saron, Halls of Reflection", GOSSIP_SENDER_MAIN, 22313);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 22);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2231: //Utgarde Keep +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 1219.720f, -4865.28f, 41.25f, 0.31f);
                break;
            case 2232: //Drak'Tharon Keep +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 4450.860f, -2045.25f, 162.83f, 0.00f);
                break;
            case 2233: //Ahn'kahet: The Old Kingdom +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3646.760f, 2045.17f, 1.79f, 4.37f);
                break;
            case 2234: //The Violet Hold +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 5679.820f, 486.80f, 652.40f, 4.08f);
                break;
            case 2235: //Halls of Stone +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 8922.68f, -990.62f, 1039.387f, 1.60f);
                break;
            case 2236: //The Oculus, The Nexus +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3808.83f, 6967.47f, 100.1f, 0.468f);
                break;
            case 2237: //Azjol-Nerub +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3675.430f, 2169.00f, 35.90f, 2.29f);
                break;
            case 2238: //The Culling of Stratholme +
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -8484.37f, -4491.85f, -212.96f, 1.61f);
                break;
            case 2239: //Gundrak +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 6937.540f, -4455.98f, 450.68f, 1.00f);
                break;
            case 22310: //Halls of Lightning +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 9131.15f, -1334.98f, 1061.533f, 5.517f);
                break;
            case 22311: //Utgarde Pinnacle +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 1245.690f, -4856.59f, 216.86f, 3.45f);
                break;
            case 22312: //Trial of the Champion +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 8604.999f, 791.69f, 558.26f, 3.14f);
                break;
            case 22313: //Forge of Souls, Pit of Saron, Halls of Reflection +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 5635.173f, 2048.567f, 798.10f, 4.67f);
                break;
            case 23: //Third menu (TELEPORT_RAID_INSTANCES) and it's cases
                if (player->GetLevel() >= 50)
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Classic Raids", GOSSIP_SENDER_MAIN, 231);
                }
                if (player->GetLevel() >= 65)
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Burning Crusade Raids", GOSSIP_SENDER_MAIN, 232);
                }
                if (player->GetLevel() >= 80)
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Wraith of the Lich King Raids", GOSSIP_SENDER_MAIN, 233);
                }
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 2);
                
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                return true;
                break;
            case 231: //Third menu (TELEPORT_RAID_INSTANCES_CLASSIC) and it's cases
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Molten Core", GOSSIP_SENDER_MAIN, 2311);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Blackwing Lair", GOSSIP_SENDER_MAIN, 2312);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Ruins of Ahn'Qiraj, Temple of Ahn'Qiraj", GOSSIP_SENDER_MAIN, 2313);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Zul'Gurub", GOSSIP_SENDER_MAIN, 2314);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 23);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2311: //Molten Core +
                CloseGossipMenuFor(player);
                player->TeleportTo(230, 1117.7214f, -458.81399f, -102.87f, 3.56f);
                break;
            case 2312: //Blackwing Lair +
                CloseGossipMenuFor(player);
                player->TeleportTo(229, 166.0446f, -455.714f, 121.97f, 4.74f);
                break;
            case 2313: //Ruins of Ahn'Qiraj, Temple of Ahn'Qiraj +
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -8164.7f, 1526.07f, 4.1949f, 2.84f);
                break;
            case 2314: //Zul'Gurub +
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -11916.85f, -1207.372f, 92.287f, 4.72f);
                break;
            case 232: //Third menu (TELEPORT_RAID_INSTANCES_BC) and it's cases
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Karazhan", GOSSIP_SENDER_MAIN, 2321);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Gruul's Lair", GOSSIP_SENDER_MAIN, 2322);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Magtheridon's Lair", GOSSIP_SENDER_MAIN, 2323);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Zul'Aman", GOSSIP_SENDER_MAIN, 2324);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "oilfang Reservoir: Serpentshrine Cavern", GOSSIP_SENDER_MAIN, 2325);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Tempest Keep: The Eye", GOSSIP_SENDER_MAIN, 2326);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Caverns of Time: Battle for Mount Hyjal", GOSSIP_SENDER_MAIN, 2327);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Black Temple", GOSSIP_SENDER_MAIN, 2328);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Sunwell Plateau", GOSSIP_SENDER_MAIN, 2329);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 23);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2321: //Karazhan +
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -11118.8f, -2010.84f, 47.0807f, 0.0f);
                break;
            case 2322: //Gruul's Lair +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 3539.007568f, 5082.357910f, 1.691071f, 0.0f);
                break;
            case 2323: //Magtheridon's Lair +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, -363.460f, 3173.506f, -97.280f, 5.26f);
                break;
            case 2324: //Zul'Aman +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 6846.95f, -7954.5f, 170.028f, 4.61501f);
                break;
            case 2325: //Coilfang Reservoir: Serpentshrine Cavern +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 734.39f, 6866.09f, -69.72f, 0.00f);
                break;
            case 2326: //Tempest Keep: The Eye +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 3090.6503f, 1408.584f, 189.652f, 4.61f);
                break;
            case 2327: //Caverns of Time: Battle for Mount Hyjal +
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -8484.37f, -4491.85f, -212.96f, 1.61f);
                break;
            case 2328: //Black Temple +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, -3610.719482f, 324.987579f, 37.400028f, 3.282981f);
                break;
            case 2329: //Sunwell Plateau +
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 12574.1f, -6774.81f, 15.0904f, 3.13788f);
                break;
            case 233: //Third menu (TELEPORT_RAID_INSTANCES_LK) and it's cases
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Eye of Eternity", GOSSIP_SENDER_MAIN, 2331);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Onyxia's Lair", GOSSIP_SENDER_MAIN, 2332);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Vault of Archavon (Outside of the Vault)", GOSSIP_SENDER_MAIN, 2333);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Naxxramas", GOSSIP_SENDER_MAIN, 2334);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Ulduar", GOSSIP_SENDER_MAIN, 2335);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Trial of the Crusader", GOSSIP_SENDER_MAIN, 2336);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Ice Crown Citadel", GOSSIP_SENDER_MAIN, 2337);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Ruby Sanctum, Obsidian Sanctum", GOSSIP_SENDER_MAIN, 2338);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 23);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2331: //The Eye of Eternity +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3808.83f, 6967.47f, 100.1f, 0.468f);
                break;
            case 2332: //Onyxia's Lair +
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -4672.895f, -3664.67f, 46.648f, 4.189f);
                break;
            case 2333: //Vault of Archavon +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 5128.978f, 2840.344f, 407.17f, 0.0255f);
                break;
            case 2334: //Naxxramas +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3668.719f, -1262.460f, 243.63f, 2.304f);
                break;
            case 2335: //Ulduar +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 9330.955f, -1114.82f, 1245.146f, 6.28f);
                break;
            case 2336: //Trial of the Crusader +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 8515.889f, 629.25f, 547.395f, 1.574f);
                break;
            case 2337: //Ice crown citadel +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 5873.819f, 2110.979f, 636.0109f, 3.55f);
                break;
            case 2338: //Ruby Sanctum, Obsidian Sanctum +
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3624.78f, 280.987f, -120.145f, 3.2f);
                break;
            case 24: //Fourth menu (TELEPORT_PVPZONES) and it's cases
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Azshara Crater", GOSSIP_SENDER_MAIN, 241);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Circle of Blood", GOSSIP_SENDER_MAIN, 242);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Gurubashi Arena", GOSSIP_SENDER_MAIN, 243);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Maul", GOSSIP_SENDER_MAIN, 244);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The Ring of Trials", GOSSIP_SENDER_MAIN, 245);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 2);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 241: //Azshara Crater
                CloseGossipMenuFor(player);
                player->TeleportTo(37, 852.267f, 339.953f, 269.508f, 4.23f);
                break;
            case 242: //Circle of Blood
                CloseGossipMenuFor(player);
                player->TeleportTo(530, 2756.722f, 5850.075f, -2.239f, 1.12f);
                break;
            case 243: //Gurubashi Arena
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -13259.043f, 164.976f, 35.49f, 1.1f);
                break;
            case 244: //The Maul
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -3660.064f, 1092.909f, 162.115005f, 3.15f);
                break;
            case 245: //The Ring of Trials
                CloseGossipMenuFor(player);
                player->TeleportTo(530, -2102.818f, 6743.5473f, -3.536184f, 5.5f);
                break;
            case 25: //Fifth menu (TELEPORT_PLACESOFINTEREST) and it's cases
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Easter Eggs", GOSSIP_SENDER_MAIN, 251);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Hidden Locations", GOSSIP_SENDER_MAIN, 252);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 2);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 251: //Easter Eggs
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Trapdoor from Lost TV Series", GOSSIP_SENDER_MAIN, 2511);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 25);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2511: //Trapdoor from lost                    
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 6208.752f, 5238.3823f, -97.823f, 1.757f);
                break;
            case 252: //Hidden Locations
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Ahn Qiraj Hall ", GOSSIP_SENDER_MAIN, 2521);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Old Ironforge Map ", GOSSIP_SENDER_MAIN, 2522);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Tanaris Islands", GOSSIP_SENDER_MAIN, 2523);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Wetlands, Mountain Village", GOSSIP_SENDER_MAIN, 2524);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Dalaran Crater", GOSSIP_SENDER_MAIN, 2525);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The highest point of Blackrock Mountain", GOSSIP_SENDER_MAIN, 2526);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Kael Thalas", GOSSIP_SENDER_MAIN, 2527);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Mount Hyjal", GOSSIP_SENDER_MAIN, 2528);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "The highest point of Ironforge", GOSSIP_SENDER_MAIN, 2529);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Aquarium", GOSSIP_SENDER_MAIN, 2530);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Mograine Tomb", GOSSIP_SENDER_MAIN, 2531);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Heliport", GOSSIP_SENDER_MAIN, 2532);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Frostwhisper Gorge ", GOSSIP_SENDER_MAIN, 2533);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Emerald Forest", GOSSIP_SENDER_MAIN, 2535);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Emerald Valley", GOSSIP_SENDER_MAIN, 2536);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Emerald Mountain", GOSSIP_SENDER_MAIN, 2537);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Virgin field", GOSSIP_SENDER_MAIN, 2538);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Back", GOSSIP_SENDER_MAIN, 25);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2521: //Ahn Qiraj Hall             
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -8516.652344f, 2016.7146f, 104.748749f, 3.557f);
                break;
            case 2522: //Old Ironforge Map                   
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -4817.906738f, -973.383911f, 464.709106f, 3.821704f);
                break;
            case 2523: //Tanaris Islands                  
                CloseGossipMenuFor(player);
                player->TeleportTo(1, -11839.35f, -4754.812f, 6.23413f, 1.757f);
                break;
            case 2524: //Wetlands, Mountain Village               
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -4053.333f, -1260.901f, 146.313f, 1.757f);
                break;
            case 2525: //Dalaran Crater
                CloseGossipMenuFor(player);
                player->TeleportTo(0, 191.492950f, 319.546478f, 51.883720f, 1.329635f);
                break;
            case 2526: //The highest point of Blackrock Mountain
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -7468.000000f, -1082.0000f, 896.779968f, 2.727f);
                break;
            case 2527: // Kael Thalas
                CloseGossipMenuFor(player);
                player->TeleportTo(0, 4266.936523f, -2806.1621f, 5.232010f, 0.978f);
                break;
            case 2528: // Mount Hyjal
                CloseGossipMenuFor(player);
                player->TeleportTo(1, 4674.879883f, -3638.3701f, 965.263977f, 3.546f);
                break;
            case 2529: // The highest point of Ironforge           
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -4796.000000f, -1001.0000f, 895.838318f, 3.452f);
                break;
            case 2530: //Aquarium           
                CloseGossipMenuFor(player);
                player->TeleportTo(369, 22.635477f, 1338.2740f, -125.199516f, 4.884f);
                break;
            case 2531: //Mograine Tomb           
                CloseGossipMenuFor(player);
                player->TeleportTo(0, -11077.514648f, -1792.3932f, 52.740955f, 3.419f);
                break;
            case 2532: //Heliport            
                CloseGossipMenuFor(player);
                player->TeleportTo(1, 1980.703369f, 1529.4971f, 234.253830f, 3.818f);
                break;
            case 2533: //Frostwhisper Gorge           
                CloseGossipMenuFor(player);
                player->TeleportTo(1, 5087.804688f, -5115.4434f, 930.923157f, 4.565f);
                break;
            case 2535: //Emerald Forest           
                CloseGossipMenuFor(player);
                player->TeleportTo(169, 2965.000000f, -3040.0000f, 98.556290f, 5.067f);
                break;
            case 2536: //Emerald Valley           
                CloseGossipMenuFor(player);
                player->TeleportTo(169, -3824.000000f, 3367.0000f, 133.387482f, 5.063f);
                break;
            case 2537: //Emerald mountains           
                CloseGossipMenuFor(player);
                player->TeleportTo(169, 3225.000000f, 3046.0000f, 23.221907f, 5.063f);
                break;
            case 2538: //Virgin field           
                CloseGossipMenuFor(player);
                player->TeleportTo(169, -1650.000000f, -577.0000f, 119.326241f, 5.063f);
                break;
            default:
                break;
            }
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new TC_TeleNPCAI(creature);
    }
};

void AddSC_Trinitycore_TeleNPC()
{
    new TC_TeleNPC();
}
