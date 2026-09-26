-- ******************** UNIQUE CITYSTATES：DiplomaticPrestige******************** --
alter table Buildings add DiplomaticPrestige int default 0;
alter table Policies  add DiplomaticPrestige int default 0;
alter table Traits    add DiplomaticPrestige int default 0;
-- League project rewards: one-shot Diplomatic Prestige (e.g. World Trade Organization tier 2)
alter table LeagueProjectRewards add DiplomaticPrestige int default 0;

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

-- Economic Aid (Super Power V11): base length of one global aid round, scaled by game speed
INSERT INTO Defines (Name, Value) VALUES ('ECONOMIC_AID_ROUND_LENGTH', 20);


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
    -- Belgrade: ally-built military units gain XP (purchases do not apply)
    MilitaryUnitProductionXP                        integer DEFAULT 0,
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
    -- Sidon: attacker ignores this % of the defended city's building defense (30 ally / 15 friend)
    CityAttackIgnoreBuildingDefensePercent          integer DEFAULT 0,
    -- Sidon: % modifier on the per-turn XP granted by allied militaristic city-states (100 = +100%)
    MilitaryXPPerTurnModifier                       integer DEFAULT 0,
    -- Sidon: when non-zero, the militaristic city-state per-turn XP also applies to sea and air domains
    MilitaryXPSeaAir                                integer DEFAULT 0,
    -- Sofia: hills cities
    HillsCityDamageReduction                        integer DEFAULT 0,
    HillsMovementModifier                           integer DEFAULT 0,
    HillsCityRangeBonus                             integer DEFAULT 0,
    -- Vatican City: religion spread speed
    ReligionSpreadSpeedModifier                     integer DEFAULT 0,
    -- Vatican City: Papal Recognition - league delegate votes granted to each civilization with a majority of cities following the ally's religion (mainstream votes, 1 = +1 vote)
    PapalRecognitionVotes                           integer DEFAULT 0,
    -- Vatican City: Papal Recognition - league delegate votes granted to the ally per following civilization (including itself, 1 = +1 vote per follower)
    PapalRecognitionAllyVotes                       integer DEFAULT 0,
    -- Jerusalem: +X% religious pressure of the founder's religion per holy city owned by the ally/friend
    ReligiousPressureModifierPerHolyCity            integer DEFAULT 0,
    -- Jerusalem: player who is the ally of this city-state cannot be denounced
    DenounceImmunity                                boolean DEFAULT 0,
    -- Kyzyl: trade route cap -> trade route distance
    LandTradeRouteDistancePerTradeSlot              integer DEFAULT 0,
    -- Dubai: donation counting
    HappinessPerGoldDonated                         integer DEFAULT 0,
    GoldDonationInterval                            integer DEFAULT 0,
    WonderProductionPerDonationHappiness            integer DEFAULT 0,
    IdeologyPressurePerDonationHappiness            integer DEFAULT 0,
    -- Genoa: each sea trade route grants a bonus % to influence gained from gold donations (1 = +1% per sea route)
    GoldDonationInfluenceModifierPerSeaRoute        integer DEFAULT 0,
    -- Malacca: luxury happiness
    LuxuryHappinessModifier                         integer DEFAULT 0,
    -- Malacca: food surplus per happy luxury type (value = per-type food% * 100; 每1=100, 每2=50)
    FoodKeptModifierPerLuxury                           integer DEFAULT 0,
    -- Malacca: trade route gold per luxury type (value = per-type gold% * 100; 每1=100)
    TradeRouteGoldModifierPerLuxuryType             integer DEFAULT 0,
    -- Panama: international trade route gold per distance tile (value = per-tile gold% * 100; 每1=100, 每2=50)
    TradeRouteGoldModifierPerDistance               integer DEFAULT 0,
    -- Panama: population unhappiness reduction per cross-continental trade route (value = per-route % * 100; 每1=100, cap 90)
    UnhappinessReductionPerCrossContinentRoute      integer DEFAULT 0,
    -- Valletta: enemy city besieged by >= this many of our combat units cannot heal
    EnemyCityNoHealBesiegeCount                      integer DEFAULT 0,
    -- Prague: killing an enemy spy grants spy progress toward a new spy (100 = kill 1 gain 1, 20 = kill 5 gain 1)
    SpyKillGainSpyProgress                           integer DEFAULT 0,
    -- Antananarivo: coastal city food growth threshold modifier (ordinary modifier, -20 = -20%)
    CoastalCityGrowthThresholdModifier              integer DEFAULT 0,
    -- Antananarivo: diplomatic prestige per city (value = prestige * 100; 10 = 0.1 prestige per city)
    DiplomaticPrestigePerCity                       integer DEFAULT 0,
    -- Vilnius: Mod-type golden-age threshold change per population (applied before percentage modifiers; negative = lower threshold, -100 = -1 per 1 population)
    GoldenAgeThresholdPerPopulation                 integer DEFAULT 0,
    -- Sofia: coup/espionage spy UA (columns on the effect table, one row per ally/friend effect)
    CoupChanceModifier          integer DEFAULT 0,  -- +% coup success, may exceed the 85% cap (ally 30 / friend 12)
    CoupFailSpySurvives         boolean DEFAULT 0,  -- failed coup keeps the spy alive (ally only)
    StealTechSpeedPerSpy        integer DEFAULT 0,  -- +% steal-tech speed per alive spy (ally only)
    SpyKillChancePerSpy         integer DEFAULT 0,  -- +% chance to catch/kill enemy spies per alive spy (ally only)
    -- Gangtok: per city worldwide following the player's religion, global happiness (100 = +1 happiness per city)
    HappinessPerFollowingCity   integer DEFAULT 0,
    -- Gangtok: ally may buy influence at ANY city-state with faith, at (gold price / divisor) faith
    -- (divisor > 0 also enables the feature; 4 = 1/4 of the gold price)
    FaithInfluencePurchaseCostDivisor  integer DEFAULT 0,
    -- Gangtok: how many faith influence purchases the ally may make per turn (1 = once per turn, globally)
    FaithInfluencePurchasePerTurnLimit integer DEFAULT 0,
    -- Wittenberg: ally may buy a self-chosen belief and add it to the ally-led religion (once per major)
    FaithBeliefPurchase boolean DEFAULT 0,
    -- Wittenberg: when this religion is cleansed from a city-state city by an inquisitor/great prophet, keep this % of the followers
    InquisitorRetentionPercent integer DEFAULT 0,
    -- La Venta: ally may buy an idle pantheon belief and add it to the ally-led religion (price doubles per purchase)
    FaithPantheonPurchase boolean DEFAULT 0,
    -- La Venta: +X% great-person rate per masterpiece/artifact the ally owns
    GreatPersonRateModifierPerGreatWork integer DEFAULT 0,
    -- Kathmandu: the first gold donation each turn refunds this % of the amount as faith to the ally
    FaithRefundPerDonationPercent integer DEFAULT 0,
    -- Geneva: diplomatic prestige per major civilization whose majority religion is the ally-led religion
    -- (value = prestige * 100 per civ; 50 = +1 prestige per 2 civs, integer division gives "per 2 civs +1")
    DiplomaticPrestigePerMajorityCiv integer DEFAULT 0,
    -- Geneva: per-turn influence with each met city-state, one unit per FollowingCityDivisor following cities
    -- (value = influence * 100; 100 = +1 influence per unit). Returned via GetFriendshipChangePerTurnTimes100,
    -- so it drives both the real DoFriendship settlement and the Lua influence-trend UI from the same source.
    InfluencePerTurnPerFollowCityMod integer DEFAULT 0,
    -- Geneva: how many cities following the ally-led religion produce one influence unit (3 = one per 3 cities)
    FollowingCityDivisor integer DEFAULT 0,
    -- Vancouver: global happiness per coastal city owned by the ally/friend (100 = +1 happiness per coastal city)
    CoastalCityHappiness integer DEFAULT 0,
    -- Yerevan: global happiness per worked holy-site improvement (IMPROVEMENT_HOLY_SITE), accumulated in GetHappinessFromMinorCivs
    -- (basis points, 100 = +1 global happiness per worked holy site; no local-population cap). Ally = 300.
    HolySiteHappiness integer DEFAULT 0
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

