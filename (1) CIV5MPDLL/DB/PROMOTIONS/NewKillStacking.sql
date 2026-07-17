--******************** Per Kill Stacking Bonuses ********************--
-- 每击杀永久进攻加成（百分位：50=每2击杀+1%进攻加成）
alter table UnitPromotions add PerKillAttackMod integer default 0;
-- 每击杀永久防御加成（百分位）
alter table UnitPromotions add PerKillDefenseMod integer default 0;
-- 每击杀永久基础近战战斗力百分比
alter table UnitPromotions add PerKillBaseCombatMod integer default 0;
-- 每击杀永久基础远程战斗力百分比
alter table UnitPromotions add PerKillRangedCombatMod integer default 0;
-- 每击杀永久最大生命百分比
alter table UnitPromotions add PerKillMaxHpMod integer default 0;
-- 每击杀造成伤害固定增加值
alter table UnitPromotions add PerKillInflictDamageChange integer default 0;
-- 每击杀受到伤害固定减少值
alter table UnitPromotions add PerKillDefenseDamageChange integer default 0;
