INSERT INTO Defines(Name, Value) VALUES('ORIGINAL_CAPITAL_MODMAX', 10);
INSERT INTO Defines(Name, Value) VALUES('BONUS_PER_ADJACENT_FRIEND_RANGED', 0);

INSERT INTO Defines(Name, Value) VALUES('HEALTH_DISEASE_CONNECTION_MOD', 10);
INSERT INTO Defines(Name, Value) VALUES('HEALTH_DISEASE_TRADE_MOD',10);
INSERT INTO Defines(Name, Value) VALUES('FRESH_WATER_HEALTH_YIELD', 2);
INSERT INTO Defines(Name, Value) VALUES('AI_CITIZEN_VALUE_HEALTH', 7);

INSERT INTO Defines(Name, Value) VALUES('VERY_UNHAPPY_DISEASE_PENALTY_PER_UNHAPPY', 2);
INSERT INTO Defines(Name, Value) VALUES('VERY_UNHAPPY_MAX_DISEASE_PENALTY', 100);
INSERT INTO Defines(Name, Value) VALUES('VERY_UNHAPPY_CRIME_PENALTY_PER_UNHAPPY', 2);
INSERT INTO Defines(Name, Value) VALUES('VERY_UNHAPPY_MAX_CRIME_PENALTY', 100);

INSERT INTO Defines(Name, Value) VALUES('CITY_CRIME_SPY_YIELD', 6);
INSERT INTO Defines(Name, Value) VALUES('CITY_CRIME_OPINION_REVOLUTIONARY_WAVE_YIELD', 10);
INSERT INTO Defines(Name, Value) VALUES('CITY_CRIME_OPINION_CIVIL_RESISTANCE_YIELD', 4);
INSERT INTO Defines(Name, Value) VALUES('CITY_CRIME_OPINION_DISSIDENTS_YIELD', 1);
INSERT INTO Defines(Name, Value) VALUES('CITY_CRIME_GOLDEN_AGE_YIELD',-10);
INSERT INTO Defines(Name, Value) VALUES('CITY_LOYALTY_GOLDEN_AGE_YIELD', -10);

-- Spy points system: threshold progression (base 100, +50 per spy)
INSERT INTO Defines(Name, Value) VALUES('SPY_POINTS_THRESHOLD_BASE', 100);
INSERT INTO Defines(Name, Value) VALUES('SPY_POINTS_THRESHOLD_INCREASE', 50);

alter table Features add PseudoNaturalWonder integer default 0;
alter table Features add Volcano boolean default 0;

alter table HandicapInfos add StrategicResourceMod integer default 100;
alter table HandicapInfos add StrategicResourceModPerEra integer default 0;
-- If AIFirstProphetPercent is positive, it overrides AITrainPercent on AI's First Prophet Cost
alter table HandicapInfos add AIFirstProphetPercent integer default 0;

alter table Worlds add ExtraCityDistance integer default 0;
create table World_HandicapExtraAIStartingUnit (
    WorldType text references Worlds(Type),
    HandicapType text references HandicapInfos(Type),
    ExtraAIStartingUnit integer default 0
);

alter table GameSpeeds add FreePromotion text references UnitPromotions(Type);
alter table GameSpeeds add SetterExtraPercent integer default 0;

alter table Processes add DefenseValue integer default 0;

alter table Projects add FreePromotion text references UnitPromotions(Type);
alter table Projects add NoBroadcast boolean default 0;
alter table Projects add PolicyBranchPrereq text references PolicyBranchTypes(Type);
create table Project_PolicyNeeded (
    ProjectType text references Projects(Type),
    PolicyType text references Policies(Type)
);

alter table Builds add ObsoleteTech text default null;
create table Build_ResourceRemove (
    BuildType text references Builds(Type),
    ResourceType text references Resources(Type)
);

-- Build action modifies plot terrain (e.g., land reclamation/canal building)
-- Empty/NULL TerrainType means this build does not involve terrain modification
create table Build_TerrainChange (
    BuildType text references Builds(Type),
    TerrainType text references Terrains(Type)
);

-- Terrains where the build action is allowed (whitelist)
-- When records exist, the build is only available on listed terrains; unrestricted when no records
create table Build_ValidTerrains (
    BuildType text references Builds(Type),
    TerrainType text references Terrains(Type)
);

-- Build action requires adjacent terrain (at least one match among 6 adjacent plots)
-- When records exist, the requirement must be met; unrestricted when no records
create table Build_AdjacentTerrainRequired (
    BuildType text references Builds(Type),
    TerrainType text references Terrains(Type)
);

-- Tech-triggered automatic building upgrade rules.
-- One UpgradeGroup upgrades at most once per city; SPriority decides which old
-- building is reported when several are present (lower = higher priority).
-- Human players always upgrade; AI players follow HandicapInfos.AIAutoBuildingUpgrade.
create table Building_AutoUpgrade (
    TechType text NOT NULL references Technologies(Type),
    UpgradeGroup text NOT NULL,
    OldBuildingClass text NOT NULL references BuildingClasses(Type),
    NewBuildingClass text NOT NULL references BuildingClasses(Type),
    SPriority integer NOT NULL default 0,
    primary key (TechType, UpgradeGroup, OldBuildingClass)
);

-- 1 = AI players on this handicap also run the automatic building upgrade.
-- Human players are unaffected by this column.
alter table HandicapInfos add AIAutoBuildingUpgrade boolean default 0;