-- Kathmandu CS UA: special buildings (e.g. Everest Camp) declare a prereq effect id that gates construction
-- (must run before any mod XML inserts rows into Buildings with this column)
ALTER TABLE Buildings ADD 'PrereqEffect' TEXT DEFAULT NULL;
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

create table CityStateUAEffect_BuildingClassYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    BuildingClassType text references BuildingClasses(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Brussels): specified specialist's great person point accumulation rate (%)
create table CityStateUAEffect_SpecialistPointRate (
    EffectType text references CityStateUAEffects(Type),
    SpecialistType text references Specialists(Type),
    Rate integer default 0
);

-- CityState UA (Brussels): each great work of a class (literature/art/music) grants great person points (Rate=100 => 1 great work = 1 point)
create table CityStateUAEffect_GreatWorkGreatPersonPoints (
    EffectType text references CityStateUAEffects(Type),
    GreatWorkClassType text references GreatWorkClasses(Type),
    SpecialistType text references Specialists(Type),
    Rate integer default 0,
    CapitalOnly boolean default 0
);

-- CityState UA (Brussels): specified unit class's one-shot great person output modifier (%)
create table CityStateUAEffect_GreatPersonOneShotModifier (
    EffectType text references CityStateUAEffects(Type),
    UnitClassType text references UnitClasses(Type),
    Modifier integer default 0
);

