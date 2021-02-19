Use world;

Set @NpcName = "Pick a spec",
	@NpcSubname = "Trinitycore",
	@NpcEntry = 55002,
	@NpcDisplayID = 31833,
	@NpcLevel = 80;

INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `spell_school_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES 
(@NpcEntry, 0, 0, 0, 0, 0, @NpcDisplayID, 0, 0, 0, @NpcName, @NpcSubname, NULL, 0, @NpcLevel, @NpcLevel, 0, 35, 1, 1, 1.14286, 1, 0, 0, 2000, 2200, 1, 1, 2, 0, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 50, 50, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'TemplateNPC', 0);

INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES 
(55002, 'Here you can select a character template which will Gear, Gem, Spec, and finish glyphs for your toon instantly.\r\n\r\nSelect your spec:', 'Here you can select a character template which will Gear, Gem, Spec, and finish glyphs for your toon instantly.\r\n\r\nSelect your spec:');

INSERT INTO `command` (`name`, `help`) VALUES 
('templatenpc reload', 'Syntax: .templatenpc reload\nType .templatenpc reload to reload TemplateNPC custom script.');