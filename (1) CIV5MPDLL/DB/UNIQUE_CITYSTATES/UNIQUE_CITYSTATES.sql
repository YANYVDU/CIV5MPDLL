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

-- ==================== Unique CityState UA System (Rule 1-18) ====================

-- Effects definition table (internal data, not shown to players): one row per ally or friend effect
-- Columns are grouped and annotated by source city-state; 0 means the effect does not apply
CREATE TABLE CityStateUAEffects (
    ID                                              INTEGER PRIMARY KEY AUTOINCREMENT,
    Type                                            TEXT NOT NULL UNIQUE,
    Help                                            TEXT DEFAULT NULL REFERENCES Language_en_US(Tag),
    -- Florence: reduces the incremental rise of faith-bought great people cost (applies to the increment only, not the full cost)
    FaithPurchaseGreatPeopleCostRiseModifier       integer DEFAULT 0,
    FaithPurchaseGreatPeopleCostRiseModifierPerGW  integer DEFAULT 0,
    FaithPurchaseAllGreatPeople                     boolean DEFAULT 0,
    -- Buenos Aires: great musician does not die after a great work and retains concert tourism
    GPNoDeathAfterGreatWork                         boolean DEFAULT 0,
    GPConcertTourismRetentionPercent                integer DEFAULT 0,
    -- Brussels: special great musician concert modifiers
    GreatMusicianConcertTourismModifier             integer DEFAULT 0,
    GreatMusicianConcertGoldPercent                 integer DEFAULT 0,
    -- Bratislava: capital and second capital
    CapitalAndSecondCapitalCultureModifier          integer DEFAULT 0,
    -- Kyiv: capital accumulates per turn
    CapitalCultureModifierPerTurn                   integer DEFAULT 0,
    CapitalFaithModifierPerTurn                     integer DEFAULT 0,
    CapitalPerTurnYieldModifierMax                  integer DEFAULT 0,
    -- Bucharest: international migration
    ImmigrationRatePerImmigrant                     integer DEFAULT 0,
    ImmigrationRateMax                              integer DEFAULT 0,
    EmigrationRatePerImmigrant                      integer DEFAULT 0,
    EmigrationRateMax                               integer DEFAULT 0,
    -- Kuala Lumpur: puppet city science threshold
    PuppetNoTechCostPenalty                         boolean DEFAULT 0,
    PuppetTechCostPartial                           integer DEFAULT 0,
    -- Almaty: can pillage neutral trade routes
    CanPillageNeutralTradeRoute                     boolean DEFAULT 0,
    -- Belgrade: garrison city defense
    GarrisonCityDefenseModifier                     integer DEFAULT 0,
    -- Budapest: immune to river crossing penalties
    LandUnitsImmuneRiverCrossing                    boolean DEFAULT 0,
    -- Hanoi: fixed damage in borders + peace treaty + being declared war on
    EnemyFixedDamageModifierInBorders               integer DEFAULT 0,
    CulturePerWarPeace                              integer DEFAULT 0,
    EnemyCombatModifierInBordersPerBeenDoW          integer DEFAULT 0,
    -- Mbanza-Kongo: city-count related
    UnitProductionModifierPerCity                   integer DEFAULT 0,
    ManpowerPerCity                                 integer DEFAULT 0,
    CombatBonusPerTechDifference                    integer DEFAULT 0,
    -- Sidon: naval siege + heal outside borders
    NavalAttackIgnoreBuildingDefense                integer DEFAULT 0,
    ForeignRegenPercent                             integer DEFAULT 0,
    -- Sofia: hills cities
    HillsCityDamageReduction                        integer DEFAULT 0,
    HillsMovementModifier                           integer DEFAULT 0,
    HillsCityRangeBonus                             integer DEFAULT 0,
    -- Vatican City: religion spread speed
    ReligionSpreadSpeedModifier                     integer DEFAULT 0,
    -- Kyzyl: trade route cap -> trade route distance
    LandTradeRouteDistancePerTradeSlot              integer DEFAULT 0,
    -- Dubai: donation counting
    HappinessPerGoldDonated                         integer DEFAULT 0,
    GoldDonationInterval                            integer DEFAULT 0,
    WonderProductionPerDonationHappiness            integer DEFAULT 0,
    -- Malacca: luxury happiness
    LuxuryHappinessModifier                         integer DEFAULT 0
);

-- UA type table (shown to players): pairs a city-state's ally and friend effects
-- Description = ally effect text  Help = friend effect text, combined in the UI layer
CREATE TABLE CityStateUAs (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Type TEXT NOT NULL UNIQUE,
    Description TEXT DEFAULT NULL REFERENCES Language_en_US(Tag),
    Help TEXT DEFAULT NULL REFERENCES Language_en_US(Tag),
    AllyEffectType TEXT NOT NULL REFERENCES CityStateUAEffects(Type),
    FriendEffectType TEXT NOT NULL REFERENCES CityStateUAEffects(Type)
);

-- MinorCivilizations reference UAType directly (no mapping table needed)
alter table MinorCivilizations add column UAType text default null references CityStateUAs(Type);
-- CityState UA: adds per-turn Great Person Points (per SpecialistType) to all cities
create table CityStateUAEffect_GreatPersonPoints (
    EffectType text references CityStateUAEffects(Type),
    SpecialistType text references Specialists(Type),
    Points integer default 0
);

-- CityState UA: born great person grants extra specialist yield (per SpecialistType)
create table CityStateUAEffect_BornGreatPersonSpecialistYield (
    EffectType text references CityStateUAEffects(Type),
    SpecialistType text references Specialists(Type),
    UnitClassType text references UnitClasses(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

create table CityStateUAEffect_BuildingGreatPersonPoints (
    EffectType text references CityStateUAEffects(Type),
    BuildingClassType text references BuildingClasses(Type),
    SpecialistType text references Specialists(Type),
    Points integer default 0
);

create table CityStateUAEffect_BornGreatPersonAllyInfluenceMod (
    EffectType text references CityStateUAEffects(Type),
    UnitClassType text references UnitClasses(Type),
    ModPerBorn integer default 0
);