-- CityState UA (Colombo): the international trade route (InternalTR) ending at this city-state (UCS)
-- grants the origin player a flat per-era yield (Era+1 times YieldValue)
create table CityStateUAEffect_InternalTRToUCSPerEraYield (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    YieldValue integer default 0
);

-- City State UA (Colombo): for cities running a trade route to this city-state (UCS), a percentage of
-- the input yield (InYieldType) is granted as extra output yield (OutYieldType); the original input yield is unchanged
create table CityStateUAEffect_YieldToYieldViaTRToUCS (
    EffectType text references CityStateUAEffects(Type),
    InYieldType text references Yields(Type),
    OutYieldType text references Yields(Type),
    Percent integer default 0
);

-- CityState UA (Valletta): buying the specified building class grants all units of the specified domain XP
create table CityStateUAEffect_PurchasedBuildingXP (
    EffectType text references CityStateUAEffects(Type),
    BuildingClassType text references BuildingClasses(Type),
    DomainType text references Domains(Type),
    XP integer default 0
);

-- CityState UA (Valletta): born unit of the specified unit class grants a configurable yield equal to
-- YieldMod% of the player's influence with the specified city-state (MinorCivType)
create table CityStateUAEffect_UnitBornYield (
    EffectType text references CityStateUAEffects(Type),
    MinorCivType text references MinorCivilizations(Type),
    UnitClassType text references UnitClasses(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- MinorCivilizations free building classes (analogous to Civilization_FreeBuildingClasses):
-- grants the building class to the city-state's first city, also at game start
create table MinorCivilization_FreeBuildingClasses (
    MinorCivType text references MinorCivilizations(Type),
    BuildingClassType text references BuildingClasses(Type)
);

-- CityState UA (Prague): a city with our own spy garrisoned in it grants a configurable
-- yield percentage modifier (per YieldType) to that city, e.g. YIELD_SCIENCE / 20 = +20% science
create table CityStateUAEffect_SpyGarrisonYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Melbourne): a city that owns the specified resource (must be improved with its
-- standard improvement) grants a yield percentage modifier per YieldType, e.g. RESOURCE_GOLD / YIELD_GOLD / 50 = +50% gold
create table CityStateUAEffect_ResourceYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    ResourceType text references Resources(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Bogota): named special-city predicates, reused by several effects.
-- The ID column is REQUIRED: the C++ side loads this table through PrefetchCollection,
-- which issues "SELECT * FROM <table> WHERE ID > -1 ORDER BY ID" and FNEWs one entry per row.
create table CityStateUAEffect_SpecialCityTypes (
    ID   INTEGER PRIMARY KEY AUTOINCREMENT,
    Type text not null unique
);

-- A city type matches when: EVERY And-row matches AND (the Or table is empty OR at least one Or-row matches).
-- Both are CHILD tables (read via "where SpecialCityType = ?"), so neither needs an ID column.
-- ConditionType is a fixed enum parsed in CvSpecialCityTypeEntry::CacheResults; Value holds the
-- resource/feature type name for value-carrying conditions (NULL for boolean ones).
-- Currently HAS_RESOURCE / HAS_FEATURE / IS_RIVER / IS_COASTAL are implemented; add enum branches
-- for future kinds (CAPITAL / HILLS / HAS_BUILDINGCLASS / trade-route kinds / continent kinds).
create table CityStateUAEffect_SpecialCityTypeConditionsOr (
    SpecialCityType text references CityStateUAEffect_SpecialCityTypes(Type),
    ConditionType   text,
    Value           text
);

create table CityStateUAEffect_SpecialCityTypeConditionsAnd (
    SpecialCityType text references CityStateUAEffect_SpecialCityTypes(Type),
    ConditionType   text,
    Value           text
);

-- A city matching the special city type grants a yield percentage modifier to that city.
-- SPECIAL_CITY_BOGOTA_LUXURY / YIELD_CULTURE / 35 = +35% culture for cities of that type.
-- Modifiers ACCUMULATE: when one city matches several special city types, every matching row of the
-- same YieldType is summed (CvCity::GetBaseYieldRateModifier adds without breaking). Example -- Riga's
-- river+coastal city matches RIVER and RIVER_COASTAL, so gold/food get +15% + 15% = +30%.
-- NOTE: YieldMod here is a PLAIN PERCENT (35 means +35%), NOT basis points -- the opposite convention
-- from CityStateUAEffect_BornGreatPersonYieldModifiers (100 = +1%). CvCity::GetBaseYieldRateModifier
-- adds this value straight to its percent total, whereas the born-great-person table is divided by 100.
create table CityStateUAEffect_SpecialCityYieldModifiers (
    EffectType      text references CityStateUAEffects(Type),
    SpecialCityType text references CityStateUAEffect_SpecialCityTypes(Type),
    YieldType       text references Yields(Type),
    YieldMod        integer default 0
);

-- For each owned city matching the special city type, ALL cities gain a yield percentage modifier.
-- SPECIAL_CITY_BOGOTA_LUXURY / YIELD_TOURISM / 5 = +5% tourism per qualifying city
-- NOTE: YieldMod here is a PLAIN PERCENT (5 means +5% per qualifying city), NOT basis points.
-- CvPlayer::GetCSUAYieldPercentModifier multiplies it by 100 because that function accumulates basis
-- points and divides by 100 at the end; do not "pre-convert" the stored value.
create table CityStateUAEffect_SpecialCityCountYieldModifiers (
    EffectType      text references CityStateUAEffects(Type),
    SpecialCityType text references CityStateUAEffect_SpecialCityTypes(Type),
    YieldType       text references Yields(Type),
    YieldMod        integer default 0
);

-- CityState UA (Antananarivo): each worked plot holding the specified improvement
-- grants a yield percentage modifier to the city (e.g. MINE / YIELD_GOLD / 3 = +3% gold per worked mine)
create table CityStateUAEffect_ImprovementYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    ImprovementType text references Improvements(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Zanzibar): each worked plot holding the specified improvement
-- grants flat local happiness to the city (e.g. PLANTATION / 2 = +2 local happiness per worked plantation)
create table CityStateUAEffect_ImprovementHappiness (
    EffectType text references CityStateUAEffects(Type),
    ImprovementType text references Improvements(Type),
    Happiness integer default 0
);

-- CityState UA (Genoa): each friendly city-state grants a yield percentage modifier per YieldType
-- (e.g. YIELD_SCIENCE / 100 = +1% science per friendly city-state; Rate=100 => +1%)
create table CityStateUAEffect_FriendCityStateYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Genoa): each allied city-state grants a yield percentage modifier per YieldType
-- (e.g. YIELD_SCIENCE / 200 = +2% science per allied city-state; Rate=100 => +1%)
create table CityStateUAEffect_AllyCityStateYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Vilnius): each unlocked social policy grants a yield percentage modifier per YieldType
-- (e.g. YIELD_GOLD / 200 = +2% gold per unlocked policy; Rate=100 => +1%)
create table CityStateUAEffect_PolicyYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Jerusalem/Wittenberg): for each city worldwide following the player's religion,
-- the player's capital gains +Modifier% of YieldType (Modifier=100 => +1% per following city)
create table CityStateUAEffect_CapitalYieldModifierPerFollowingCity (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    Modifier integer default 0
);

