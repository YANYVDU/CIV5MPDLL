-- ******************** UNIQUE CITYSTATES：DiplomaticPrestige******************** --
alter table Buildings add DiplomaticPrestige int default 0;
alter table Policies  add DiplomaticPrestige int default 0;
alter table Traits    add DiplomaticPrestige int default 0;

-- Diplomatic Overextension Penalty Ratios (GlobalDefines, Rule 20)
INSERT INTO Defines (Name, Value) VALUES ('DIPLOMATIC_OVEREXTENSION_DECAY_MODIFIER', 10);
INSERT INTO Defines (Name, Value) VALUES ('DIPLOMATIC_OVEREXTENSION_RISE_MODIFIER', -10);
INSERT INTO Defines (Name, Value) VALUES ('DIPLOMATIC_OVEREXTENSION_UNHAPPINESS_MODIFIER', 3);

-- City-State UA Basic Effects (GlobalDefines, per-ally modifier values)
-- Cultured
INSERT INTO Defines (Name, Value) VALUES ('CS_CULTURED_POLICY_COST_MODIFIER', -2);
INSERT INTO Defines (Name, Value) VALUES ('CS_CULTURED_IMMIGRATION_REGRESSAND_MODIFIER', -2);
-- Militaristic
INSERT INTO Defines (Name, Value) VALUES ('CS_MILITARISTIC_LAND_XP_PER_TURN', 1);
-- Maritime
INSERT INTO Defines (Name, Value) VALUES ('CS_MARITIME_SEA_TRADE_GOLD_PER_ERA', 1);
-- Religious
INSERT INTO Defines (Name, Value) VALUES ('CS_RELIGIOUS_FAITH_COST_MODIFIER', -5);
INSERT INTO Defines (Name, Value) VALUES ('CS_RELIGIOUS_PRESSURE_MODIFIER', 10);
-- Mercantile
INSERT INTO Defines (Name, Value) VALUES ('CS_MERCANTILE_LUXURY_HAPPINESS_MODIFIER', 5);
INSERT INTO Defines (Name, Value) VALUES ('CS_MERCANTILE_TREASURY_INTEREST_RATE', 1);
INSERT INTO Defines (Name, Value) VALUES ('CS_TREASURY_INTEREST_CAP_MULTIPLIER', 100);


-- MinorCivAlliesThresholdExtra: per-era ally threshold increase (Rule 8)
alter table Eras add column MinorCivAlliesThresholdExtra int default 0;

-- MinorCivAlliesThresholdModifier: per-building/policy/trait threshold modifier
alter table Buildings add column MinorCivAlliesThresholdModifier int default 0;
alter table Policies  add column MinorCivAlliesThresholdModifier int default 0;
alter table Traits    add column MinorCivAlliesThresholdModifier int default 0;
