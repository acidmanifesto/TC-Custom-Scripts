/*
Equip Changer NPC inspired and written originally by Smrtokvitek
https://github.com/TrinityCore/TrinityCoreCustomChanges/issues/13
Changes Player's Equipement
Modernized and rewritten\overhauled (Because i didnt like the old version at all) for up to date recompile on TC 335 by SPP DEV MDIC
Requested by TC DEV Aokromes
*/

#include "Chat.h"
#include "Config.h"
#include "Creature.h"
#include "GameEventMgr.h"
#include "Item.h"
#include "ItemEnchantmentMgr.h"
#include "Player.h"
#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "WorldSession.h"

//INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `spell_school_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES (77777, 0, 0, 0, 0, 0, 16941, 0, 0, 0, 'Level 80 Equipment Exchanger', 'Trinitycore', NULL, 0, 60, 60, 0, 35, 1, 1, 0.99206, 1, 0, 0, 2000, 2000, 1, 1, 1, 768, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 66, 'npc_equip', 12340);

class Trinitycore_Equipment_Exchange_announce : public PlayerScript
{
public:
    Trinitycore_Equipment_Exchange_announce() : PlayerScript("Trinitycore_Equipment_Exchange_announce") { }

    void OnLogin(Player* Player, bool /*firstLogin*/) override
    {
        if (sConfigMgr->GetBoolDefault("Trinitycore.Equipment.Exchange.Announce.enable", true))
        {
            ChatHandler(Player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Trinitycore Equipment Exchange NPC |rmodule.");
        }
    }
};

class npc_equip : public CreatureScript
{
public:

    npc_equip() : CreatureScript("npc_equip") { }

    struct npc_equipAI : public ScriptedAI
    {
        npc_equipAI(Creature* creature) : ScriptedAI(creature) { }

        bool OnGossipHello(Player* player) override
        {
            if (player->IsInCombat())
            {
                ClearGossipMenuFor(player);
                ChatHandler(player->GetSession()).PSendSysMessage("You are still in combat!");
                return true;
            }
            else
            {
                return OnGossipHello(player, me);
            }
        }

        static bool OnGossipHello(Player* player, Creature* creature)
        {
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Menu", GOSSIP_SENDER_MAIN, 11);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            return OnGossipSelect(player, me, sender, action);
        }