-- CityState UA (Vatican): for each city worldwide following the player's religion,
-- the player's religion's holy city gains +Modifier% of YieldType (Modifier=100 => +1% per following city, e.g. YIELD_TOURISM / 100 = +1% tourism per city)
create table CityStateUAEffect_HolyCityYieldModifierPerFollowingCity (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    Modifier integer default 0
);

-- CityState UA (Sydney): each immigrant received grants a yield percentage modifier per YieldType
-- (Modifier=100 => +1% per immigrant received, e.g. YIELD_CULTURE / 400 = +4% culture per immigrant)
create table CityStateUAEffect_ImmigrantYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    Modifier integer default 0
);

-- CityState UA (Sydney): each immigrant received grants cash equal to CashPercent% of the treasury,
-- capped at CashCapBase x (current era + 1) x game-speed culture percent / 100
-- (e.g. CashPercent=1 / CashCapBase=100 => +1% of treasury, cap = 100 x era x speed)
create table CityStateUAEffect_ImmigrantCashReward (
    EffectType text references CityStateUAEffects(Type),
    CashPercent integer default 0,
    CashCapBase integer default 0
);

-- CityState UA (Hormuz): each unit of surplus strategic resource grants trade-route gold %
-- (per ResourceType; Modifier in basis points (percent x 100), e.g. Modifier 400 = +4% gold per surplus oil;
--  surplus = max(0, getNumResourceAvailable), never negative)
create table CityStateUAEffect_TradeRouteGoldPerSurplusResource (
    EffectType text references CityStateUAEffects(Type),
    ResourceType text references Resources(Type),
    Modifier integer default 0
);

-- CityState UA (Vancouver): each point of the player's net happiness grants a yield percentage
-- modifier per YieldType, capped per yield. YieldMod is in basis points (100 = +1% per happiness);
-- Cap is in percent (50 = +50% maximum). Applies to e.g. YIELD_FOOD and YIELD_TOURISM.
create table CityStateUAEffect_HappinessYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0,
    Cap integer default 0
);

-- CityState UA (Ife): per-unitclass discount on the FAITH cost of buying great people. CostRiseModifier
-- is in percent applied to the final price (negative = discount), keyed by UnitClassType so it affects
-- only specified great people classes (e.g. UNITCLASS_ARTIST/WRITER/MUSICIAN). -30 = -30% final faith cost.
create table CityStateUAEffect_FaithGPClassCostModifier (
    EffectType text references CityStateUAEffects(Type),
    UnitClassType text references UnitClasses(Type),
    CostRiseModifier integer default 0
);

-- CityState UA (Ife): each great work / artifact of the specified GreatWorkClassType grants a yield
-- percentage modifier per YieldType, nation-wide. YieldMod in basis points (100 = +1% per great work);
-- e.g. GREAT_WORK_ARTIFACT / YIELD_FAITH / 200 = +2% faith nation-wide per artifact owned.
create table CityStateUAEffect_GreatWorkYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    GreatWorkClassType text references GreatWorkClasses(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Ife): while the player is in a golden age, grant a yield percentage modifier per YieldType
-- nation-wide. YieldMod is a plain percent (25 = +25% faith during golden age), not basis points.
create table CityStateUAEffect_GoldenAgeYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Yerevan): literacy rate (owned techs / total techs x 100, integer percent points)
-- grants a nation-wide yield percentage modifier, keyed by YieldType so it can apply to any yield
-- (YieldMod is basis points per literacy point; ally 100 = +1% production per point, friend 50 = +1% per 2 points).
create table CityStateUAEffect_LiteracyYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Yerevan): each born great person of the specified UnitClassType grants a nation-wide
-- yield percentage modifier per YieldType (YieldMod is basis points per born great person;
-- ally prophet: UNITCLASS_PROPHET / YIELD_FAITH / 500 = +5% faith per born prophet).
create table CityStateUAEffect_BornGreatPersonYieldModifiers (
    EffectType text references CityStateUAEffects(Type),
    UnitClassType text references UnitClasses(Type),
    YieldType text references Yields(Type),
    YieldMod integer default 0
);

-- CityState UA (Yerevan): if this plot is an improvement and an adjacent plot's improvement is
-- AdjacentImprovementType, this plot gains +Yield of YieldType. Direction matches the vanilla
-- *_AdjacentImprovementYieldChanges family (neighbor improvement -> this improvement).
-- ImprovementType is the LOCAL (affected) improvement and MUST be a concrete type; rows are meant to be
-- fully enumerated for every improvement via SP SQL (INSERT...SELECT FROM Improvements + an AFTER INSERT
-- trigger, same approach as SP_AdjacentImprovementYieldChangesForNewImproments). No wildcard supported.
-- Ally: each <ImprovementType> / IMPROVEMENT_HOLY_SITE / YIELD_CULTURE / 1 = +1 culture to that improved
-- plot when it borders a holy site. Mirrors the Policy/Trait/Building adjacent-improvement family.
create table CityStateUAEffect_AdjacentImprovementYieldChanges (
    EffectType text references CityStateUAEffects(Type),
    ImprovementType text references Improvements(Type),
    AdjacentImprovementType text references Improvements(Type),
    YieldType text references Yields(Type),
    Yield integer default 0
);