        bool OnGossipSelect(Player* player, Creature* _creature, uint32 sender, uint32 uiAction)
        {
            const int32 EXC1 = sConfigMgr->GetIntDefault("Equipment.Exchange.Gold", 100);
            Item* headItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, 0);
            Item* handsItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, 9);
            Item* chestItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, 4);
            Item* legsItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, 6);
            Item* shoulderItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, 2);
            std::ostringstream messageDK;
            messageDK << "Death Knight Equipment Exchange for -" << EXC1 << " Gold.";
            std::ostringstream messageDR;
            messageDR << "Druid Equipment Exchange for  -" << EXC1 << " Gold.";
            std::ostringstream messageHU;
            messageHU << "Hunter Equipment Exchange for -" << EXC1 << " Gold.";
            std::ostringstream messageMA;
            messageMA << "Mage Equipement Exchange for -" << EXC1 << " Gold.";
            std::ostringstream messagePA;
            messagePA << "Paladin Equipment Exchange for  -" << EXC1 << " Gold.";
            std::ostringstream messagePR;
            messagePR << "Priest Equipment Exchange for -" << EXC1 << " Gold.";
            std::ostringstream messageRO;
            messageRO << "Rogue Equipment Exchange for -" << EXC1 << " Gold.";
            std::ostringstream messageSH;
            messageSH << "Shaman Equipment Exchange for -" << EXC1 << " Gold.";
            std::ostringstream messageWL;
            messageWL << "Warlock Equipment Exchange for -" << EXC1 << " Gold.";
            std::ostringstream messageWA;
            messageWA << "Warrior Equipment Exchange for -" << EXC1 << " Gold.";
            ClearGossipMenuFor(player);

            switch (uiAction)
            {

            case 11:

                if (player->GetLevel() == 80)
                {
                    if (player->GetClass() == CLASS_DEATH_KNIGHT)
                    {
                        AddGossipItemFor(player, 10, messageDK.str(), GOSSIP_SENDER_MAIN, 1);
                    }
                    if (player->GetClass() == CLASS_DRUID)
                    {
                        AddGossipItemFor(player, 10, messageDR.str(), GOSSIP_SENDER_MAIN, 2);
                    }
                    if (player->GetClass() == CLASS_HUNTER)
                    {
                        AddGossipItemFor(player, 10, messageHU.str(), GOSSIP_SENDER_MAIN, 3);
                    }
                    if (player->GetClass() == CLASS_MAGE)
                    {
                        AddGossipItemFor(player, 10, messageMA.str(), GOSSIP_SENDER_MAIN, 4);
                    }
                        if (player->GetClass() == CLASS_PALADIN)
                    {
                        AddGossipItemFor(player, 10, messagePA.str(), GOSSIP_SENDER_MAIN, 5);
                    }
                    if (player->GetClass() == CLASS_PRIEST)
                    {
                        AddGossipItemFor(player, 10, messagePR.str(), GOSSIP_SENDER_MAIN, 6);
                    }
                    if (player->GetClass() == CLASS_ROGUE)
                    {
                        AddGossipItemFor(player, 10, messageRO.str(), GOSSIP_SENDER_MAIN, 7);
                    }
                    if (player->GetClass() == CLASS_SHAMAN)
                    {
                        AddGossipItemFor(player, 10, messageSH.str(), GOSSIP_SENDER_MAIN, 8);
                    }
                    if (player->GetClass() == CLASS_WARLOCK)
                    {
                        AddGossipItemFor(player, 10, messageWA.str(), GOSSIP_SENDER_MAIN, 9);
                    }
                    if (player->GetClass() == CLASS_WARRIOR)
                    {
                        AddGossipItemFor(player, 10, messageWA.str(), GOSSIP_SENDER_MAIN, 10);
                    }
                    AddGossipItemFor(player, 9, "Exit", GOSSIP_SENDER_MAIN, 1111);
                }
                else
                {
                    return false;
                }
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 1111://close
                CloseGossipMenuFor(player);
                break;

            case 1://Death Knight
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 112);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 113);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 11);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 112://Death Knight
                AddGossipItemFor(player, 9, "Desecration", GOSSIP_SENDER_MAIN, 1121);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 1121://Death Knight - PVP Wrathful Gladiator
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51415, true);//Wrathful Gladiator's Dreadplate Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51414, true);//Wrathful Gladiator's Dreadplate Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51413, true);//Wrathful Gladiator's Dreadplate Chestpiece
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51416, true);//Wrathful Gladiator's Dreadplate Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51418, true);//Wrathful Gladiator's Dreadplate Shoulders
                CloseGossipMenuFor(player);
                break;

            case 113://Death Knight - PVE MENU
                AddGossipItemFor(player, 7, "Melee", GOSSIP_SENDER_MAIN, 114);
                AddGossipItemFor(player, 7, "Tank", GOSSIP_SENDER_MAIN, 115);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 114://Death Knight - PVE MELEE MENU
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 1141);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 1142);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 1143);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 1141://Death Knight - PVE MELEE Emblem
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51684, true);//Scourgelord Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51685, true);//Scourgelord Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51686, true);//Scourgelord Battleplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51683, true);//Scourgelord Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51682, true);//Scourgelord Shoulderplates
                CloseGossipMenuFor(player);
                break;

            case 1142://Death Knight - PVE MELEE Sanctified
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51127, true);//Sanctified Scourgelord Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51128, true);//Sanctified Scourgelord Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51129, true);//Sanctified Scourgelord Battleplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51126, true);//Sanctified Scourgelord Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51125, true);//Sanctified Scourgelord Shoulderplates
                CloseGossipMenuFor(player);
                break;

            case 1143://Death Knight - PVE MELEE Heoric Sanctified
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51312, true);//Heoric Sanctified Scourgelord Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51311, true);//Heoric Sanctified Scourgelord Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51310, true);//Heoric Sanctified Scourgelord Battleplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51313, true);//Heoric Sanctified Scourgelord Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51314, true);//Heoric Sanctified Scourgelord Shoulderplates
                CloseGossipMenuFor(player);
                break;

            case 115://Death Knight - PVE MELEE MENU
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 1151);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 1152);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 1153);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 1151://Death Knight - PVE Tank Emblem Scourgelord's Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51689, true);//Emblem Scourgelord Faceguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51688, true);//Emblem Scourgelord Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51687, true);//Emblem Scourgelord Chestguards
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51690, true);//Emblem Scourgelord Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51691, true);//Emblem Scourgelord Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 1152://Death Knight - PVE Tank Sanctified Scourgelord's Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51133, true);//Sanctified Scourgelord Faceguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51132, true);//Sanctified Scourgelord Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51134, true);//Sanctified Scourgelord Chestguards
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51131, true);//Sanctified Scourgelord Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51130, true);//Sanctified Scourgelord Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 1153://Death Knight - PVE Tank Heroic Sanctified Scourgelord's Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51306, true);//Heroic Sanctified Scourgelord Faceguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51307, true);//Heroic Sanctified Scourgelord Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51305, true);//Heroic Sanctified Scourgelord Chestguards
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51308, true);//Heroic Sanctified Scourgelord Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51309, true);//Heroic Sanctified Scourgelord Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 2://Druid
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 212);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 213);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 212://Druid PVP MENU
                AddGossipItemFor(player, 7, "Balance", GOSSIP_SENDER_MAIN, 214);
                AddGossipItemFor(player, 7, "Feral Combat", GOSSIP_SENDER_MAIN, 215);
                AddGossipItemFor(player, 7, "Restoration", GOSSIP_SENDER_MAIN, 216);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 213://Druid PVE MENU
                AddGossipItemFor(player, 7, "Restoration", GOSSIP_SENDER_MAIN, 2131);
                AddGossipItemFor(player, 7, "Feral", GOSSIP_SENDER_MAIN, 2132);
                AddGossipItemFor(player, 7, "Balance", GOSSIP_SENDER_MAIN, 2133);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 2131://Druid PVE Restoration
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 21311);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 21312);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 21313);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 21311://Druid PVE Restoration Emblem Lasherweave Garb
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51694, true);//Lasherweave Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51695, true);//Lasherweave Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51696, true);//Lasherweave Robes
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51693, true);//Lasherweave Legplaters
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51692, true);//Lasherweave Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 21312://Druid PVE Restoration Sanctified Lasherweave Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51699, true);//Lasherweave Headguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51698, true);//Lasherweave Handgrips
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51697, true);//Lasherweave Raiment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51700, true);//Lasherweave Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51701, true);//Lasherweave Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 21313://Druid PVE Restoration Heroic Sanctified Lasherweave Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51302, true);//Sanctified Lasherweave Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51301, true);//Sanctified Lasherweave Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51300, true);//Sanctified Lasherweave Robes
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51303, true);//Sanctified Lasherweave Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51304, true);//Sanctified Lasherweave Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 2132://Druid PVE Feral
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 21321);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 21322);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 21323);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 21321://Druid PVE Feral Emblem Lasherweave Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51699, true);//Lasherweave Headguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51698, true);//Lasherweave Handgrips
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51697, true);//Lasherweave Raiment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51700, true);//Lasherweave Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51701, true);//Lasherweave Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 21322://Druid PVE Feral Sanctified Lasherweave Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51143, true);//Sanctified Lasherweave Headguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51144, true);//Sanctified Lasherweave Handgrips
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51141, true);//Sanctified Lasherweave Raiment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51142, true);//Sanctified Lasherweave Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51140, true);//Sanctified Lasherweave Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 21323://Druid PVE Feral Heroic Sanctified Lasherweave Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51296, true);//Heroic Sanctified Lasherweave Headguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51295, true);//Heroic Sanctified Lasherweave Handgrips
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51298, true);//Heroic Sanctified Lasherweave Raiment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51297, true);//Heroic Sanctified Lasherweave Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51299, true);//Heroic Sanctified Lasherweave Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 2133://Druid PVE Balance
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 21331);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 21332);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 21333);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 21331://Druid PVE Balance Emblem Lasherweave Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51704, true);//Lasherweave Cover
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51703, true);//Lasherweave Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51702, true);//Lasherweave Vestment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51705, true);//Lasherweave Trousers
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51706, true);//Lasherweave Mantle
                CloseGossipMenuFor(player);
                break;

            case 21332://Druid PVE Balance Sanctified Lasherweave Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51149, true);//Sanctified Lasherweave Cover
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51148, true);//Sanctified Lasherweave Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51145, true);//Sanctified Lasherweave Vestment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51146, true);//Sanctified Lasherweave Trousers
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51147, true);//Sanctified Lasherweave Mantle
                CloseGossipMenuFor(player);
                break;

            case 21333://Druid PVE Balance Heroic Sanctified Lasherweave Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51290, true);//Sanctified Lasherweave Cover
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51291, true);//Sanctified Lasherweave Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51294, true);//Sanctified Lasherweave Vestment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51293, true);//Sanctified Lasherweave Trousers
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51292, true);//Sanctified Lasherweave Mantle
                CloseGossipMenuFor(player);
                break;

            case 214://Druid PVP Balance Wrathful Gladiator's Wildhide
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51435, true);//Wrathful Gladiator's Wyrmhide Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51434, true);//Wrathful Gladiator's Wyrmhide Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51433, true);//Wrathful Gladiator's Wyrmhide Robes
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51436, true);//Wrathful Gladiator's Wyrmhide Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51438, true);//Wrathful Gladiator's Wyrmhide Spaulders
                CloseGossipMenuFor(player);
                break;

            case 215://Druid PVP Feral Combat Wrathful Gladiator's Sanctuary
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51427, true);//Wrathful Gladiator's Dragonhide Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51426, true);//Wrathful Gladiator's Dragonhide Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51425, true);//Wrathful Gladiator's Dragonhide Robes
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51428, true);//Wrathful Gladiator's Dragonhide Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51430, true);//Wrathful Gladiator's Dragonhide Spaulders
                CloseGossipMenuFor(player);
                break;

            case 216://Druid PVP Restoration Wrathful Gladiator's Refuge
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51421, true);//Wrathful Gladiator's Kodohide Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51420, true);//Wrathful Gladiator's Kodohide Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51419, true);//Wrathful Gladiator's Kodohide Robes
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51422, true);//Wrathful Gladiator's Kodohide Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51424, true);//Wrathful Gladiator's Kodohide Spaulders
                CloseGossipMenuFor(player);
                break;

            case 3://Hunter
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 31);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 32);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 31://Hunter PVP MENU
                AddGossipItemFor(player, 9, "Pursuit", GOSSIP_SENDER_MAIN, 311);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 3);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 311://Hunter PVP Wrathful Gladiator's Pursuit
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51460, true);//Wrathful Gladiator's Chain Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51459, true);//Wrathful Gladiator's Chain Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51458, true);//Wrathful Gladiator's Chain Armor
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51461, true);//Wrathful Gladiator's Chain Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51462, true);//Wrathful Gladiator's Chain Spaulders
                CloseGossipMenuFor(player);
                break;

            case 32://Hunter PVP MENU
                AddGossipItemFor(player, 9, "Ranged", GOSSIP_SENDER_MAIN, 321);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 3);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 321://Hunter PVE RANGED Ahn'Kahar Blood Hunter's Battlegear
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 3211);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 3212);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 3213);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 3);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 3211://Hunter PVE RANGED Emblem Ahn'Kahar Blood Hunter's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51710, true);//Ahn'Kahar Blood Hunter's Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51711, true);//Ahn'Kahar Blood Hunter's  Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51707, true);//Ahn'Kahar Blood Hunter's  Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51709, true);//Ahn'Kahar Blood Hunter's Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51708, true);//Ahn'Kahar Blood Hunter's Spaulders
                CloseGossipMenuFor(player);
                break;

            case 3212://Hunter PVE RANGED Sanctified Ahn'Kahar Blood Hunter's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51153, true);//Sanctified Ahn'Kahar Blood Hunter's Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51154, true);//Sanctified Ahn'Kahar Blood Hunter's  Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51150, true);//Sanctified Ahn'Kahar Blood Hunter's  Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51152, true);//Sanctified Ahn'Kahar Blood Hunter's Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51151, true);//Sanctified Ahn'Kahar Blood Hunter's Spaulders
                CloseGossipMenuFor(player);
                break;

            case 3213://Hunter PVE RANGED Heroic Sanctified Ahn'Kahar Blood Hunter's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51286, true);//Heroic Sanctified Ahn'Kahar Blood Hunter's Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51285, true);//Heroic Sanctified Ahn'Kahar Blood Hunter's  Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51289, true);//Heroic Sanctified Ahn'Kahar Blood Hunter's  Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51287, true);//Heroic Sanctified Ahn'Kahar Blood Hunter's Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51288, true);//Heroic Sanctified Ahn'Kahar Blood Hunter's Spaulders
                CloseGossipMenuFor(player);
                break;

            case 4://Mage
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 41);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 42);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 41://Mage PVP MENU
                AddGossipItemFor(player, 9, "Caster", GOSSIP_SENDER_MAIN, 411);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 4);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 411://Mage PVP Wrathful Gladiator's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51465, true);//Wrathful Gladiator's Silk Cowl
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51464, true);//Wrathful Gladiator's Silk Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51463, true);//Wrathful Gladiator's Silk Raiment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51466, true);//Wrathful Gladiator's Silk Trousers
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51467, true);//Wrathful Gladiator's Silk Amice
                CloseGossipMenuFor(player);
                break;

            case 42://Mage PVE Caster Bloodmage's Regalia
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 421);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 422);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 423);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 4);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 421://Mage PVE Emblem Bloodmage's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51715, true);//Bloodmage Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51716, true);//Bloodmage Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51713, true);//Bloodmage Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51714, true);//Bloodmage Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51712, true);//Bloodmage Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 422://Mage PVE Sanctified Bloodmage's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51158, true);//Sanctified Bloodmage Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51159, true);//Sanctified Bloodmage Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51156, true);//Sanctified Bloodmage Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51157, true);//Sanctified Bloodmage Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51155, true);//Sanctified Bloodmage Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 423://Mage PVE Heroic Sanctified Bloodmage's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51281, true);//Heroic Sanctified Bloodmage Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51280, true);//Heroic Sanctified Bloodmage Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51283, true);//Heroic Sanctified Bloodmage Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51282, true);//Heroic Sanctified Bloodmage Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51284, true);//Heroic Sanctified Bloodmage Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 5://Paladin
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 51);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 52);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 51://Paladin
                AddGossipItemFor(player, 9, "Heal", GOSSIP_SENDER_MAIN, 511);
                AddGossipItemFor(player, 9, "Damage", GOSSIP_SENDER_MAIN, 512);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 5);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 511://Paladin PVP Heal Wrathful Gladiator's Redemption
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51470, true);//Wrathful Gladiator's Ornamented Headcover
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51469, true);//Wrathful Gladiator's Ornamented Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51468, true);//Wrathful Gladiator's Ornamented Chest Guard
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51471, true);//Wrathful Gladiator's Ornamented Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51473, true);//Wrathful Gladiator's Ornamented Spaulders
                CloseGossipMenuFor(player);
                break;

            case 512://Paladin PVP Damage Wrathful Gladiator's Vindication
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51476, true);//Wrathful Gladiator's Scaled Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51475, true);//Wrathful Gladiator's Scaled Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51474, true);//Wrathful Gladiator's Scaled Chestpiece
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51477, true);//Wrathful Gladiator's Scaled Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51479, true);//Wrathful Gladiator's Scaled Shoulders
                CloseGossipMenuFor(player);
                break;

            case 52://Paladin PVE MENU
                AddGossipItemFor(player, 9, "Holy", GOSSIP_SENDER_MAIN, 521);
                AddGossipItemFor(player, 9, "Protection", GOSSIP_SENDER_MAIN, 522);
                AddGossipItemFor(player, 9, "Retribution", GOSSIP_SENDER_MAIN, 523);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 5);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 521://Paladin PVE HOLY Lightsworn Garb
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 5211);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 5212);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 5213);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 5);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 5211://Paladin PVE Heal Emblem Lightsworn Garb
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51724, true);//Emblem Lightsworn Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51723, true);//Emblem Lightsworn Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51722, true);//Emblem Lightsworn Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51725, true);//Emblem Lightsworn Greaves
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51726, true);//Emblem Lightsworn Spaulders
                CloseGossipMenuFor(player);
                break;

            case 5212://Paladin PVE Heal Sanctified Lightsworn Garb
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51167, true);//Sanctified Lightsworn Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51169, true);//Sanctified Lightsworn Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51165, true);//Sanctified Lightsworn Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51168, true);//Sanctified Lightsworn Greaves
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51166, true);//Sanctified Lightsworn Spaulders
                CloseGossipMenuFor(player);
                break;

            case 5213://Paladin PVE Heal Heroic Sanctified Lightsworn Garb
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51162, true);//Heroic Sanctified Lightsworn Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51163, true);//Heroic Sanctified Lightsworn Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51164, true);//Heroic Sanctified Lightsworn Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51161, true);//Heroic Sanctified Lightsworn Greaves
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51160, true);//Heroic Sanctified Lightsworn Spaulders
                CloseGossipMenuFor(player);
                break;

            case 522://Paladin PVE Protection Lightsworn Plate
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 5221);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 5222);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 5223);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 5);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 5221://Paladin PVE Protection Emblem Lightsworn Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51729, true);//Emblem Lightsworn Faceguard 
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51728, true);//Emblem Lightsworn Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51727, true);//Emblem Lightsworn Chestguard
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51730, true);//Emblem Lightsworn Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51731, true);//Emblem Lightsworn Shoulderguards
                CloseGossipMenuFor(player);
                break;

            case 5222://Paladin PVE Protection Sanctified Lightsworn Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51173, true);//Sanctified Lightsworn Faceguard 
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51172, true);//Sanctified Lightsworn Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51174, true);//Sanctified Lightsworn Chestguard
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51171, true);//Sanctified Lightsworn Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51170, true);//Sanctified Lightsworn Shoulderguards
                CloseGossipMenuFor(player);
                break;

            case 5223://Paladin PVE Protection Heroic Sanctified Lightsworn Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51266, true);//Heroic Sanctified Lightsworn Faceguard 
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51267, true);//Heroic Sanctified Lightsworn Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51265, true);//Heroic Sanctified Lightsworn Chestguard
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51268, true);//Heroic Sanctified Lightsworn Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51269, true);//Heroic Sanctified Lightsworn Shoulderguards
                CloseGossipMenuFor(player);
                break;

            case 523://Paladin PVE Retribution Lightsworn Battlegear
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 5231);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 5232);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 5233);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 5);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 5231://Paladin PVE Retribution Emblem Lightsworn Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51729, true);//Emblem Lightsworn Faceguard 
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51728, true);//Emblem Lightsworn Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51727, true);//Emblem Lightsworn Chestguard
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51730, true);//Emblem Lightsworn Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51731, true);//Emblem Lightsworn Shoulderguards
                CloseGossipMenuFor(player);
                break;

            case 5232://Paladin PVE Retribution Sanctified Lightsworn Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51162, true);//Sanctified Lightsworn Helmet 
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51163, true);//Sanctified Lightsworn Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51164, true);//Sanctified Lightsworn Battleplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51161, true);//Sanctified Lightsworn Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51160, true);//Sanctified Lightsworn Shoulderplates
                CloseGossipMenuFor(player);
                break;

            case 5233://Paladin PVE Retribution Heroic Sanctified Lightsworn Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51277, true);//Heroic Sanctified Lightsworn Helmet 
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51276, true);//Heroic Sanctified Lightsworn Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51275, true);//Heroic Sanctified Lightsworn Battleplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51278, true);//Heroic Sanctified Lightsworn Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51279, true);//Heroic Sanctified Lightsworn Shoulderplates
                CloseGossipMenuFor(player);
                break;

            case 6://Priest
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 61);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 62);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 61://Priest PVP Menu
                AddGossipItemFor(player, 9, "Heal", GOSSIP_SENDER_MAIN, 611);
                AddGossipItemFor(player, 9, "Damage", GOSSIP_SENDER_MAIN, 612);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 6);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 611://Paladin PVP Heal Wrathful Gladiator's Investiture
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51484, true);//Wrathful Gladiator's Mooncloth Hood 
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51483, true);//Wrathful Gladiator's Mooncloth Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51482, true);//Wrathful Gladiator's Mooncloth Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51485, true);//Wrathful Gladiator's Mooncloth Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51486, true);//Wrathful Gladiator's Mooncloth Mantle
                CloseGossipMenuFor(player);
                break;

            case 612://Paladin PVP Damage Wrathful Gladiator's Raiment
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51489, true);//Wrathful Gladiator's Satin Hood 
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51488, true);//Wrathful Gladiator's Satin Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51487, true);//Wrathful Gladiator's Satin Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51490, true);//Wrathful Gladiator's Satin Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51491, true);//Wrathful Gladiator's Satin Mantle
                CloseGossipMenuFor(player);
                break;

            case 62://Priest PVE Menu
                AddGossipItemFor(player, 9, "Caster", GOSSIP_SENDER_MAIN, 621);
                AddGossipItemFor(player, 9, "Healer", GOSSIP_SENDER_MAIN, 622);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 6);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 621://Priest PVE Caster
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 6211);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 6212);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 6213);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 6);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 6211://Priest PVE Caster Emblem Crimson Acolyte's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51737, true);//Emblem Crimson Acolyte Cowl
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51738, true);//Emblem Crimson Acolyte Handwraps
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51740, true);//Emblem Crimson Acolyte Raiments
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51741, true);//Emblem Crimson Acolyte Pants
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51739, true);//Emblem Crimson Acolyte Mantle
                CloseGossipMenuFor(player);
                break;

            case 6212://Priest PVE Caster Sanctified Crimson Acolyte's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51184, true);//Sanctified Crimson Acolyte Cowl
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51183, true);//Sanctified Crimson Acolyte Handwraps
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51180, true);//Sanctified Crimson Acolyte Raiments
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51181, true);//Sanctified Crimson Acolyte Pants
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51182, true);//Sanctified Crimson Acolyte Mantle
                CloseGossipMenuFor(player);
                break;

            case 6213://Priest PVE Caster Heroic Sanctified Crimson Acolyte's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51255, true);//Heroic Sanctified Crimson Acolyte Cowl
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51256, true);//Heroic Sanctified Crimson Acolyte Handwraps
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51259, true);//Heroic Sanctified Crimson Acolyte Raiments
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51258, true);//Heroic Sanctified Crimson Acolyte Pants
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51257, true);//Heroic Sanctified Crimson Acolyte Mantle
                CloseGossipMenuFor(player);
                break;

            case 622://Priest PVE Healer
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 6221);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 6222);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 6223);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 6);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 6221://Priest PVE Healer Emblem Crimson Acolyte's Raiment
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51736, true);//Emblem Crimson Acolyte Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51735, true);//Emblem Crimson Acolyte Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51733, true);//Emblem Crimson Acolyte Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51732, true);//Emblem Crimson Acolyte LEggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51734, true);//Emblem Crimson Acolyte Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 6222://Priest PVE Healer Sanctified Crimson Acolyte's Raiment
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51178, true);//Sanctified Crimson Acolyte Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51179, true);//Sanctified Crimson Acolyte Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51176, true);//Sanctified Crimson Acolyte Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51177, true);//Sanctified Crimson Acolyte LEggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51175, true);//Sanctified Crimson Acolyte Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 6223://Priest PVE Healer Heroic Sanctified Crimson Acolyte's Raiment
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51261, true);//Heroic Sanctified Crimson Acolyte Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51260, true);//Heroic Sanctified Crimson Acolyte Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51263, true);//Heroic Sanctified Crimson Acolyte Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51262, true);//Heroic Sanctified Crimson Acolyte Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51264, true);//Heroic Sanctified Crimson Acolyte Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 7://Rogue
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 71);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 72);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 71://Rogue PVP Menu
                AddGossipItemFor(player, 9, "Vestments", GOSSIP_SENDER_MAIN, 711);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 7);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 711://Rogue PVP Wrathful Gladiator's Vestments
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51494, true);//Wrathful Gladiator's Leather Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51493, true);//Wrathful Gladiator's Leather Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51492, true);//Wrathful Gladiator's Leather Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51495, true);//Wrathful Gladiator's Leather Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51496, true);//Wrathful Gladiator's Leather Spaulders
                CloseGossipMenuFor(player);
                break;

            case 72://Rogue PVE Menu
                AddGossipItemFor(player, 9, "Melee", GOSSIP_SENDER_MAIN, 721);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 7);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 721://Rogue PVE Melee
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 7211);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 7212);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 7213);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 7);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 7211://Rogue PVE Melee Emblem Shadowblade's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51745, true);//Emblem Shadowblade Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51742, true);//Emblem Shadowblade Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51743, true);//Emblem Shadowblade Breastplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51744, true);//Emblem Shadowblade Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51746, true);//Emblem Shadowblade Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 7212://Rogue PVE Melee Sanctified Shadowblade's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51187, true);//Sanctified Shadowblade Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51188, true);//Sanctified Shadowblade Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51189, true);//Sanctified Shadowblade Breastplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51186, true);//Sanctified Shadowblade Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51185, true);//Sanctified Shadowblade Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 7213://Rogue PVE Melee Heroic Sanctified Shadowblade's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51252, true);//Heroic Sanctified Shadowblade Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51251, true);//Heroic Sanctified Shadowblade Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51250, true);//Heroic Sanctified Shadowblade Breastplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51253, true);//Heroic Sanctified Shadowblade Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51254, true);//Heroic Sanctified Shadowblade Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 8://Shaman
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 81);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 82);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 81://Shaman PVP Menu
                AddGossipItemFor(player, 9, "Elemental", GOSSIP_SENDER_MAIN, 811);
                AddGossipItemFor(player, 9, "Enhancement", GOSSIP_SENDER_MAIN, 812);
                AddGossipItemFor(player, 9, "Restoration", GOSSIP_SENDER_MAIN, 813);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 8);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 811://Shaman PVP Elemental Wrathful Gladiator's Thunderfist
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51511, true);//Wrathful Gladiator's Mail Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51510, true);//Wrathful Gladiator's Mail Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51509, true);//Wrathful Gladiator's Mail Armor
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51512, true);//Wrathful Gladiator's Mail Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51514, true);//Wrathful Gladiator's Mail Spaulders
                CloseGossipMenuFor(player);
                break;

            case 812://Shaman PVP Enhancement Wrathful Gladiator's Earthshaker
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51505, true);//Wrathful Gladiator's Linked Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51504, true);//Wrathful Gladiator's Linked Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51503, true);//Wrathful Gladiator's Linked Armor
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51506, true);//Wrathful Gladiator's Linked Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51508, true);//Wrathful Gladiator's Linked Spaulders
                CloseGossipMenuFor(player);
                break;

            case 813://Shaman PVP Restoration Wrathful Gladiator's Wartide
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51499, true);//Wrathful Gladiator's Ringmail Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51498, true);//Wrathful Gladiator's Ringmail Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51497, true);//Wrathful Gladiator's Ringmail Armor
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51500, true);//Wrathful Gladiator's Ringmail Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51502, true);//Wrathful Gladiator's Ringmail Spaulders
                CloseGossipMenuFor(player);
                break;

            case 82://Shaman PVE Menu
                AddGossipItemFor(player, 9, "Restoration", GOSSIP_SENDER_MAIN, 821);
                AddGossipItemFor(player, 9, "Enhancement", GOSSIP_SENDER_MAIN, 822);
                AddGossipItemFor(player, 9, "Elemental", GOSSIP_SENDER_MAIN, 823);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 8);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 821://Rogue PVE Restoration
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 8211);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 8212);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 8213);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 8);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 8211://Shaman PVE Restoration Emblem Frost Witch's Garb
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51749, true);//Emblem Frost Witch Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51748, true);//Emblem Frost Witch Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51747, true);//Emblem Frost Witch Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51750, true);//Emblem Frost Witch Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51751, true);//Emblem Frost Witch Spaulders
                CloseGossipMenuFor(player);
                break;

            case 8212://Shaman PVE Restoration Sanctified Frost Witch's Garb
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51192, true);//Sanctified Frost Witch Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51191, true);//Sanctified Frost Witch Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51190, true);//Sanctified Frost Witch Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51193, true);//Sanctified Frost Witch Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51194, true);//Sanctified Frost Witch Spaulders
                CloseGossipMenuFor(player);
                break;

            case 8213://Shaman PVE Restoration Heroic Sanctified Frost Witch's Garb
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51247, true);//Heroic Sanctified Frost Witch Headpiece
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51248, true);//Heroic Sanctified Frost Witch Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51249, true);//Heroic Sanctified Frost Witch Tunic
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51246, true);//Heroic Sanctified Frost Witch Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51245, true);//Heroic Sanctified Frost Witch Spaulders
                CloseGossipMenuFor(player);
                break;

            case 822://Rogue PVE Enhancement
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 8221);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 8222);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 8223);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 8);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 8221://Shaman PVE Enhancement Emblem Frost Witch's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51754, true);//Emblem Frost Witch Faceguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51753, true);//Emblem Frost Witch Grips
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51752, true);//Emblem Frost Witch Chestguard
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51755, true);//Emblem Frost Witch War-Kilt
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51756, true);//Emblem Frost Witch Shoulderguard
                CloseGossipMenuFor(player);
                break;

            case 8222://Shaman PVE Enhancement Sanctified Frost Witch's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51197, true);//Sanctified Frost Witch Faceguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51196, true);//Sanctified Frost Witch Grips
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51195, true);//Sanctified Frost Witch Chestguard
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51198, true);//Sanctified Frost Witch War-Kilt
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51199, true);//Sanctified Frost Witch Shoulderguard
                CloseGossipMenuFor(player);
                break;

            case 8223://Shaman PVE Enhancement Heroic Sanctified Frost Witch's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51242, true);//Heroic Sanctified Frost Witch Faceguard
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51243, true);//Heroic Sanctified Frost Witch Grips
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51244, true);//Heroic Sanctified Frost Witch Chestguard
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51241, true);//Heroic Sanctified Frost Witch War-Kilt
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51240, true);//Heroic Sanctified Frost Witch Shoulderguard
                CloseGossipMenuFor(player);
                break;

            case 823://Rogue PVE Elemental
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 8231);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 8232);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 8233);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 8);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 8231://Shaman PVE Elemental Emblem Frost Witch's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51759, true);//Emblem Frost Witch Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51758, true);//Emblem Frost Witch Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51757, true);//Emblem Frost Witch Hauberk
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51760, true);//Emblem Frost Witch Kilt
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51761, true);//Emblem Frost Witch Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 8232://Shaman PVE Elemental Sanctified Frost Witch's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51202, true);//Sanctified Frost Witch Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51201, true);//Sanctified Frost Witch Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51200, true);//Sanctified Frost Witch Hauberk
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51203, true);//Sanctified Frost Witch Kilt
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51204, true);//Sanctified Frost Witch Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 8233://Shaman PVE Elemental Heroic Sanctified Frost Witch's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51237, true);//Heroic Sanctified Frost Witch Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51238, true);//Heroic Sanctified Frost Witch Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51239, true);//Heroic Sanctified Frost Witch Hauberk
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51236, true);//Heroic Sanctified Frost Witch Kilt
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51235, true);//Heroic Sanctified Frost Witch Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 9://Warlock
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 91);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 92);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 91://Warlock PVP Menu
                AddGossipItemFor(player, 9, "Felshroud", GOSSIP_SENDER_MAIN, 911);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 9);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 911://Warlock PVP Wrathful Gladiator's Felshroud
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51538, true);//Wrathful Gladiator's Felweave Crowl
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51537, true);//Wrathful Gladiator's Felweave Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51536, true);//Wrathful Gladiator's Felweave Raiment
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51539, true);//Wrathful Gladiator's Felweave Trousers
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51540, true);//Wrathful Gladiator's Felweave Amice
                CloseGossipMenuFor(player);
                break;

            case 92://Warlock PVE Menu
                AddGossipItemFor(player, 9, "Caster", GOSSIP_SENDER_MAIN, 921);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 9);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 921://Warlock PVE Caster
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 9211);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 9212);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 9213);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 9);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 9211://Warlock PVE Emblem Dark Coven's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51765, true);//Emblem Dark Coven Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51766, true);//Emblem Dark Coven Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51763, true);//Emblem Dark Coven Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51764, true);//Emblem Dark Coven Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51762, true);//Emblem Dark Coven Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 9212://Warlock PVE Sanctified Dark Coven's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51208, true);//Sanctified Dark Coven Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51209, true);//Sanctified Dark Coven Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51206, true);//Sanctified Dark Coven Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51207, true);//Sanctified Dark Coven Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51205, true);//Sanctified Dark Coven Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 9213://Warlock PVE Heroic Sanctified Dark Coven's Regalia
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51231, true);//Heroic Sanctified Dark Coven Hood
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51230, true);//Heroic Sanctified Dark Coven Gloves
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51233, true);//Heroic Sanctified Dark Coven Robe
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51232, true);//Heroic Sanctified Dark Coven Leggings
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51234, true);//Heroic Sanctified Dark Coven Shoulderpads
                CloseGossipMenuFor(player);
                break;

            case 10://Warrior
                AddGossipItemFor(player, 9, "PvP [Wrathful]", GOSSIP_SENDER_MAIN, 101);
                AddGossipItemFor(player, 9, "PvE [Tier 10]", GOSSIP_SENDER_MAIN, 102);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 101://Warrior PVP Menu
                AddGossipItemFor(player, 9, "Battlegear", GOSSIP_SENDER_MAIN, 1011);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 10);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 1011://Warrior PVP Wrathful Gladiator's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51543, true);//Wrathful Gladiator's Plate Helm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51542, true);//Wrathful Gladiator's Plate Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51541, true);//Wrathful Gladiator's Plate Chestpiece
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51544, true);//Wrathful Gladiator's Plate Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51545, true);//Wrathful Gladiator's Plate Shoulders
                CloseGossipMenuFor(player);
                break;

            case 102://Warrior PVE Menu
                AddGossipItemFor(player, 9, "Melee", GOSSIP_SENDER_MAIN, 1021);
                AddGossipItemFor(player, 9, "Tank", GOSSIP_SENDER_MAIN, 1022);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 10);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 1021://Warrior PVE Melee
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 10211);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 10212);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 10213);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 10);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 10211://Warrior PVE Emblem Ymirjar Lord's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51769, true);//Emblem Ymirjar Lord's Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51770, true);//Emblem Ymirjar Lord's Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51771, true);//Emblem Ymirjar Lord's Battleplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51768, true);//Emblem Ymirjar Lord's Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51767, true);//Emblem Ymirjar Lord's Shoulderplates
                CloseGossipMenuFor(player);
                break;

            case 10212://Warrior PVE Sanctified Ymirjar Lord's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51212, true);//Sanctified Ymirjar Lord's Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51213, true);//Sanctified Ymirjar Lord's Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51214, true);//Sanctified Ymirjar Lord's Battleplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51211, true);//Sanctified Ymirjar Lord's Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51210, true);//Sanctified Ymirjar Lord's Shoulderplates
                CloseGossipMenuFor(player);
                break;

            case 10213://Warrior PVE Heroic Sanctified Ymirjar Lord's Battlegear
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51227, true);//Heroic Sanctified Ymirjar Lord's Helmet
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51226, true);//Heroic Sanctified Ymirjar Lord's Gauntlets
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51225, true);//Heroic Sanctified Ymirjar Lord's Battleplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51228, true);//Heroic Sanctified Ymirjar Lord's Legplates
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51229, true);//Heroic Sanctified Ymirjar Lord's Shoulderplates
                CloseGossipMenuFor(player);
                break;

            case 1022://Warrior PVE Melee
                AddGossipItemFor(player, 9, "Emblem", GOSSIP_SENDER_MAIN, 10221);
                AddGossipItemFor(player, 9, "Sanctified", GOSSIP_SENDER_MAIN, 10222);
                AddGossipItemFor(player, 9, "Heroric Sanctified", GOSSIP_SENDER_MAIN, 10223);
                AddGossipItemFor(player, 7, "[Back]", GOSSIP_SENDER_MAIN, 10);
                AddGossipItemFor(player, 7, "[Exit]", GOSSIP_SENDER_MAIN, 1111);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                break;

            case 10221://Warrior PVE Emblem Ymirjar Lord's Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51774, true);//Emblem Ymirjar Lord's Greathelm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51773, true);//Emblem Ymirjar Lord's Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51772, true);//Emblem Ymirjar Lord's Breastplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51775, true);//Emblem Ymirjar Lord's Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51776, true);//Emblem Ymirjar Lord's Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 10222://Warrior PVE Sanctified Ymirjar Lord's Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51218, true);//Sanctified Ymirjar Lord's Greathelm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51217, true);//Sanctified Ymirjar Lord's Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51219, true);//Sanctified Ymirjar Lord's Breastplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51216, true);//Sanctified Ymirjar Lord's Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51215, true);//Sanctified Ymirjar Lord's Pauldrons
                CloseGossipMenuFor(player);
                break;

            case 10223://Warrior PVE Heroic Sanctified Ymirjar Lord's Plate
                if (!player->HasEnoughMoney(int32(EXC1 * GOLD)))//gold check
                    return true;
                player->ModifyMoney(int32(-EXC1 * GOLD));//Deducting the money if check passes
                player->CastSpell(player, 47292);//Level up visual effect to let you know the transaction did occure and money is deducted
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 51221, true);//Heroic Sanctified Ymirjar Lord's Greathelm
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 51222, true);//Heroic Sanctified Ymirjar Lord's Handguards
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 51220, true);//Heroic Sanctified Ymirjar Lord's Breastplate
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 51223, true);//Heroic Sanctified Ymirjar Lord's Legguards
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 51224, true);//Heroic Sanctified Ymirjar Lord's Pauldrons
                CloseGossipMenuFor(player);
                break;

            default:

                break;
            }
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
    return new npc_equipAI(creature);
    }
};


void AddSC_npc_equip()
{
    new Trinitycore_Equipment_Exchange_announce();
    new npc_equip();
}
